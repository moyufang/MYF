
![[stm32f1核心架构.png]]

# ADC

stm32f103 系列支持 1~3 个 ADC，有 12 位分辨率，0V~3.3V 的范围，16+2=18 个通道，4 个结果寄存器的注入组，1 个结果寄存器的规则组，高/低阈值看门狗。

ADC_CLK 不得超过14MHz，它是由PCLK2经分频产生。

一次 ADC 转换包括采样阶段和转换阶段，转换阶段时间固定为 12.5 个周期，在采样阶段时间被设置为 1.5 个周期且 ADC 频率为 14MHZ 时，ADC 有最短的转换时间 1us。

- ADC状态寄存器(ADC_SR)
	- AWD：看门狗事件发生
	- EOC：规则组结束，由软件清除
	- JEOC：注入组结束，由软件清除
	- JSTART：注入组开始
	- START：规则组开始
- ADC控制寄存器 1(ADC_CR1)
	- AWDCH：看门狗通道设置，0~17对应通道0~17
	- EOCIE：EOC中断使能
	- AWDIE：看门狗中断使能
	- JEOCIE：JEOC中断使能
	- SCAN：扫描模式使能，置 1 时每次结束转换后切换采用通道
	- AWDSGL：置 0 时看门狗检测扫描模式中的所有通道，置 1 时仅检测由 AWDCH 决定的通道
	- JAWDEN：在注入通道上开启看门狗
	- AWDEN：在规则通道上开启看门狗
- ADC控制寄存器 2(ADC_CR2)
	- ADON：开关 ADC，校准前应保持 ADON 为 0 至少 2 个 ADC 周期
	- CONT：开关连续转换模式
	- CAL：开关校准，由软件置 1 开启校准，硬件清 0 代表校准结束
	- RSTCAL：开关复位校准，读写模式同 CAL
	- DMA：开关 DMA 模式，注意只有 ADC1 和 ADC3 能产生 DMA 请求
	- ALIGN：置 0 右对齐，置 1 左对齐
	- JEXTSEL：选择启动注入通道转换的外部事件
	- JEXTTRIG：开关使用外部事件启动注入通道转换
	- EXTSEL：规则通道版 JEXTSEL
	- EXTTRIG：规则通道版 JEXTTRIG
	- JSWSTART：软件置 1 时启动注入通道转换，转换一旦开始则由硬件置 0
	- SWSTART：规则通道版 JSWSTART
	- TSVREFE：温度传感器和 $V_{REFINT}$ 使能，在多个 ADC 中该位仅出现在 ADC1
![[ADC_CR2_JEXTSEL.png]]
- ADC采样时间寄存器 1(ADC_SMPR1)
	-  SMPx：分别设置通道10~17采样时间
![[ADC_SMPR1.png]]
- ADC采样时间寄存器 2(ADC_SMPR2)
	-  SMPx：分别设置通道0~9的采样时间
- ADC注入通道数据偏移寄存器x (ADC_JOFRx)(x=1..4)
	- JOFFSETx：注入通道x的数据偏移，转换结果为原始数据减去此值，对齐模式应保持为右对齐
- ADC看门狗高阀值寄存器(ADC_HTR)
	- HT：12位高阈值，比较不受对齐模式影响，但受注入通道x数据偏移影响
- ADC看门狗低阀值寄存器(ADC_LRT
	- LT：12位低阈值，比较不受对齐模式影响，但受注入通道x数据偏移影响
- ADC规则序列寄存器 1(ADC_SQR1)
	- SQx(x=13,14,15,16)：规则序列中第 x 个转换对应的通道序号
	- L：4位，规则通道序列长度，0 对应 1 个转换，15 则由 16 个转换
- ADC规则序列寄存器 2(ADC_SQR2)
	- SQx(x=7,8,9,10,11,12)：规则序列中第 x 个转换对应的通道序号
- ADC规则序列寄存器 3(ADC_SQR3)
	- SQx(x=1,2,3,4,5,6)：规则序列中第 x 个转换对应的通道序号
- ADC注入序列寄存器(ADC_JSQR)
	- 
### 双 ADC 模式
- ADC状态寄存器(ADC_SR)
	- DUALMOD：指定双 ADC 的模式
### 间断模式

- ADC控制寄存器 1(ADC_CR1)
	- JAUTO：
	- DISCEN：规则组上间断模式使能
	- JDISCEN：注入组上间断模式使能
	- DIS