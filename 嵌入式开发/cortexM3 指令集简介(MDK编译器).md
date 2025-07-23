
### 寄存器
通用寄存器
- `r0-r3`：参数传递/临时值（类似MIPS的$a0-$a3）
- `r4-r11`：需保存的寄存器（类似MIPS的$s0-$s7）
- `r13 (sp)`：栈指针（类似MIPS的$sp）
- `r14 (lr)`：链接寄存器（存储返回地址）
- `r15 (pc)`：程序计数器
状态寄存器
- `xPSR`：程序状态寄存器（包含条件标志）
	- `IPSR`：当前服务中断号寄存器
	- `EPSP`：执行状态寄存器
	- `APSR`：上条指令结果的标志(ZVCS)
	- `PSR`：APSR+EPSR+IPSR
- 中断屏蔽寄存器
	- `PSIMASK`：只能为 0 和 1，后者时关闭可屏蔽异常，只响应 NVI 和硬 fault
	- `FAULTMASK`：只能为 0 和 1，后者时关闭所有异常与软中断，只响应 NVI
	- `BASEPRI`：除了为 0 时不关闭任何中断，关闭优先级大于该值的中断，不影响 fault
- `CONTROL`：控制寄存器，堆栈选择，特权等级
### 指令

### 基本指令
```armasm
; 数据移动指令
mov r0, #0x20        ; 立即数加载
mov r1, r0           ; 寄存器间移动
ldr r0, [r1]         ; 内存加载 (32位)，r0 = *r1 (类似MIPS: lw $t0, 0($t1))
ldr r0, [r1, #4]     ; r0 = *(r1+4) 
str r0, [r1]         ; 内存存储 (32位)，*r1 = r0
str r0, [r1, #-8]!   ; 先r1-=8, 再*r1=r0 (预索引)
;其它
ldrh r0, [r1]        ; 16 位加载
ldrb r0, [r1]        ; 8 位加载
strh r0, [r1]        ; 16 位存储
strb r0, [r1]        ; 8 位存储
; 高、低位常量加载，仅支持立即数寻址
movw r0, #0x1234     ; 低16位
movt r0, #0x5678     ; 高16位 -> r0=0x56781234

; 算数指令
add r0, r1, r2       ; r0 = r1 + r2
add r0, r1, #10      ; r0 = r1 + 10
adc r0, r1, r2       ; 带进位加法
sub r0, r1, r2       ; r0 = r1 - r2
sbc r0, r1, r2       ; 带借位减肥
mul r0, r1, r2       ; r0 = r1 * r2
and r0, r1, r2       ; r0 = r1 & r2 (按位与)
orr r0, r1, r2       ; r0 = r1 | r2 (按位或)
eor r0, r1, r2       ; r0 = r1 ^ r2 (按位异或)
bic r0, r1, r2       ; r0 = r1 & ~r2 (位清除)
mvn r0, r1           ; r0 = ~r1 (按位取反)
neg r0, r1           ; r0 = -r1 (算数取负)
; 位移指令，可用立即数和寄存器表示 Rs，但寄存器必须为低寄存器(R0-R7)
lsl r0, r1, r2       ; 逻辑左移
lsr r0, r1, r2       ; 逻辑右移
asr r0, r1, r2       ; 算术右移
ror r0, r1, r2       ; 循环右移
; 不常见的算数指令
rev r0, r1           ; 反转字节序
revh r0, r1          ; 分为高 16 位和低 16 位，每 16 位数中反转字节序
revsh r0, r1         ; 把一个 32 位寄存器的低 16 位半字进行字节反转，然后带符号扩展到 32 位
sxtb r0, r1          ; 带符号扩展一个字节到有符号数
sxth r0, r1          ; 带符号扩展一个半字到有符号数
uxtb r0, r1          ; 无符号扩展一个字节到有符号数
uxth r0, r1          ; 无符号扩展一个半字到有符号数

rev r0, r1           ; 反转字节序
revh r0, r1          ; 分为高 16 位和低 16 位，每 16 位数中反转字节序
revsh r0, r1         ; 把一个 32 位寄存器的低 16 位半字进行字节反转，然后带符号扩展到 32 位


; 控制流指令
b   label            ; 跳转到标签 (类似MIPS: j label)
bl  function_label   ; 函数调用，跳转并保存返回地址到lr (类似MIPS: jal)
bx  r0               ; 类似 b，通过寄存器保存目标地址，根据目标地址的最低位决定切换到 ARM 或 Thumb 状态
blx  r0              ; 类似 bl，通过寄存器保存目标地址，可能切换 ARM 或 Thumb 状态
; bx 与 blx 的跳转范围大于 b 与 bl
; 条件跳转 (基于xPSR标志)
cmp r0, r1           ; 比较r0和r1 (设置标志)
cmn r0, r1           ; 负向比较，比较 r0 与 -r1
beq label            ; 相等则跳转 (Z=1)
bne label            ; 不等则跳转 (Z=0)
bgt label            ; 大于则跳转 (Z=0 & N=V)
; 条件码
; EQ NE LT LE GT GE 
; LS 无符号小于或等于 HI 无符号大于或等于
; CS/HS 进位标志为 1 CC/LO 进位标志为 0
; MI 负数标志为 1 PL 负数标志为 0
; VS 溢出标志为 1 VC 溢出标志为 0

; 入、出栈 (多个寄存器)
push {r0, r4, lr}    ; 等价于 stmdb sp!, {r0,r4,lr}
pop {r0, r4, pc}     ; 出栈并返回 (恢复lr到pc)

```

