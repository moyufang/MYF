### n维欧式空间的定义与结构

为了与维数坐标区分开来，$\mathbb{R}^n$ 中的点列记作 $\{x^m\}$，其中 $x_i^m(1\le i\le n)$ 代表点 $x^m$ 的第 $i$ 维坐标。

**1.1** $\mathbb{R}^n$ 的距离空间 $\rho$ 满足所谓距离公理：设 $x,y\in\mathbb{R}^n$

$(\text{i})$ 正定性：$\rho(x,y)\ge 0$ 且 $\rho(x,y)=0$ 的充要条件是 $x=y$。

$(\text{ii})$ 对称性：$\rho(x,y)=\rho(y,x)$。

$(\text{iii})$ 三角不等式：$\rho(x,z)\le\rho(x,y)+\rho(y,z)$。

$\rho(x,y)$ 也用范数符号 $|x-y|$ 来表示。 

**1.2** 设 $x,y\in\mathbb{R}^n$，则 $p$ 距离（$p\ge 1$ 或 $p=\infty$）的定义是
$$
|x-y|_p=\left(\sum_{i=1}^n|x_i-y_i|^p\right)^{1\over p}
$$

$$
|x-y|_\infty=\max_{i=1}^n\{|x_i-y_i|\}
$$

$\forall p,q>1$ 可以证明
$$
{1\over\sqrt[p]{n}}|x-y|_p\le|x-y|_\infty\le|x-y|_p
$$

$$
{1\over\sqrt[p]{n}}|x-y|_p\le|x-y|_q\le\sqrt[q]{n}|x-y|_p
$$



$p$ 距离统称为闵氏距离。

**1.3** $\text{Holder}$ 不等式。

**1.4** $\text{Minkovski}$ 不等式。

由 $\text{Minkovski}$ 不等式可以证明 $p$ 距离的定义满足距离空间公里中的三角不等式，于是可知 $p$ 距离的确是一个距离空间。

**1.5** 定义了 $\mathbb{R}^n$ 上的一个距离空间 $\rho$ 后，可以定义数列极限的概念。

$\{x^m\}$ 收敛于 $x\in\mathbb{R}^n$ 的定义是，$\forall \delta>0,\ \exists N,\ \forall n>N$，有 $\rho(x_n,x)=|x-y|<\delta$。记作
$$
\lim_{m\to\infty}x^m=x
$$
由极限的定义可知，$\mathbb{R}^n$ 上所有 $p$ 距离在极限的意义下都是等价的的，以后无特殊声明则默认 $p=2$，并且将 $|x-y|_2$ 简写成 $|x-y|$，也就是欧式距离。

**1.6** 极限存在则唯一。

**1.7** $\{x^m\}$ 收敛于 $x\in\mathbb{R}^n$ 等价于 $\forall 1\le i\le n$ 均有 $\displaystyle \lim_{m\to\infty} x^m_i=x_i$。

**1.8** $\displaystyle \lim_{m\to\infty} x^m=x$ 等价于 $\{x^m\}$ 是柯西收敛列。



### $\mathbb{R}^n$ 上的区域

**1.1** 在 $\mathbb{R}^n$ 上的一些定义。

区域 $A\sube \mathbb{R}^n$ 的补集记作 $A^c=\mathbb{R}^n-A$。

设 $r>0,\ x_0\in\mathbb{R}^n$，定义开球体 $B(x_0,r)=\{x:|x-x_0|<r,x\in\mathbb{R}^n\}$。

定义 $U\sube\mathbb{R}^n$ 为 $x_0$ 的一个开邻域当且仅当 $\exists r>0$，使得 $B(x_0,r)\sube U$。

区域 $A$ 为开集的定义是： $\forall x_0\in A$，存在 $x_0$ 的一个开邻域 $U$ 满足 $U\sube A$。

区域 $B$ 为闭集的定义是： $B^c$ 是开集。

规定 $\mathbb{R}^n$ 与 $\emptyset$ 即是开集，也是闭集。 

区域 $A$ 是有界当且仅当 $\exists r>0$ 使得 $B(0,r)\supe A$。



设 $A\sube\mathbb{R}^n,\ x\in\mathbb{R}^n$。

