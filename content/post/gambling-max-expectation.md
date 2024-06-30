---
title: "赌徒输光前最大值的期望"
date: 2024-04-11
lastmod: 2024-06-29
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

计算赌徒在输光前最大值的期望，分析各个因素对期望的影响。

<!--more-->


## 前言

我很喜欢博尔赫斯的《巴比伦彩票》，这个故事描绘了一个随机性主导的世界，人们的命运由神秘的彩票决定。

> 正如所有的巴比伦人一样，我当过总督；正如所有的人一样，我当过奴隶；我有过至高无上的权力，也受过屈辱，蹲过监狱。瞧：我右手的食指已被剁掉。瞧：从我袍子的裂口可以看到一个橙黄色的刺花：那是第二个符号贝思。在月圆的夜晚，这个字母赋予我支配那些刺有吉梅尔记号的人，但是我得听从有阿莱夫记号的人，而他们在没有月亮的夜晚则听从有吉梅尔记号的人支配。拂晓的时候，我在地窖的一块黑色岩石前面扼杀圣牛。有一个太阴年，我被宣布为无形：我大声呼喊，却无人理睬，我偷面包，却不被抓住砍头。我经历过希腊人所不了解的事情：忧惧。那是一间青铜的秘屋，面对默不作声的披着头巾的绞刑刽子手，希望始终陪伴着我；不过在欢乐的长河中也有惊慌。赫拉克利德斯·本都库斯赞叹不已地说毕达哥拉斯记得他前生是派罗，是欧福尔波，再前生是另一个人；我回忆相似的沧桑变幻时却不需要投生轮回，甚至不需要假冒欺骗。

博尔赫斯笔下的人物在随机的环境中喜欢谈论最值。

我想算一下赌徒输光前最大值的期望。我不太会组合、鞅，所以只能胡乱分析一通。

> 需要用到：概率论、Laurent 级数

## 等概率等步长的情况

### 问题描述

一个赌徒有 a 元钱，每次投入 1 元，直到输光为止，输赢的概率都是 $\frac{1}{2}$。求输光前最大值的期望

### 两个吸收壁的随机游走

先考虑这个两个吸收壁的随机游走问题：

有两个吸收壁，分别是 -a 和 b，初始位置是 0。向左和向右的概率都是 $\frac{1}{2}$，到达吸收壁停止。问停在 b 的概率是多少。

设 $p_n$ 是从 n 出发停在 b 的概率，那么有如下递推关系

$$
\begin{aligned}
p_b &= 1 \\\
p_{-a} &= 0 \\\
p_n &= \frac{p_{n-1} + p_{n+1}}{2}
\end{aligned}
$$

> 这里应该是用了 Markov 链的某个性质

解这个方程，得到

$$
p_n = \frac{a+n}{b+a}
$$

所以 $p_0 = \frac{a}{b+a}$

### 解答

设赌徒最大值的随机变量是 M

上面的问题的结果相当于 $P(M \geq b) = \frac{a}{b+a}$

$$
\begin{aligned}
P(M = b) &= P(M \geq b) - P(M \geq b+1) \\\
 &= \frac{b}{b+a} - \frac{b+1}{b+a+1} \\\
 & = \frac{a}{(b+a)(b+a+1)}
\end{aligned}
$$

所以期望

$$
\begin{aligned}
E(M) &= \sum_{b=0}^{\infty} b \cdot P(M = b) \\\
&= \sum_{b=0}^{\infty} \frac{a b}{(b+a)(b+a+1)}
\end{aligned}
$$

简单估一下阶，就是 $\displaystyle\sum_{b=1}^{\infty} \frac{1}{b}$，所以是期望是无穷大

> 我算到这里的时候也很惊讶

计算过程对应这段代码

```mathematica
eqns = {2 p[x] == p[x - 1] + p[x + 1], p[-a] == 0, p[b] == 1};
result[x_, a_, b_] = p[x] /. First@RSolve[eqns, p[x], x]
P[a_, b_] = result[0, a, b] - result[0, a, b + 1] // Simplify
Sum[b P[a, b], {b, 0, \[Infinity]}]
```

也可以用生存函数（survival function）来计算期望

$$
\begin{aligned}
\mathrm{SF}(b) &= P(M > b) \\\
&= \frac{a}{b+a+1} \\\
E(M) &= \sum_{b=0}^{\infty} \mathrm{SF}(b)= \infty
\end{aligned}
$$





## 不均等概率的情况

### 问题描述

