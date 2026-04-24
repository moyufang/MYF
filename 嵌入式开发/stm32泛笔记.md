# stm32 开发基础

注意事项：
（1）DC005供电范围：6~24V
（2）IO口不要接大于 5V 的电压，否则会报废板子
（3）不要用 IO 口直接驱动感性负载（电机、电磁阀、继电器等）
（4）防静电

### 调试

JTAG 与 SWD

MDK5 + MDK6

### 最小系统设计

- 供电
- 复位
- 时钟
- boot 启动
- 下载电路
- 后备电池

### stm32 复位与启动流程

在上电或复位后，cortex M3 会从 `0x0000_0000` 读出 `MSP`，然后从 `0x0000_0004` 读出 `reset_handler` 的入口地址，最后执行 `reset_handler` 函数。

所以要保证 `cortex M3` 能从 `0x0000_0000` 访问向量表，然而代码可能在不同存储器上，ST 的解决方案是对 `0x0000_0000` 做地址映射，在 STM32F1/F4 里，可以通过 BOOT[1:0] 引脚选择三种不同启动模式。

| Boot1 | Boot0 | 启动模式   | 映射地址                          |
| ----- | ----- | ------ | ----------------------------- |
| X     | 0     | 主闪存存储器 | `0x0800_0000` → `0x0000_0000` |
| 0     | 1     | 系统存储器  | `0x1FFF_0000` → `0x0000_0000` |
| 1     | 1     | SRAM   | `0x2000_0000` → `0x0000_0000` |

### 程序烧录

常见有 USB+串口、JTAG、SWD

usb 串口烧录流程
- **硬件连接** 将开发板上 uart 的 RX 与 TX 分别与 mcu 的指定串口的 TX 与  RX 分别连接起来，在正点原子的板子上是将 RXD 与 PA9 以及 TXD 与 PA10 连接起来。
- **软件配置** 使用软件串口烧录软件，如 FlyMcu，选择相应的 Port 与 bps（波特率），stmf1 系列支持的最高波特率是 460800，stmf4 是 76800。
- **标准方法(可选)**
	- 把 B0 接 V3.3（保持 B1 接 GND），按下复位键
	- 点击烧录
	- 将 B0 接回 GND，按下复位键
- **一键烧录** 标准方法需要麻烦地动两次跳线帽，flymcu 利用串口的 DTR 和 RTS 信号，分别控制 STM32 的复位和 B0，实现一键下载，但需要在 flymcu 中选择选项 DTR 的低电平复位，RTS 高电平进 BootLoader 。
flymcu 下载是全片擦除的，较 ST link 慢。

```
- DTR（Data Terminal Ready，数据终端就绪）：通常由主机（如电脑）发出，表示自身已准备好通信。在 FlyMcu 中，通过设置 DTR 为低电平来触发 STM32 的复位（NRST）引脚。
  
- RTS（Request to Send，请求发送）：通常用于请求对方允许发送数据。这里通过设置 RTS 为高电平来使 Boot0（B0）引脚进入高电平状态，从而引导芯片进入系统存储器中的 BootLoader 模式。
```

JTAG/SWD 烧录流程（包括 ST-Link、J-Link、U-Link、DAP-Link）
- 将调试器接入板子。
- 打开 Keil 工程，进入 `Options for Target` → `Debug` 选项卡，选择调试器。
- 点击 Settings，在 Port 选项选择 JTAG 或者 SW，选择时钟（默认 4MHz）
- MDK 上点击 Download(F8) 按钮烧录。

JTAG 支持更复杂的调试功能，适用复杂芯片的调试（FPGA、多核），速度较慢。
SWD 适合 STM32 等 ARM Cortex-M 常规调试，速度更快。

### stm32 工程搭建

##### 方案一、基于 MDK 的手动创建工程

- 选择芯片型号，这里是 stm32f103zet6
- 根据型号选取 ARM 公司提供的基本文件，最小工程包含如下文件，除了 `main.c` 都可以在 ST 公司提供的固件库中找到。
```
main.c
startup_stm32f103_hd.s
stm32f10x.h
system_stm32f1xx.h
system_stm32f1xx.c
core_cm3.h
core_cm3.c
```
其中 `main.c` 内容如下
```c
#include "stm32f10x.h"  // 包含设备寄存器定义

int main(void) {
	// SystemInit();
	int i = 0;

	while (1) {
		i++;
	}
}
```

- 程序烧录与 Debug

ST 公司提供的 startup 文件的核心功能包括：初始化堆栈、设置向量表、初始化.data/.bss段、跳转到main。

##### 方案二、STM32CubeMX 生成基础工程

待补充