若 $A$ 包含 $x$ 的一个开邻域，则称 $x$ 是 $A$ 的内点。

若 $x$ 是 $A^c$ 的内点，则称 $x$ 是 $A$ 的外点。

若 $x$ 的任意开邻域 $U$ 均满足 $U\cap A\neq \empty$，则称 $x$ 是 $A$ 的触点。

若 $x$ 的任意开邻域 $U$ 均满足 $U\cap A\neq\empty$ 和 $U\cap A^c\neq\empty$，则称 $x$ 是 $A$ 的边界点。

若 $x$ 的任意开邻域中均存在异于 $x$ 的点 $y$，满足 $y\in A$，则称 $x$ 是 $A$ 的导点。

$A$ 的内点集合记作 $A^0$，外点集合可以表示为 $(\overline{A})^c$ 或 $(A^c)^0$，触点集合记作 $\overline{A}$，导点集合记作 $A'$，边界点集合记作 $\part A$。$A^0$ 也称作 $A$ 的内部，$A$ 的外点集合也称作 $A$ 的外部，$\overline{A}$ 也称作 $A$ 的闭包，$A'$ 也称作 $A$ 的导集，$\part A$ 也称作 $A$ 的边界。



**1.2** 设区域 $A,B\sube\mathbb{R}^n$，点 $x\in\mathbb{R}^n$，则以下公式成立
$$
\begin{aligned}&A^0\sube A\sube \overline{A},\ A^0\sube A'\sube \overline{A}\\
&\overline{A}=A\cup\part A=A'\cup \part A=A^0\cup\part A\\
&A^0=A-\part A=A'-\part A=A^0-\part A\\
&A\sube B\Rightarrow A^0\sube B^0,\ \overline{A}\sube \overline{B}\\
&\overline{A \cup B}=\overline{A}\cap\overline{B},\ \overline{A\cap B}\sube \overline{A}\cup \overline{B}\\
&(A\cap B)^0=A^0\cup B^0,\ (A\cup B)^0\ A^0\supe B^0
\end{aligned}
$$
**1.3** 任意开集的并集是开集，有限开集的交集是开集。任意闭集的交集是闭集，有限闭集的并集是闭集。



### $\mathbb{R}^n$ 中有界闭集的列紧性和紧性

$A\subseteq\mathbb{R}^n$ 是列紧集的定义是：$A$ 上任意点列均有收敛于 $A$ 中的点的子列。

$A\subseteq\mathbb{R}^n$ 是紧集的定义是：$A$ 的任意开覆盖集 $\{O_i\}$ 中存在有限子覆盖。

**2.1** $A\subseteq\mathbb{R}^n$ 是闭集当且仅当 $A$ 是完备的，即 $A$ 中任意收敛点列 $\{x^m\}$ 均满足 $\displaystyle \lim_{m\to\infty} x^m\in A$。

**2.2** $A\subseteq\mathbb{R}^n$ 是闭集当且仅当 $A=\overline{A}$，$A$ 是开集当且仅当 $A=A^0$。

**2.3** $\text{Bolzano-Weierstrass}$ 定理。设 $\{x^m\}$ 是有界点列，则 $\{x^m\}$ 存在收敛子列。

**2.4** $A\in\mathbb{R}^n$，则 $A$ 是有界闭集 $\Leftrightarrow$ $A$ 是列紧集。

**2.5** $\text{Heine-Borel}$ 定理。$A\in\mathbb{R}^n$，则 $A$ 是有界闭集 $\Leftrightarrow$ $A$ 是紧集。

定理 **2.3** 与 **2.4** 与 **2.5** 在 $\mathbb{R}^n$ 的闵氏距离空间上成立，如果将闵氏距离空间换成一般距离空间，则有界闭集不一定是列紧集或紧集，但此时仍然可以证明紧集等价于列紧集，证明在习题中给出。

**2.6** 设 $A$ 是 $\mathbb{R}^n$ 中的有界闭集，$\{O_i:i\in I\}$ 是 $A$ 的开覆盖，则存在 $\delta>0$，使得 $\mathbb{R}^n$ 中的集合 $B$ 只要满足
$$
B\cap A\neq\emptyset,\ \mathrm{diam} B=\sup_{x,y\in B}\{|x-y|\}<\delta
$$
就存在 $i\in I$ 使得 $B\subseteq O_i$。