一个赌徒有 a 元钱，每次有 t 的概率输一元，剩余的概率赢一元。求输光前最大值的期望



### 解答

设 $p_n$ 是从 n 出发停在 b 的概率，那么有如下递推关系

$$
\begin{aligned}
p_b &= 1 \\\
p_{-a} &= 0 \\\
p_n &= t p_{n-1} + (1-t) p_{n+1}
\end{aligned}
$$

使用 Z 变换，可以解出

$$
p_n = \frac{c^{a+n}-1}{c^{a+b}-1}
$$

其中

$$
c = \frac{t}{1-t}
$$


当 $t>\frac{1}{2}$ 时（每局的期望小于 0），$c>1$


$$
\begin{aligned}
\mathrm{SF}(b) &= P(M > b) = \frac{c^a - 1}{c^{a+b+1} - 1} \\\
E(M) &= \sum_{b=0}^{\infty} \mathrm{SF}(b) \\\
&= \sum_{b=1}^{\infty} \frac{c^{a+n}-1}{c^{a+b}-1}\\\
&= \frac{c^a - 1}{2 \ln c} \left( - 2 \ln (c-1) +(2a+1) \ln c - 2 \psi_c(1+a) \right)
\end{aligned} \tag{1}
$$

> 其中 $\psi_q(z)$ 是 [q-Polygamma](https://mathworld.wolfram.com/q-PolygammaFunction.html) 函数

1. 当 $c\to 1$ 时。 $E(M)$ 和 $\ln t$ 大致成线性关系

$$
E(M) = -a (\ln(c-1) + \psi(1+a)) + O(c-1)
$$

2. 当 $c\to \infty$ 时，渐近显然是 0

$$
E(M) = \frac{1}{c} + O\left(\frac{1}{c^2}\right)
$$


3. 当 $a=0$ 时，$E(M) = 0$

4. 当 $a\to \infty$ 时，渐近是

$$
\lim_{a\to \infty} E(M) = \sum_{b=0}^{\infty} \lim_{a\to \infty} \mathrm{SF}(b) =  \sum_{b=1}^{\infty} \frac{1}{c^b} = \frac{1}{c-1}
$$

$E(M)$ 在 $c\in(1, \infty)$ 上递减，在 $a\in[0, \infty)$ 上递增

## 不均等步长的情况

### 问题描述

一个赌徒有 a 元钱，每次有 t 的概率输一元，剩余的概率赢 m 元。求输光前最大值的期望

$$
\begin{aligned}
p_n &= t p_{n-1} + (1-t) p_{n-m} \\\
p_{-a} &= 0 \\\
p_x &= 1 \quad x \geq b \\\
\end{aligned}
$$

### 解答

如果还是用 Z 变换的话，要解一个 m 次方程，而且还要代入两边的值。

可以只考虑 $p_n$ 的**上界**，猜测形式为指数函数的形式

$$
p_n = \frac{c^{a+n}-1}{c^{a+b}-1} \tag{2}
$$

算期望的过程和前面一样。

使用 $(2)$ 仍然需要要解一个 m 次方程

$$
c + (1-t) c^{m+1} = t
$$

不过一般情况下 $\Delta =c-1 \ll 1$

> 例如 $m=1, t=0.51$ 时，每局的期望 $E=-0.02$，$c=1.041$

所以转换成关于 $t$ 的表达式，然后展开


$$
\begin{aligned}
t &= \frac{c(c^m-1)}{c^{m+1}-1} \\\
&=  \frac{m}{1+m} + \frac{m}{2(1+m)} \Delta + O(\Delta^2) \\\
E &= (1-t)m -t \\\
\Delta &= -\frac{2 E}{m} \\\
\end{aligned}
$$

得到估计上界的公式

$$
\begin{aligned}
c &= 1 - \frac{2 E}{m} \\\
E(M)
&= \frac{c^a - 1}{2 \ln c} \left( - 2 \ln (c-1) +(2a+1) \ln c - 2 \psi_c(1+a) \right)  \\\
\end{aligned}\tag{3}
$$

## 代码验证

### 代码

```bash
cargo add rand
```

```rust
#![allow(non_snake_case)]
use rand::prelude::*;

fn simulate(n: i32, initMoney: i32, loseProb: f64, winValue: i32, loseValue: i32) {
    let mut rng = thread_rng();
    let mut total = 0;
    let expected = winValue as f64 * (1. - loseProb) + loseValue as f64 * loseProb;
    if expected > 0. {
        println!("expected > 0, expected = {}", expected);
    }

    for _ in 0..n {
        let mut money = initMoney;
        let mut maxValue = money;

        while money > 0 {
            let randomNum: f64 = rng.gen(); // 生成 0 到 1 之间的随机数

            if randomNum < loseProb {
                money += loseValue;
            } else {
                money += winValue;
            }

            if money > maxValue {
                maxValue = money;
            }
        }
        total += maxValue;
    }
    let average = total as f64 / n as f64;
    println!("n = {}\tinitMoney(a) = {}\tloseProb(t) = {}\twinValue(m) = {} \tloseValue = {}\texpected = {:.4}\taverage = {}", n, initMoney, loseProb, winValue, loseValue, expected, average);
}

fn main() {
    let n = 10000;
    simulate(n, 10, 0.505, 1, -1);
}
```

### 计算公式

$m=1$ 时使用 $(1)$ 计算期望，$m>1$ 时使用 $(3)$ 计算期望


### 结果分析

<details>
<summary>运行结果</summary>


```
n = 10000       initMoney(a) = 1        loseProb(t) = 0.55      winValue(m) = 1         expected = -0.1000      average = 2.4201
n = 10000       initMoney(a) = 1        loseProb(t) = 0.505     winValue(m) = 1         expected = -0.0100      average = 4.7507
n = 10000       initMoney(a) = 1        loseProb(t) = 0.5005    winValue(m) = 1         expected = -0.0010      average = 6.6379
n = 10000       initMoney(a) = 1        loseProb(t) = 0.50005   winValue(m) = 1         expected = -0.0001      average = 9.31

n = 10000       initMoney(a) = 1        loseProb(t) = 0.505     winValue(m) = 1         expected = -0.0100      average = 4.4791
n = 10000       initMoney(a) = 10       loseProb(t) = 0.505     winValue(m) = 1         expected = -0.0100      average = 28.6915
n = 10000       initMoney(a) = 100      loseProb(t) = 0.505     winValue(m) = 1         expected = -0.0100      average = 145.958
n = 10000       initMoney(a) = 1000     loseProb(t) = 0.505     winValue(m) = 1         expected = -0.0100      average = 1049.2985

n = 10000000    initMoney(a) = 1        loseProb(t) = 0.505     winValue(m) = 1         expected = -0.0100      average = 4.5408291
n = 10000000    initMoney(a) = 1        loseProb(t) = 0.901     winValue(m) = 9         expected = -0.0100      average = 5.4323858
n = 10000000    initMoney(a) = 1        loseProb(t) = 0.9901    winValue(m) = 99        expected = -0.0100      average = 5.5748435
n = 10000000    initMoney(a) = 1        loseProb(t) = 0.99901   winValue(m) = 999       expected = -0.0100      average = 5.665609

n = 10000000    initMoney(a) = 10       loseProb(t) = 0.505     winValue(m) = 1         expected = -0.0100      average = 28.4152255
n = 10000000    initMoney(a) = 10       loseProb(t) = 0.901     winValue(m) = 9         expected = -0.0100      average = 45.6279621
n = 10000000    initMoney(a) = 10       loseProb(t) = 0.9901    winValue(m) = 99        expected = -0.0100      average = 54.6321773
n = 10000000    initMoney(a) = 10       loseProb(t) = 0.99901   winValue(m) = 999       expected = -0.0100      average = 55.3056739

n = 100000      initMoney(a) = 100      loseProb(t) = 0.505     winValue(m) = 1         expected = -0.0100      average = 145.93686
n = 100000      initMoney(a) = 100      loseProb(t) = 0.901     winValue(m) = 9         expected = -0.0100      average = 278.31953
n = 100000      initMoney(a) = 100      loseProb(t) = 0.9901    winValue(m) = 99        expected = -0.0100      average = 463.56179
n = 100000      initMoney(a) = 100      loseProb(t) = 0.99901   winValue(m) = 999       expected = -0.0100      average = 560.83374
```

</details>

| 初始金额 a | **每局期望** E | 步长 m | 计算结果 | 代码结果 |
| ---------- | -------------- | ------ | -------- | -------- |
| 1          | -0.1           | 1      | 2.4730   | 2.4201   |
| 1          | -0.01          | 1      | 4.5394   | 4.7507   |
| 1          | -0.001         | 1      | 6.7991   | 6.6379   |
| 1          | -0.0001        | 1      | 9.0954   | 9.31     |

最大值期望和 $\ln E$ 呈线性关系

| **初始金额** a | 每局期望 E | 步长 m | 计算结果 | 代码结果 |
| -------------- | ---------- | ------ | -------- | -------- |
| 1              | -0.01      | 1      | 4.5394   | 4.54082  |
| 10             | -0.01      | 1      | 28.414   | 28.4152  |
| 100            | -0.01      | 1      | 145.95   | 145.958  |
| 1000           | -0.01      | 1      | 1049.5   | 1049.29  |

最大值期望在 $a$ 较小时是线性的，但是在 $a$ 较大时，增长速度减慢


| **初始金额** a | 每局期望 E | **步长** m | 计算结果    | 代码结果 |
| -------------- | ---------- | ---------- | ----------- | -------- |
| 1              | -0.01      | 1          | **4.549**   | 4.54082  |
| 1              | -0.01      | 9          | 6.6956      | 5.43239  |
| 1              | -0.01      | 99         | 9.0854      | 5.57484  |
| 1              | -0.01      | 999        | 11.396      | 5.66561  |
| 10             | -0.01      | 1          | **28.4943** | 28.4152  |
| 10             | -0.01      | 9          | 48.1239     | 45.6280  |
| 10             | -0.01      | 99         | 71.6277     | 54.6322  |
| 10             | -0.01      | 999        | 94.6799     | 55.3057  |
| 100            | -0.01      | 1          | **146.343** | 145.958  |
| 100            | -0.01      | 9          | 280.231     | 278.319  |
| 100            | -0.01      | 99         | 494.695     | 463.562  |
| 100            | -0.01      | 999        | 721.585     | 560.834  |


当 $m\leq a$ 时，计算结果较为准确

这是因为将 $p_n$ 近似成了指数函数，当 $a$ 较小时，近似不准确。

当 $a=1$ 时，有 $t$ 的概率转移到 $m+1$，$1-t$ 的概率转移到 $0$

所以先计算一个方向上的最大值的极限


$$
\begin{aligned}
c &= 1 - \frac{2E}{m} \\\
a &= m + 1 \\\
L=\lim_{m\to \infty} \frac{a+E(M)}{a} &= 1 - E + \ln-\frac{1}{2E}
\end{aligned}
$$

一个更严格的上界

$$
\begin{aligned}
\lim_{m\to \infty} E'(M) &= \lim_{m\to \infty} \left(\frac {a}{m} L m + \left(1 - \frac{a}{m}\right) a\right) \\\
&= (L+1)a \\\
&= (2 - E + \ln-\frac{1}{2E}) a \\\
\end{aligned} \tag{4}
$$

例如当 $E=-\frac{1}{100}$ 时，$E'M < 5.92\ a$

> 其实有不严格的地方，懒得说明 $E'M$ 是随 m 单调的了


## 结论

本文讨论了赌徒输光前最大值的期望问题

对于等概率等步长的情况

- 最大值超过本金的 $k$ 倍的概率，$P(M \geq k a) = \frac{1}{k+1}$
- 期望是无穷大，$E(M) = \infty$

分析最大值期望与初始金额、期望、步长之间的关系 $(1)$ $(3)$

通过代码验证了计算结果。给出了更精确的上界 $(4)$

## 后记

起因是我之前在 bilibili 上看到了 [戒社](https://space.bilibili.com/27492426) 的视频，他的视频里有很多赌狗的自白（在我看来他们就是不懂大数定律，也不懂信息安全），这些人似乎喜欢谈自己曾经赢到过多少。于是想到的这个问题。


> 彩票公司谨小慎微，避免一切招摇。它的代理人自然都是秘密的：公司源源不断发出的指令同骗子层出不穷的花招没有区别。再说，有谁能自诩为单纯的骗子呢？醉汉心血来潮发出荒唐的命令。做梦的人突然醒来掐死了睡在他身旁的老婆，他们岂非是执行公司的秘密指示？这种默默无声的运转可同上帝的旨意相比，引起各种各样的猜测。有一种猜测恶毒地暗示说公司已经消失了几百年，我们生活中的神圣的混乱纯属遗传和传统；另一种猜测认为公司是永恒的，声称它将持续到最后一位上帝消灭世界之前的最后一个夜晚。还有一种猜测说公司无所不能，但干预一些微不足道的小事：鸟鸣、铁锈和灰尘的颜色、破晓时的迷糊等等。再有一种猜测借异端创始人之口说公司以前没有，以后也不会有。还有一种同样恶劣的说法认为肯定或否认那个诡秘的公司的存在无关紧要，因为巴比伦无非是一场无限的赌博。
