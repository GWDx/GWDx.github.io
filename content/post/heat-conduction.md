---
title: "恒定热源的一维热传导方程"
date: 2024-05-09
keywords: []
description: ""
tags: []
categories: []
author: ""
summary: ""

comment: true
toc: false
autoCollapseToc: false
contentCopyright: false
reward: false
mathjax: true
---


用分离变量法、Fourier 变换求解左侧是恒定温度的热源的一维热传导方程

<!--more-->

## Fourier 变换

后面要用到 Fourier 变换，这里给出符号说明

> 使用酉变换的形式，好看一些

Fourier 变换、逆变换

$$
\begin{aligned}
\mathcal{F}\{f(x)\} &= \frac{1}{\sqrt{2\pi}} \int_{-\infty}^{\infty} f(x) e^{i\omega x} \, dx \\
\mathcal{F}^{-1}\{g(\omega)\} &= \frac{1}{\sqrt{2\pi}} \int_{-\infty}^{\infty} g(\omega) e^{-i\omega x} \, d\omega
\end{aligned}
$$

Fourier 余弦变换、逆变换

$$
\begin{aligned}
\mathcal{F}_c\{f(x)\} &= \sqrt{\frac{2}{\pi}} \int_{0}^{\infty} f(x) \cos(\omega x) \, dx \\
\mathcal{F}_c^{-1}\{g(\omega)\} &= \sqrt{\frac{2}{\pi}} \int_{0}^{\infty} g(\omega) \cos(\omega x) \, d\omega
\end{aligned}
$$

Fourier 正弦变换、逆变换

$$
\begin{aligned}
\mathcal{F}_s\{f(x)\} &= \sqrt{\frac{2}{\pi}} \int_{0}^{\infty} f(x) \sin(\omega x) \, dx \\
\mathcal{F}_s^{-1}\{g(\omega)\} &= \sqrt{\frac{2}{\pi}} \int_{0}^{\infty} g(\omega) \sin(\omega x) \, d\omega
\end{aligned}
$$

Laplace 变换、逆变换

$$
\begin{aligned}
\mathcal{L}\{f(t)\} &= \int_{0}^{\infty} f(t) e^{-st} \, dt \\
\mathcal{L}^{-1}\{g(s)\} &= \frac{1}{2\pi i} \int_{\sigma-i\infty}^{\sigma+i\infty} g(s) e^{st} \, ds
\end{aligned}
$$

## 热传导方程


最简单的一维热传导方程是这样的形式：

$$
\begin{aligned}
u_t &= k\ u_{xx} \\
u(0, x) &= \delta(x)
\end{aligned} \tag{1}
$$

这是一个线性偏微分方程

先考虑 $k = 1$ 的情况

使用**分离变量法**，设 $u(t, x) = T(t)X(x)$，代入方程得到