$\delta $ 称为开覆盖 $\{O_i:i\in I\}$ 的勒贝格($\text{Lebesgue}$)数。

$\mathrm{diam} B$ 称作 $B$ 的直径。



### 连通性

道路连通的定义。

连通的定义。

**3.1** $A\subseteq\mathbb{R}^n$，若 $A$ 是道路连通集，则 $A$ 是连通集。

注意连通集不一定是道路连通集。

**3.2** $A\subseteq\mathbb{R}^n$，若 $A$ 是连通集，则 A 的闭包 $\overline{A}$ 是连通集。

**3.3** $A\subseteq\mathbb{R}^n$，若 $A$ 是连通开集，则 $A$ 是道路连通集。



### 多元函数的极限

多元极限的定义。

**4.1** 设 $x^0$ 是函数 $f:x\in\mathbb{R}^n\to f(x)\in\mathbb{R}$ 定义域 $D$ 的聚点，则 $\displaystyle \lim_{x\to x^0} f(x)=l$ 的充要条件是：$D$ 中任意收敛于 $x^0$ 的点列 $\{x^m\}$ 均满足 $\displaystyle \lim_{m\to+\infty} f(x^m)=l$。

多元函数极限的性质与运算与一元函数类似，有加减乘除、保序性、夹逼定理、柯西收敛原理，这里略。



### 多元函数的连续性

多元函数连续性的定义。

多元函数一致连续性的定义。

多元函数 $\text{Lipschitz}$ 连续的定义。

多元函数的连续性在加减乘除（除数不为零）、复合运算下仍成立。

**5.1** 设 $f(x)$ 是定义在 $\mathbb{R}^n$ 的有界闭集 $D$ 的连续函数，则 $f(x)$ 在 $D$ 上有界。

**5.2** 设 $f(x)$ 是定义在 $\mathbb{R}^n$ 的有界闭集 $D$ 的连续函数，则 $f(x)$ 在 $D$ 上有最大值和最小值。

**5.3** 设 $f(x)$ 是定义在 $\mathbb{R}^n$ 的有界闭集 $D$ 的连续函数，则 $f(x)$ 在 $D$ 上一致连续。

**5.4** 介值定理：设 $f(x)$ 是定义在 $\mathbb{R}^n$ 的连通子集 $D$ 的连续函数，若 $x^1,x^2\in D$ 且 $f(x^1)<f(x^2)$，则 $\forall f(x^1)<c<f(x^2)$，$\exists x^0\in D$，使 $f(x^0) = c$ 。

**5.5** 若函数 $f(x)$ 是定义在 $\mathbb{R}^{n}$ 上的凸函数，则 $f(x)$ 连续。



### 习题

**1.** 在一般距离空间 $\{\mathbb{R}^n,\rho\}$上，设 $A$ 是 $\mathbb{R}^n$ 上的非空子集，证明 $A$ 的列紧性与紧性等价。

**证明**

紧性 $\Rightarrow$ 列紧性

设 $A$ 是紧集，可以证明 $A$ 列紧集，否则存 $A$ 中的点列 $\{x^i\}$，使得它没有收敛于 A 中点的子列。

$\forall x\in A$，存在 $x$ 的开领域 $U(x)$。

因为 $\{x^i\}$ 没有收敛于 A 中点的子列，故 $\exists m(x)\in\mathbb{N}$，使 $\forall l>m(x)$，均有 $x^l\not\in U(x)$。

由 $A$ 的紧性可知，$A$ 存在有限覆盖 $\{U(y^i):y^i\in A,i\in[1,l_1]\cap\mathbb{N}\}$。

取 $\displaystyle m=\max_{i=1}^{l_1}\{m(y^i)\}$，则当 $l>m$ 时，$\displaystyle x^m\not\in \bigcup_{i=1}^{l_1} U(y^i)$

而 $\{U(y^i)\}$ 是 $A$ 的开覆盖，故 $x^m\not\in A$，这与 $x^m\in A$ 矛盾。

故 $A$ 是列紧集。

列紧性 $\Rightarrow$ 紧性



