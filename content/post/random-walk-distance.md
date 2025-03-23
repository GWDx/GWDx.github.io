---
title: "随机游走到原点的距离的渐近估计"
date: 2024-06-30
lastmod: 2024-06-30
keywords: []
description: ""
tags: []
categories: []
author: ""
summary: ""

comment: true
toc: true
autoCollapseToc: false
contentCopyright: false
reward: false
mathjax: true
---

用中心极限定理估计 k 维空间中随机游走 n 步后到原点的距离服从的分布。

这个结论可以用来判断 k 维随机游走的常返性。

<!--more-->


## 高维中心极限定理

先介绍 [高维中心极限定理](https://en.wikipedia.org/wiki/Central_limit_theorem#Multidimensional_CLT)

$\{\vec{A}_1, \cdots, \vec{A}_n\}$ 是独立同分布的 k 维随机变量，$\vec{\mu}$ 是均值，$\Sigma$ 是 $\vec{A}$ 服从的分布的协方差矩阵。则

$$
\sqrt{n} \left(\frac{1}{n} \sum_{i=1}^{n} \vec{A}_i - \vec{\mu} \right) \xrightarrow{d} N(0, \Sigma)
$$

## 位置 $\vec{P}_n$ 的近似分布

对于 k 维空间的随机游走，每次选择 2k 个方向中的一个移动一步。

假设其中两个方向是 $X$ 和 $Y$，即 $\vec{A} = (X, Y, \cdots)$

则各有有 $\frac{1}{2k}$ 的概率向 +x 方向、-x 方向、+y 方向、-y 方向移动。

计算方差：

$$
\begin{aligned}
E(X) &= 0 \\
E(X^2) &=  \frac{1}{k} \\
Var(X) &= E(X^2) - E^2(X) = \frac{1}{k} \\
\end{aligned}
$$

协方差显然是 0

$$
\begin{aligned}
E(XY) &= 0 \\
Cov(X, Y) &= E(XY) - E(X)E(Y) = 0
\end{aligned}
$$

协方差矩阵

$$
\Sigma = \frac{I}{k}
$$

设 n 步后位置为 $\vec{P}_n=\displaystyle\sum_{i=1}^{n} \vec{A}_i$

位置近似服从正态分布

$$
\begin{aligned}
\frac{\vec{P}_n}{\sqrt{n}} &\xrightarrow{d} N(0, \Sigma) \\
\sqrt\frac{k}{n} \vec{P}_n &\sim N\left(0, I \right)
\end{aligned}
$$


## 距离 $R_{k,n}$ 的近似分布

设 $P_n$ 到原点的距离为 $R_{k,n} = \sqrt{\vec{P}_n \cdot \vec{P}_n}$，简记为 $R_k$

当 n 较大时，距离（乘上一个常数后）近似服从自由度为 k 的[卡分布](https://en.wikipedia.org/wiki/Chi_distribution)


$$
\sqrt\frac k n R_k \sim \chi_k \\
$$

<center>
<img src="chi-distribution-pdf.png" alt="chi-distribution-pdf" width="400"/>

卡分布的概率密度函数
</center>

## 期望距离

$$
\begin{align*}
E(\chi_k) &= \sqrt 2 \frac{\Gamma\left(\frac{k+1}{2}\right)}{\Gamma\left(\frac k 2\right)} \\
E(R_k) &\approx \sqrt\frac n k E(\chi_k) =\sqrt \frac {2} k \frac{\Gamma\left(\frac{k+1}{2}\right)}{\Gamma\left(\frac k 2\right)} \sqrt n
\end{align*}
$$

对于前几个维度，

$$
\begin{align*}
E(R_1) &\approx \sqrt\frac 2 \pi \sqrt n \approx 0.798 \sqrt n \\
E(R_2) &\approx \frac {\sqrt \pi} 2 \sqrt n \approx 0.886 \sqrt n \\
E(R_3) &\approx 2 \sqrt\frac 2 {3\pi} \sqrt n \approx 0.921 \sqrt n
\end{align*}
$$

当 $k \to \infty$ 时，前面的系数趋近于 1


比起平均距离，均方根好看很多

$$
\begin{align*}
E(\chi_k^2) &= k \\
\sqrt{E(R_k^2)} &\approx \sqrt\frac{n}k \sqrt {E(\chi_k^2)} = \sqrt n
\end{align*}
$$

> 事实上这个式子两边是相等的

## 常返性

判断到原点的常返性，可以看到达原点的次数的期望是否有限。

自由度为 k 的卡分布的概率密度函数：

$$
\begin{align*}
f(x) &= \frac{1}{2^{\frac k 2-1}\Gamma\left(\frac k 2\right)} x^{k-1} e^{-\frac{x^2}{2}} \\
&= C_1 x^{k-1} e^{-\frac{x^2}{2}} \\
&= C_1 x^{k-1} + O(x^{k+1})
\end{align*}
$$


然后积分估计第 n 步在原点的概率：

$$
\begin{align*}
P\left(\vec A_n=\vec 0\right) \approx P(R_{k,n} < 1) &= \int_0^{\sqrt\frac k n} f(r) dr \\
&= C_2 r^k + O(r^{k+2}) \Big|_0^{\sqrt\frac k n} \\
&= C_2 \left(\frac k n\right)^{\frac k 2} + O(n^{-\frac k 2-1}) \\
&= C_3 n^{-\frac k 2} + O(n^{-\frac k 2-1})
\end{align*}
$$

经过原点次数的期望

$$
\begin{align*}
E(N) &= \sum_{n=1}^\infty P\left(\vec A_n=\vec 0\right) \\
&\approx C_3 \sum_{n=1}^\infty n^{-\frac k 2} + O
\end{align*}
$$

- $k\leq 2$ 时，期望为无穷大，常返
- $k>2$ 时，期望为有限值，不常返

这个过程有点不严谨。不过可以简单理解：1 维和 2 维的随机游走是常返的，3 维以上的随机游走不常返

> 就是那个喝醉酒的人可以回到家，喝醉酒的鸟可能回不了家的故事

## 总结

使用中心极限定理，可以将 k 维空间中随机游走 n 步后的位置近似为正态分布。到原点的距离近似服从自由度为 k 的卡分布。

可以利用这一点估计到原点的期望距离，判断 k 维随机游走的常返性