ARM 特色指令

```armasm
; 条件执行
addeq r0, r1, r2    ; 仅当Z=1时执行加法
movne r0, #0         ; 仅当Z=0时执行移动

; 批量加载/存储
ldmia r0!, {r1-r4}   ; r1=*r0, r2=*(r0+4),... r0自动增加
stmdb sp!, {r5-r8}   ; 压栈多个寄存器

; IT 指令
; 格式 I<x><y><z> <cond>
; 其中 <x>,<y>,<z> 可为 T 或 E, <cond> 为条件码
itee ge
mov r0, #1 ; ge 时执行
mov r0, #2 ;
mov r0, #3

```

编译器提示
**AREA**
```armasm
label AREA sectionname{,attr}{,attr}...
```
- `sectionname`：段名（如 `.text`, `.data`, `.stack` 等）
- `attr`：段属性（可多个）：
    - `CODE` - 代码段
    - `DATA` - 数据段
    - `READONLY` - 只读
    - `READWRITE` - 读写
    - `ALIGN=n` - 按 2^n 字节对齐（如 `ALIGN=3` 表示 8 字节对齐）
    - `NOINIT` - 不初始化

**EXPORT/IMPORT DCD/DCW/DCB**
```armasm
	EXPORT symbol [WEAK] ; [weak]弱定义(允许被覆盖)，可不加
	IMPORT symbol [WEAK] ; [weak]弱引用(符号不存在不报错)，可不加

__Vectors
	DCD 0x0              ; 4字节
	DCW 0x0              ; 2字节
	DCB 0x0              ; 1字节

	ALIGN 4              ; 强制 4 字节对齐
	
```
标签
```armasm

```


特权指令
```armasm
mrs <gp_reg> <special_reg> ; 读取特殊寄存器
msr <special_reg> <gp_reg> ; 写入特殊寄存器

; 特权级下，这些寄存器才可写
BASEPRI
FAULTMASK
PRIMASK
CONTROL

NVIC->STIR
```

原子指令
```armasm

```
### C/C++ 函数调用标准协议

该协议的建议与要求包括
- **参数传递规则**：
	- c 函数的前 4 个参数通过 r0-r3 传递，之后的参数通过栈传递。
	- 用寄存器传参时，小结构体(32位可存)，可通过寄存器传递，大结构体入栈，寄存器存地址
- 要求
	- 使用 R4-R11 前必须先入栈保存它们

### ARM Tips

4 字节栈对齐
- ARM AAPCS规范要求栈、堆必须4字节对齐，SP寄存器的值必须是 4 字节对齐。
- Cortex-M3/M4内核硬件要求中断时栈必须8字节对齐

对齐规则
- **硬件最低要求**：4 字节，防止总线错误
- **AAPCS 标准**：8 字节，确保跨架构（arm范畴下）兼容性、支持 64 位操作
关于对齐，最佳实践是 8 字节对齐。