### ALIENTEK 开发板实验分类
- **基础**：GPIO、AFIO、中断、按钮输入
- **时钟**：TIM 定时器、RTC实时时钟、待机唤醒、IWDG&WWDG看门狗
- **通信**：串口通信、IIC、SPI、CAN、USB、无线通信、网络通信
- **数模**：ADC、DAC、PWM
- **传感**：内部温度、光敏、温度、湿度、红外遥感、MPU6050 六轴传感器
- **存储**：DMA、EEPROM+IAP、外 SRAM、SD卡、内存管理、文件系统
- **交互**：OLED显示屏、TFTLCD显示屏、触摸屏、摄像头、音频、图片、文字、Joyboard
- **RTOS**：UCOS
### 仿真

(待完善)

# stm32 泛知识

### stm32 小科普

stm32 芯片有四款主流系列
- 高性能：F2, F4, F7, H7
- 通用：G0, G4, F0, F1, F3
- 低功耗：L0, L1, L4, L5, L4+, L5+
- 无线：WL, WB

stm32 芯片物理上包括 “ARM Cortex CPU 内核" + "存储器(FLASH,RAM,ROM)" + ”外设"

### stm32 结构

[stm32 结构](https://zhuanlan.zhihu.com/p/362572057)
[stm32 时钟树详解](https://zhuanlan.zhihu.com/p/339529771)
stm32 上电时所有外设时钟被禁用

### 常见词汇解释

**MDK(Microcontroller Development Kit)**：ARM 公司为 arm 系列芯片提供的嵌入式开发工具，结构包括。
![](mdk5_structure.png)

**CH340**：一种 usb 转串口的驱动

**DTR(Data Terminal Ready)**：来自USB转串口工具（如CH340、CP2102、FT232等）的硬件流控制信号。

**ISP(In-System Programming)**：烧录程序

**GPIO(General Purpose Input/Output)**：通用输入输出，是STM32等嵌入式系统中，与外部设备进行数字信号交互的端口。

**AHB(Advanced High-performance Bus)**：AHB 用于连接高性能模块，如 CPU、DMA、SRAM、Flash 控制器等，提供高带宽、低延迟的数据传输。

**APB(Advanced Peripheral Bus)**：挂载在 AHB 下，用于连接低速外设（如 UART、I2C、GPIO 等）。

**仲裁**：通常指 多个主设备（如 CPU、DMA、USB、ETH 等）同时请求使用同一总线或资源时，根据优先级规则决定哪个主设备获得访问权的机制。

**RTS 和 CTS 的原始用途：硬件流控**

在 UART 通信中（例如两个 MCU 或 MCU 与电脑之间），如果一方的发送速度过快，另一方可能来不及处理，就会造成数据丢失。硬件流控就是用额外的引脚来“暂停/继续”传输，类似于交通信号灯。

|信号|全称|方向|含义（主动方）|
|---|---|---|---|
|**RTS**|Request to Send|发送设备 → 接收设备|“我要发送数据给你，你准备好了吗？”|
|**CTS**|Clear to Send|接收设备 → 发送设备|“我准备好了，你可以发。”|

通常工作流程（以两个设备 A、B 通信为例，A 发送给 B）：
1. A 想发数据时，先将自己的 **RTS 引脚** 输出有效电平（通常是低电平，取决于芯片）。
2. B 看到了 A 的 RTS 信号，如果 B 的接收缓冲区有空余，就把自己的 **CTS 引脚** 输出有效电平给 A，表示“可以发”。
3. A 检测到 B 的 CTS 有效，才开始发送数据。
4. 如果 B 的缓冲区快满了，B 会立即拉高自己的 CTS（无效电平），A 检测到后必须暂停发送，直到 CTS 再次有效。

### stm32 数据手册、参考手册使用指导

cortex-M3 权威手册：thumb2 指令集大全
- 算数指令、逻辑指令、存取指令、中断指令、权限指令
stm32f10x 技术手册：F1 系列芯片的编程辅助手册
- F1命名规则，命名上各组成部分的含义，比如stm32f103zet6
- F1系统结构
- F1时钟树
stm32f103x 数据手册：F103 系列芯片的数据手册，包含
- 外设数据、频率数据、存储容量配置、电气特性、封装特性
- 引脚定义、各封装的物理引脚位置
- 存储器映射：FLASH上各区域的用途

### CMSIS 文件夹

ST 公式提供的 STM32 标准库形如 `STM32F10x_StdPeriph_Lib_V3.5.0`，在其中的 `Libraries` 文件夹下存放了所有标准库文件，包括 `CMSIS`、`STM32F10x_StdPeriph_Driver`

`CMSIS` 是 stm32 开发中的核心层，所有上层调用都经过 `CMSIS` 层才可访问内核与外设。
![](stm32_image/CMSIS文件夹主要文件.png)

其中
- `stm32f10x.h`: 定义寄存器地址、寄存器数据结构。
- `system_stm32f0x.h,core_cm3.h`: 内核相关文件。
- `system_stm32f10x.c`: 内核相关文件，主要对片上 RCC 外设进行操作，以实现 STM32 的时钟配置。
系统上电/复位时，首先会执行作为arm汇编程序的启动文件，其中会调用复位函数 `Systeminit`，后者就在 `system_stm32f10x.c` 中定义。