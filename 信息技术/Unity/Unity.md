### Mono 与 IL2CPP

Unity 用 C# 作为主要的跨平台开发语言，C# 代码会被编译成 IL（Intermediate Language) 然后用运行环境的解释器执行。

早期 Unity 中承担解释器的角色是 Mono，Mono既编译 C# 为 IL 也在运行时解释并执行 IL。解释器 Mono 的跨平台方案天然有运行效率低、易被反编译等缺点，不利于大型商业游戏的开发。

新版本的 Unity 在 mono 的基础上增加了 IL2CPP 的方案，即将 CIL 编译为 C++ 代码，由运行环境上的 C++ 编译器生成可执行代码。

![[mono结构.webp]]

### Mono 的反汇编

用 Mono方式来进行打包的程序会出现一堆动态链接库，程序员写的程序控制逻辑就在 Assembly-CSharp.dll 动态链接库里面，而IL2CPP不打包为dll，它将Mono虚拟机和Assembly-CSharp.dll动态链接库一起整合在了 libil2cpp.so 文件里。根据这一特点可以快速分别从游戏发行方处下载的 Unity 游戏用了哪种打包。

如果是 Mono 则容易对游戏进行反汇编和做本地修改。 

### dll
在 **dnSpy** 反编译或分析 **DLL** 文件时，**PE** 指的是 **Portable Executable（可移植可执行文件）**，这是 Windows 操作系统上 **.exe**、**.dll** 等二进制文件的通用格式标准。

### 路线
1. 如何用 dnSpy 向 Unity 游戏插入一个自己写的 C# 代码编译的 dll
2. 如何用 dnSpy 分析某 Unity 游戏的执行逻辑，找到需要插入的位置
3. 如何编写实现想要功能的 C# 代码