$$
\begin{aligned}
T'X &= TX'' \\
\frac{T'}{T} &= \frac{X''}{X} = -\lambda
\end{aligned}
$$

若 $\lambda = 0$，则

$$
\begin{aligned}
T(t) &= T_0 \\
X(x) &= Ax + B
\end{aligned}
$$

若 $\lambda < 0$，则

$$
\begin{aligned}
T(t) &= e^{\sqrt{-\lambda} t} \\
X(x) &= A\cosh(\sqrt{-\lambda} x) + B\sinh(\sqrt{-\lambda} x)
\end{aligned}
$$

> 这两个解虽然满足方程，但是太奇怪了，$x$ 无穷时是无界的
>
> 合理怀疑后一个和 Laplace 变换有关

若 $\lambda > 0$，则 $T' = -\lambda T$，$X'' = -\lambda X$，解得

$$
\begin{aligned}
T(t) &= e^{-\lambda t} \\
X(x) &= A\cos(\sqrt{\lambda} x) + B\sin(\sqrt{\lambda} x)
\end{aligned}
$$

> 这个解看起来比较顺眼

相当于把初始条件分解成一系列的正弦波，也就是 **Fourier 变换**

$$
\mathcal{F}_c\{\delta(x)\} = \frac{1}{\sqrt{2\pi}}
$$

这表示所有余弦波叠加起来，就是初始条件 $\delta(x)$

$$
\begin{aligned}
u(0,x)=\delta(x) &= \mathcal{F}_c^{-1}\left\{\frac{1}{\sqrt{2\pi}}\right\} \\
&= \frac{1}{2\pi} \int_{-\infty}^{\infty} \cos(\omega x) \, d\omega \\
\end{aligned}
$$

然后每个波的振幅随时间有不同程度的衰减（角频率为 $\omega$ 的波，衰减为 $e^{-\omega^2 t}$）。再叠加起来（逆变换），就是方程的解

$$
\begin{aligned}
u(t, x) &= \mathcal{F}_c^{-1}\left\{\frac{e^{-\omega^2 t}}{\sqrt{2\pi}}\right\} \\
&= \frac{1}{2\pi} \int_{-\infty}^{\infty} e^{-\omega^2 t} \cos(\omega x) \, d\omega \\
&= \frac{ e^{-\frac{x^2}{4t}} }{2\sqrt{\pi t}}
\end{aligned}
$$

<center><img src="solution1.gif" alt="solution1" width="400" /></center>


> 如果考虑 k 的话，就是把 $t$ 换成 $kt$
>
> $$
u(t, x) = \frac{ e^{-\frac{x^2}{4kt}} }{2\sqrt{\pi kt}} \tag{2}
$$

## 恒定热源的热传导方程

接下来考虑左侧是恒定热源的一维热传导方程。

左侧温度恒定为 0，右侧初始温度为 -1

$$
\begin{aligned}
u_t &= \begin{cases}
u_{xx} & x > 0 \\ 0 & x \le 0
\end{cases} \\
u(0, x) &= -\mathrm{unitstep}(x)= \begin{cases}
-1 & x > 0 \\ 0 & x \le 0
\end{cases}
\end{aligned} \tag{3}
$$

参考线性齐次的情况，设 $u(t, x) = T(t)X(x)$

$$
\begin{aligned}
T'X &= TX'' \\
\frac{T'}{T} &= \frac{X''}{X} = -\lambda
\end{aligned}
$$

当 $x>0$ 时

$$
\begin{aligned}
T(t) &= e^{-\lambda t} \\
X(x) &= A\cos(\sqrt{\lambda} x) + B\sin(\sqrt{\lambda} x)
\end{aligned}
$$

边界条件要求 $u(t, 0) = 0$，所以 $A = 0$

也就是边界条件要求只能由正弦波组成

用 Fourier 正弦变换

$$
\mathcal{F}_s\{\mathrm{unitstep}(x)\} = \sqrt{\frac{2}{\pi}} \frac{1}{\omega}
$$

这表示

$$
\begin{aligned}
u(0, x)=
-\mathrm{unitstep}(x) &= -\mathcal{F}_s^{-1}\left\{\sqrt{\frac{2}{\pi}} \frac{1}{\omega}\right\} \\
&=-\frac{2}{\pi} \int_{0}^{\infty} \frac{\sin(\omega x)}{\omega} \, d\omega
\end{aligned}
$$


所以，当 $x>0$ 时，

$$
\begin{aligned}
u(t, x) &= -\mathcal{F}_s^{-1}\left\{ \sqrt{\frac{2}{\pi}} \frac{e^{-\omega^2 t}}{\omega}\right\} \\
&= -\frac{2}{\pi} \int_{0}^{\infty} \frac{e^{-\omega^2 t}}{\omega} \sin(\omega x) \, d\omega \\
&= -\mathrm{erf}\left(\frac{x}{2\sqrt{t}}\right)
\end{aligned}
$$

> 计算这个 Fourier 逆变换可以使用积分性质（和 Laplace 变换类似）
> $$
\mathcal{F}^{-1}\left\{\frac{g(\omega)}{\omega}\right\} = -i \int \mathcal{F}^{-1}\{g(\omega)\} \, dx
$$

所以，原方程的解为

$$
u(t, x) = \begin{cases}
-\mathrm{erf}\left(\frac{x}{2\sqrt{t}}\right) & x > 0 \\
0 & x \le 0
\end{cases} \tag{4}
$$

> 其中 $\displaystyle\mathrm{erf}(x) = \frac{2}{\sqrt{\pi}} \int_{0}^{x} e^{-t^2} \, dt$ 是误差函数


<center><img src="solution2.gif" alt="solution2" width="400" /></center>

## 总结

使用分离变量法、Fourier 变换，计算一维均匀热传导方程 $(1)$ 的解 $(2)$，以及恒定热源的热传导方程 $(3)$ 的解 $(4)$

> 当年 Fourier 发明 Fourier 变换，就是为了求解热传导方程的。

## 后记

观察 $(4)$ 的形式，或许可以得到一些有趣的结论，比如

- 穿厚度 2 倍的衣服，保暖效果是 4 倍（只考虑热传导）
- 戴 2 层的一次性聚乙烯手套，需要 4 倍的时间才能达到相同的扩散浓度
