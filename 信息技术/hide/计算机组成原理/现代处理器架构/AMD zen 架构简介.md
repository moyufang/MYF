
以采用 Zen 4 架构的 Ryzen 9 7950x 为例，cpu 由以下部分组成
- 2 个 CCD (Core Complex Die)，每个 CDD 包括 8 个核心，共享 32MB 的 L3 Cache，每个核心各自独享 1MB 的 L2 Cache、32KB 的 L1d Cache 和 32KB 的 L1i Cache。 
- 一个 IOD (IO Die)。
- IF (Infinity Fabric) 总线。

7950x 中 2 个 CCD 通过 IF 进行相互之间的通信，CCD 可进一步划分为 2 个 CCX(Core Complex)，CCX 之间也通过 IF 进行通信。两个 CCD 间通过 IF 与 IOD 通信，IOD 与主板通信，完成 CPU 与诸如内存、PCIe、存储接口等的通信。


### Zen 架构内存模式

[【小白向科普】近几年锐龙内存模式简单讲解](https://zhuanlan.zhihu.com/p/483044293)

FCLK 是 IF 总线的频率，UCLK 是 IOD 的频率，MCLK 是内存的频率。

一般 UCLK 需要等于 MCLK。

AMD CPU 的设计中 FCLK 





