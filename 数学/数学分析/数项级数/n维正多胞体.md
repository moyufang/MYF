设 $n$ 维正多胞体的边长为 $a$，求该正多胞体中心到顶点的距离与 $a$ 的比例系数 $t_n$。

假设我们已经求得了 $t_{n-1}$，现在来求 $t_n$。

设 $O_{n-1}$ 是该正多胞体某一个超平面的中心，与这个超平面相对的顶点记作 $A_n$，由勾股定理得
$$
(A_nO_{n-1})^2+(t_{n-1}a)^2=a^2
$$
记 $h_n=A_nO_{n-1}$，于是 $h_n=a\sqrt{1-t_{n-1}^2}$。

$n$ 维正多胞体有 $n+1$ 个相同的超平面，这些超平面分别与中心 $O_n$ 的距离 $h_n'$ 均相同，于是 $h_n'={1\over n+1} h_n$。

所以 $t_na=h_n-h_n'={n\over n+1}h_n$，故得到递推式
$$
t_n={n\over n+1}\sqrt{1-t_{n-1}^2}
$$
现在证明 $\{t_n\}$ 单调递增且极限为 ${\sqrt{2}\over 2}$。



首先，$\forall n\in\mathbb{N}^+$
$$
\begin{aligned}
&t_n^2>t_{n-1}^2\\
\Leftrightarrow\ &n^2(1-t_{n-1}^2)>(n+1)^2t_{n-1}^2\\
\Leftrightarrow\ &t_{n-1}^2<{n^2\over n^2+(n+1)^2}\\
\Leftrightarrow\ &t_n^2>{n^2\over n^2+(n+1)^2}
\end{aligned}
$$
因为 $t_0=0,\ t_1={1\over 2},\ t_2={\sqrt{3}\over 3}$，显然有 $t_2>t_1>t_0$。

现在归纳假设 $t_n>t_{n-1}>t_{n-2}$ 成立，以下证明 $t_{n+1}>t_n$。

因为 $t_{n-1}>t_{n-2}$，所以
$$
\begin{aligned}
&t_{n-1}^2>t_{n-2}^2\\
\Rightarrow\ &t_{n-1}^2>{(n-1)^2\over (n-1)^2+n^2}\\
\Rightarrow\ &t_n^2<{n^2\over(n+1)^2}{n^2\over (n-1)^2+n^2}
\end{aligned}
$$
注意到函数 $f(x)={x^2\over(x-1)^2+x^2}$ 与函数 $g(x)={x^2\over (x+1)^2}$ 均在 $[1,+\infty)$ 上单调递增，所以
$$
\begin{aligned}
&t_n^2<{n^2\over(n+1)^2}{n^2\over (n-1)^2+n^2}\\
\Rightarrow\ &t_n^2<{n^2\over(n+1)^2}{(n+2)^2\over (n+1)^2+(n+2)^2}\\
\Rightarrow\ &t_n^2<{(n+1)^2\over(n+2)^2}{(n+2)^2\over (n+1)^2+(n+2)^2}\\
\Rightarrow\ &t_n^2<{(n+1)^2\over (n+1)^2+(n+2)^2}\\
\Leftrightarrow\ &t_{n+1}^2>t_n^2
\end{aligned}
$$
所以 $t_{n+1}>t_n$。

显然 $\{t_n\}$ 有上界 $1$，根据单调有界定理可知 $\displaystyle \lim_{n\to\infty} t_n$ 存在，不妨设为 $T$。

对递推式 $t_n^2={n^2\over(n+1)^2}(1-t_{n-1}^2)$ 左右两边同时对 $n$ 取极限得
$$
T^2=1-T^2
$$
舍掉负根得到 $T={\sqrt{2}\over2}$。













