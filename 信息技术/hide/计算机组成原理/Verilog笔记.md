### Tips

仿真波形极度不符合预期时，检查关键模块的 port 的变量名有没有出错，verilog 默认给为声明信号自动声明，不会有编译错误。

写 cpu 的控制单元时，有两大类信号需要控制，一类是选择信号，一类是读写使能。

对读写使能，假设实际操作在阶段 $i$ 完成，则应在 $i-1$ 阶段拉高。

##### always

- ```always``` 语句实现组合逻辑应采用阻塞赋值方式。
- `always` 块内被赋值的信号应定义成 reg 类型，即使此信号在实际电路中是线型。
- `always@(*)` 块表示当块内输入信号变化时就触发



`case` 语句最好加上 `default:`，把默认情况考虑好。



##### 七段数码管

![七段数码管管线](七段数码管（二进制—字符）.png)

```verilog
module sevenseg(
	input		[3:0]data,
    output	reg	[6:0]segments
);
    always@(*)begin
        case(data)
            0:		segment = 	7'b111_1110; //ABCDEFG
            1:		segment = 	7'b011_0000;
			2:		segment = 	7'b110_1101;
            3:		segment = 	7'b111_1001;
            4:		segment = 	7'b011_0011;
            5:		segment = 	7'b101_1011;
            6:		segment = 	7'b101_1111;
            7:		segment = 	7'b111_0000;
            8:		segment = 	7'b111_1111;
            9:		segment = 	7'b111_0011;
            default:segment =	7'b000_0000;
        endcase
    end
endmodule
```



类似于 C++ 中的模版函数，可以对 `module` 指定模版参数及其默认值。

```verilog
module mux2 
#(parameter width = 8)
(
    input	[width-1:0]	d0,d1,
    input				s,
    output	[width-1:0] y
);
    assign y = s ? d0 : d1;
endmodule
```

使用默认的模版参数的语句是 `mux2 mux_default(d0, d1, s, out)`，自定义模版参数的是 `mux2 #(12) mux_defined(d0, d1, s, out)`。



### inout 的原理和使用方法

[inout 的原理和使用方法](https://blog.csdn.net/qq_46264636/article/details/134348540)

inout 端口不能独立存在，即公共管线的两端都是 inout 端口。

在使用 inout 端口中，为了避免 `data_out` 影响 `data_in`，必须要用 `reg` 来缓存 `data_in` 以及使用 `always` 语句来读入 `data_in`。

以 DRAM 与 CPU 的交互为例，注意到 RAM 本身就是寄存器，所以在读入时不需要画蛇添足地使用 `reg` 缓存器，反之，CPU 则需要。

```verilog
module DRAM(
    input 		read_ram, write_ram,
    input[31:0]	address,
    inout[31:0]	data_bus
);
    reg [31:0] ram[32767:0];
    
    assign data_bus = read_ram ? ram[address] : 32'bz;
    
    always @(write_ram or data_bus)begin
        if (write_ram) ram[address] = data_bus;
    end
endmodule

module CPU(
	input 			read_ram, write_ram,
    inout[31:0] 	data_buss
);
    reg[31:0]	data_in;
    reg[31:0]	data_out;
    
    assign data_bus = write_ram ? data_out : 32'bz;
    
    always@(read_ram)begin
        if (read_ram) data_in = data_bus;
    end
endmodule
```

### 文件输入、输出、打印、高级数学函数

[FPGA基础设计（11）Verilog任务、函数、系统任务、系统函数](https://blog.csdn.net/FPGADesigner/article/details/102871583)

#### 测试文件的编写

`#<number>` 表示等待 `<number>` 纳秒。

`initial` 语句中被赋值的变量必须是 `reg`。

以格式化输出调试信息

```verilog
$display("Error: a:%d b:%d c:%d expected_y:%d y:%d\n", {a, b, c, exp_y, y});
```

在测试文件中读入文件

```verilog
reg[3:0] testvectors[999:0];
$readmemb("example.tv", testvectors);

//example.tv
000_1
001_0
010_0
011_0
100_1
101_1
110_0
111_0
//

```
#### task

在Verilog中，`task`是一种用户定义的程序块，用于执行特定的功能。`task`可以包含输入（`input`）和输出（`output`）端口，允许数据在`task`调用时传递。以下是`task`的一些关键特性和用法：

`task`的定义使用`task`关键字开始，后跟`task`的名称，然后是端口声明（如果有的话），接着是任务内的语句，最后以`endtask`结束。例如：

```verilog
task task_name;
  input a, b; // 输入端口声明
  output c;   // 输出端口声明
  c = a + b;   // 任务内的语句
endtask
```
`task`可以通过一条语句来调用，并且需要按照定义时的顺序传递输入和输出参数。例如：
```verilog
reg result;
task_name(input1, input2, result); // 调用task
```
#### for 循环

```verilog
module my_design;
	initial begin
		// Note that ++ operator does not exist in Verilog !
		for (integer i = 0; i < 10; i = i + 1) begin
			$display ("Current loop#%0d ", i);
		end
	end
endmodule
```
### Verilog 编译预处理

[Verilog编译预处理](https://blog.csdn.net/first_bug/article/details/133888841)


