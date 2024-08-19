---
title: "计算 Bernoulli 数"
date: 2024-05-15
lastmod: 2024-06-28
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

FLINT（快速数论函数库）是如何用 zeta 函数计算 Bernoulli 数的？

<!--more-->


## Bernoulli 数

Bernoulli 数 $B_n$ 可以用一个函数的指数生成函数（EGF）展开来定义

$$
\frac{x}{e^x - 1} = \sum_{n=0}^{\infty} \frac{B_n}{n!} x^n
$$

Bernoulli 数和多项式求和有关

$$
\sum_{i=0}^{n} i^k = \frac{1}{n+1} \sum_{i=0}^{k} (-1)^i \binom{n+1}{i} B_{k-i} n^{k+1-i} \\
$$

前几个 Bernoulli 数为

$$
\begin{aligned}
B_0 &= 1 &
B_1 &= -\frac{1}{2} \\
B_2 &= \frac{1}{6} &
B_3 &= 0 \\
B_4 &= -\frac{1}{30} &
B_5 &= 0 \\
B_6 &= \frac{1}{42} &
B_7 &= 0 \\
B_8 &= -\frac{1}{30} &
B_9 &= 0 \\
B_{10} &= \frac{5}{66} &
B_{11} &= 0 \\
\end{aligned}
$$

可以用递推公式计算 Bernoulli 数，但是这样计算效率不高

$$
\begin{aligned}
B_0 &= 1 \\
B_1 &= -\frac{1}{2} \\
\sum_{k=0}^{n} \binom{n+1}{k} B_k &= 0, \quad n \ge 2
\end{aligned}
$$




根据 [von Staudt–Clausen 定理](https://en.wikipedia.org/wiki/Von_Staudt%E2%80%93Clausen_theorem)，Bernoulli 数的分母可以比较容易地计算出来：

$$
D_n = \prod_{\substack{
    p \text{ prime} \\
    p-1 \mid n
}} p \tag{1}
$$

例如，$D_{10} = 2 \times 3 \times 11 = 66$。


## $\zeta$ 函数

黎曼 $\zeta$ 函数在 $(1, \infty)$ 上的定义是

$$
\zeta(s) = \sum_{n=1}^{\infty} \frac{1}{n^s} \tag{2}
$$

欧拉乘积公式将 $\zeta$ 函数与素数联系起来：

$$
\zeta(s) = \prod_{p \text{ prime}} \frac{1}{1 - p^{-s}} \tag{3}
$$


在偶数时，$\zeta$ 函数有特殊的取值：

$$
\begin{aligned}
\zeta(2) &= 1 + \frac{1}{2^2} + \frac{1}{3^2} + \cdots = \frac{\pi^2}{6} \\
\zeta(4) &= 1 + \frac{1}{2^4} + \frac{1}{3^4} + \cdots = \frac{\pi^4}{90} \\
\zeta(6) &= 1 + \frac{1}{2^6} + \frac{1}{3^6} + \cdots = \frac{\pi^6}{945} \\
\end{aligned}
$$

Bernoulli 数和 $\zeta$ 函数有以下关系：

$$
\zeta(2n) = (-1)^{n+1} \frac{2 \pi^{2n}}{(2n)!} B_{2n} \tag{4}
$$

## 计算 Bernoulli 数

sagemath 罗列了[几种算法](https://doc.sagemath.org/html/en/reference/rings_standard/sage/arith/misc.html#sage.arith.misc.bernoulli)，默认使用的是 FLINT 库

> `algorithm`:
>
> - `'default'` – use 'flint' for n <= 20000, then 'arb' for n <= 300000 and 'bernmm' for larger values (this is just a heuristic, and not guaranteed to be optimal on all hardware)

[FLINT](https://github.com/flintlib/flint) 库是一个数论计算库。

它使用 `_bernoulli_fmpq_ui` 给出 bernoulli 数，这个函数输入整数 $n$，输出 Bernoulli 数的分子和分母。

对于较大的情况，这个函数调用了 `_bernoulli_fmpq_ui_zeta`。

```c
void _bernoulli_fmpq_ui(fmpz_t num, fmpz_t den, ulong n) {
    if (n < (ulong)bernoulli_cache_num) {
        fmpz_set(num, fmpq_numref(bernoulli_cache + n));
        fmpz_set(den, fmpq_denref(bernoulli_cache + n));
    } else if (n < 18000 || n % 2 == 1) {
        _bernoulli_fmpq_ui_zeta(num, den, n);
    } else {
        _bernoulli_fmpq_ui_multi_mod(num, den, n, -1.0);
    }
}
```



奇数的 Bernoulli 数可以直接计算。

当 $n$ 为偶数时

- 首先使用 $(1)$ **计算 Bernoulli 数的分母**
- 然后**计算 Bernoulli 数的位数**，估计后续计算所需的精度
- 用 zeta 函数和素数的关系 $(3)$ 计算 $\boldsymbol{\zeta(n)}$（不用定义 $(2)$ 是应该因为 $(3)$ 收敛更快）
- 用 $(4)$ 从 $\zeta(n)$ 估计 Bernoulli 数的数值值，计算出 **Bernoulli 数的分子**


> 下面的代码有删减

```c
void _bernoulli_fmpq_ui_zeta(fmpz_t num, fmpz_t den, ulong n) {
    if (n % 2) {
        fmpz_set_si(num, -(n == 1));
        return;
    }
    arith_bernoulli_number_denom(den, n);

    for (prec = arith_bernoulli_number_size(n) + fmpz_bits(den) + 2;; prec += 20) {
        arb_bernoulli_ui_zeta(t, n, prec);
        arb_mul_fmpz(t, t, den, prec);

        if (arb_get_unique_fmpz(num, t))
            break;

        flint_printf("warning: %wd insufficient precision for Bernoulli number %wu\n", prec, n);
    }
}
```

运行示例：

```
flint/build/examples ./bernoulli 40
denominator = 13530
bernoulli(40) precision 75 = (-5058482495013537224755 * 2^-18) +/- (540159508 * 2^-49)
B_40 = -261082718496449122051/13530
```

用 Mathematica 简单造了一下轮子：

```mathematica
n = 40;
denominator[n_] := Times @@ Select[Divisors[n] + 1, PrimeQ];
zetaEstimate = 1/Product[(1 - Prime[i]^-n), {i, 20}];

absBernoulli = ((2  \[Pi])^n/(n!  2))^-1  zetaEstimate;
numerator = Round[absBernoulli  denominator[n]]
```

$\zeta$ 函数法计算 Bernoulli 数的时间复杂度大约是 $O(n^2 \log n)$

[基于 multimodular 的算法](https://arxiv.org/abs/0807.1347) 可以把复杂度降到大约 $O(n^2)$。似乎还有更快的算法。

## 总结

$\zeta$ 函数法利用 $\zeta$ 函数和 Bernoulli 数的关系、Bernoulli 数的分母的特点，能快速计算 Bernoulli 数
