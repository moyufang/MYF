#### LC振荡电路

只考虑理想电容 $C$ 与理想电感 $L$ 串联形成的闭合回路。

记电容、电感处的电压和电流分别为 $U_C,U_L,I_C,I_L$，选取合适的电压电流参考方向，使得任意时刻下有 $U_C=U_L,\ I_C=I_L$，于是闭合回路中的电压、电流可记为 $U,\ I$。

这里我选择使电容处的电压参考方向与电流处的参考方向相反，电感处的电流电压参考方向与电流处的参考方向相同。

$U$ 和 $I$ 都是关于时间 $t$ 的函数，也分别记作 $U(t),I(t)$。

给定初始条件：在 $t=0s$ 时，电压 $U(0)=U_m$，电流 $I(0)=0$。

根据电容和电感的电路学性质可以写出如下微分方程组。
$$
\left\{\begin{aligned}
\frac{dU}{dt}&=-\frac{1}{C}\cdot I\\
\frac{dI}{dt}&=\frac{1}{L}\cdot U
\end{aligned}\right.
$$
为了方便简便书写，记 $\displaystyle k=\frac{1}{LC}$，此方程组可以归约为
$$
\frac{d^2I}{dt^2}=-kI
$$
这是二阶齐次常系数常微分方程，有通解
$$
I=C_1\sin(\sqrt{k}t+C_2)
$$
其中 $C_1,C_2$ 是待定常系数，由初始条件决定。

再根据原方程组可得
$$
U=C_1\sqrt{k}\cos(\sqrt{k}t+C_2)
$$
将初始条件带入 $I$ 与 $U$ 的表达式中可以得到
$$
\left\{\begin{aligned}
C_1\sin(C_2)&=0\\
C_1\sqrt{k}\cos(C_2)&=U_m
\end{aligned}\right.
$$
解得一组解
$$
\left\{\begin{aligned}
C_1&=\frac{U_m}{L\sqrt{k}}=\sqrt{\frac{C}{L}}\cdot U_m\\
C_2&=0
\end{aligned}\right.
$$
记 $\displaystyle I_m=\sqrt{\frac{C}{L}}\cdot U_m$，于是
$$
\begin{aligned}
I&=I_m\sin(\frac{t}{\sqrt{LC}})\\
U&=U_m\cos(\frac{t}{\sqrt{LC}})
\end{aligned}
$$
这个振荡电路的周期 $T=2\pi\sqrt{LC}$。

















