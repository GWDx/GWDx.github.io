---
title: "再谈核苷类似物"
date: 2025-03-23
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

从生物化学、药物化学的角度解释核苷类似物如何磷酸化，Acyclovir、Lamivudine 如何发挥抗病毒作用。再谈 Lamivudine、Tenofovir 是否对 COVID 有效。

<!--more-->

[核苷类似物](https://en.wikipedia.org/wiki/Nucleoside_analogue)（nucleoside analogue）主要用作抗病毒药物，也有抗癌、分子生物学研究等用途：

- 生活中最常见到的应该是阿昔洛韦（Acyclovir），毕竟疱疹病毒（HSV）相对常见。
- 电影《达拉斯买家俱乐部》中出现了齐多夫定，是治疗 HIV 的药物。
- 分子生物学的一代测序中，使用 ddNTP 来随机终止 DNA 链。

<br>

之前写过一篇简单介绍核苷类似物的 [blog](https://gwdx.github.io/post/nucleoside-analogue/)，但感觉写得有点烂，所以重新写一下。上次有两个问题没有解决：

- 问题 1：为什么相比 dC，Lamivudine 的手性是反的？
- 问题 2：为什么 Lamivudine、Tenofovir 既能抑制 DNA 病毒，又被认为能抑制 RNA 病毒？

Lamivudine（拉米夫定，3TC）构型比较怪，并且两个问题都涉及；Acyclovir（阿昔洛韦，ACV）和 TK 有关；所以本文主要介绍这两种核苷类似物。另外，还要用循证医学解释 Lamivudine、Tenofovir 是否对 COVID 有效。

<br>

## 1. 背景介绍

首先介绍一下本文涉及的蛋白质、常见病毒、核苷类似物。

### 1.1 蛋白质

本文主要涉及的蛋白质：

- 核苷激酶 dNK
- 聚合酶 DdDp、DdRp、RdDp、RdRp

蛋白质的四级结构：

- 一级结构：氨基酸序列
- 二级结构：局部的空间构象，主要是指 α-螺旋、β-折叠
- 三级结构：单个亚基不同位置的相互作用
- 四级结构：多个亚基之间的相互作用

#### 氨基酸对照表

因为后文会提到氨基酸，所以这里先列出来。

> [标准蛋白氨基酸列表 - 维基百科](https://zh.wikipedia.org/wiki/標準蛋白胺基酸列表)
>
> [DNA密码子表 - 维基百科](https://zh.wikipedia.org/wiki/DNA密码子表)

氨基酸对照表（部分）：

| 氨基酸名称 | 三字母 | 一字母 |
| ---------- | ------ | ------ |
| 谷氨酸     | Glu    | E      |
| 苯丙氨酸   | Phe    | F      |
| 酪氨酸     | Tyr    | Y      |
| 丙氨酸     | Ala    | A      |
| 精氨酸     | Arg    | R      |
| 天冬酰胺   | Asn    | N      |
| 谷氨酰胺   | Gln    | Q      |
| 甲硫氨酸   | Met    | M      |
| 缬氨酸     | Val    | V      |
| 异亮氨酸   | Ile    | I      |

> 突变表示的示例：突变 M184V 表示 DNA/RNA 的突变导致蛋白质 184 位的 Met 变成了 Val。

<br>

### 1.2 病毒

本文主要涉及的病毒：HSV、HIV、HBV、SARS-CoV-2。

一些常见的病毒、现有的药物治疗方法：

| 中文名                          | 简称       | [Baltimore 分类](https://zh.wikipedia.org/wiki/%E5%B7%B4%E7%88%BE%E7%9A%84%E6%91%A9%E7%97%85%E6%AF%92%E5%88%86%E9%A1%9E%E7%B3%BB%E7%B5%B1) | 复制酶                     | 核苷类似物                                       | 其他药物                                   |
| ------------------------------- | ---------- | ------------------------------------------------------------------------------------------------------------------------------------------ | -------------------------- | ------------------------------------------------ | ------------------------------------------ |
| 单纯疱疹病毒                    | HSV        | 1 双链 DNA（dsDNA）                                                                                                                        | DNAP                       | 阿昔洛韦，泛昔洛韦，伐昔洛韦                     |                                            |
| 人类免疫缺陷病毒                | HIV        | 6 正单链 RNA 逆转录（ssRNA-RT）                                                                                                            | RT（具有 RdDp, DdDp 功能） | TDF，恩曲他滨 FTC,  3TC                          | NNRTI：依非韦伦 EFV，奈韦拉平 NVP<br />DTG |
| 乙型肝炎病毒                    | HBV        | 7 双链 DNA 逆转录（dsDNA-RT）                                                                                                              | RT（具有 RdDp, DdDp 功能） | TDF，恩替卡韦 ETV，替比夫定，  3TC，阿德福韦 ADV |                                            |
| 丙型肝炎病毒                    | HCV        | 4 单链 RNA（(+)ssRNA）                                                                                                                     | RdRp                       | Sofosbuvir                                       | DAA：维帕他韦                              |
| 严重急性呼吸系统综合征冠状病毒2 | SARS-CoV-2 | 4 单链 RNA（(+)ssRNA）                                                                                                                     | RdRp                       | 瑞德西韦，莫诺拉韦                               | 3CLpro 抑制剂：奈玛特韦、利托那韦          |

> 没认真查指南，列出的药物不一定对。

针对 HIV RT 的药物主要包括两类：

- NRTI 一般作为链终止剂
- NNRTI 一般作为非竞争性抑制剂

<center>
<img src="fig/HIV-RT-drug.jpg" alt="HIV-RT-drug" style="zoom:50%;" />
</center>


> **[1]** [Insights into HIV-1 Reverse Transcriptase (RT) Inhibition and Drug Resistance from Thirty Years of Structural Studies](https://pmc.ncbi.nlm.nih.gov/articles/PMC9148108/)
>

<br>

### 1.3 核苷类似物

最常见的碱基有五种：腺嘌呤、鸟嘌呤、胞嘧啶、胸腺嘧啶、尿嘧啶。碱基和核糖结合起来就是核苷。

本文涉及的核苷类似物：

- Lamivudine
- Tenofovir
- Acyclovir

> 本文只关注药效，不关注药动。所以**不区分核苷类似物和核苷酸类似物**，不特别区分 TDF 和 Tenofovir。

<details open>
<summary><b>常见的核苷类似物</b></summary>

> 大部分是 [wiki](https://en.wikipedia.org/wiki/Nucleoside_analogue) 上列出的。加粗表示天然存在的**核苷、脱氧核苷**

**腺苷类**

| 中文名                                                       | 英文名                                                   | 缩写            | 结构式                                                       | 描述                                           |
| ------------------------------------------------------------ | -------------------------------------------------------- | --------------- | ------------------------------------------------------------ | ---------------------------------------------- |
| [脱氧腺苷](https://zh.wikipedia.org/wiki/%E8%84%AB%E6%B0%A7%E8%85%BA%E8%8B%B7) | **Deoxyadenosine**                                       | dA              | <img src="A/958-09-8 Deoxyadenosine.svg" alt="958-09-8 Deoxyadenosine" /> |                                                |
| [肌苷](https://zh.wikipedia.org/wiki/%E8%82%8C%E8%8B%B7)<br />次黄苷 | **Inosine**                                              | I               | <img src="A/58-63-9 Inosine.svg" alt="58-63-9 Inosine" />    |                                                |
| [地达诺新](https://zh.wikipedia.org/wiki/%E5%9C%B0%E8%BE%BE%E8%AF%BA%E6%96%B0)<br />双脱氧肌苷 | Didanosine                                               | ddI             | <img src="A/69655-05-6 Didanosine.svg" alt="69655-05-6 Didanosine" /> | I 少了两个羟基                                 |
| [阿糖腺苷](https://zh.wikipedia.org/wiki/%E9%98%BF%E7%B3%96%E8%85%BA%E8%8B%B7) | Vidarabine                                               | ara-A           | <img src="A/24356-66-9 9H-Purin-6-amine, 9-β-D-arabinofuranosyl-, hydrate (1:1).svg" alt="24356-66-9 9H-Purin-6-amine, 9-β-D-arabinofuranosyl-, hydrate (1:1)" /> | A 改成阿拉伯糖（2' 羟基手性异构）              |
| [腺苷](https://zh.wikipedia.org/wiki/%E8%85%BA%E8%8B%B7)     | **Adenosine**                                            | A               | <img src="A/58-61-7 Adenosine.svg" alt="58-61-7 Adenosine" /> |                                                |
|                                                              | [Galidesivir](https://en.wikipedia.org/wiki/Galidesivir) |                 | <img src="A/222631-44-9 3,4-Pyrrolidinediol, 2-(4-amino-5H-pyrrolo[3,2-d]pyrimidin-7-yl)-5-(hydroxymethyl)-, hydrochloride (1:1), (2S,3S,4R,5R)-.svg" alt="222631-44-9 3,4-Pyrrolidinediol, 2-(4-amino-5H-pyrrolo[3,2-d]pyrimidin-7-yl)-5-(hydroxymethyl)-, hydrochloride (1:1), (2S,3S,4R,5R)-" /> | dA 的基础上，NH 取代 O，嘌呤环变了             |
| [瑞德西韦](https://zh.wikipedia.org/wiki/%E7%91%9E%E5%BE%B7%E8%A5%BF%E9%9F%A6) | Remdesivir                                               |                 | <img src="A/1809249-37-3 L-Alanine, N-[(S)-hydroxyphenoxyphosphinyl]-, 2-ethylbutyl ester, 6-ester with 2-C-(4-aminopyrrolo[2,1-f][1,2,4]triazin-7-yl)-2,5-anhydro-D-altrononitrile.svg" alt="1809249-37-3 L-Alanine, N-[(S)-hydroxyphenoxyphosphinyl]-, 2-ethylbutyl ester, 6-ester with 2-C-(4-aminopyrrolo[2,1-f][1,2,4]triazin-7-yl)-2,5-anhydro-D-altrononitrile" /> | A 的基础上加了个氰基，5'-OH 连酯基。嘌呤也变了 |
| [替诺福韦](https://zh.wikipedia.org/wiki/%E6%9B%BF%E8%AF%BA%E7%A6%8F%E9%9F%A6%E4%BA%8C%E5%90%A1%E5%91%8B%E9%85%AF) | Tenofovir                                                | TFV  (TDF, TAF) | <img src="A/201341-05-1 Tenofovir disoproxil.svg" alt="201341-05-1 Tenofovir disoproxil" /> | AMP 酯化。去掉 O C，开环，加甲基。             |

**胞苷类**

| 中文名                                                                         | 英文名                                                   | 缩写  | 结构式                                                                        | 描述                                            |
| ------------------------------------------------------------------------------ | -------------------------------------------------------- | ----- | ----------------------------------------------------------------------------- | ----------------------------------------------- |
| [胞苷](https://zh.wikipedia.org/wiki/%E8%83%9E%E8%8B%B7)                       | **Cytidine**                                             | C     | <img src="C/65-46-3 Cytidine.svg" alt="65-46-3 Cytidine" />                   |                                                 |
| [阿糖胞苷](https://zh.wikipedia.org/wiki/%E9%98%BF%E7%B3%96%E8%83%9E%E8%8B%B7) | Cytarabine                                               | ara-C | <img src="C/147-94-4 Cytarabine.svg" alt="147-94-4 Cytarabine" />             | C 改成阿拉伯糖                                  |
| [吉西他滨](https://zh.wikipedia.org/wiki/%E5%90%89%E8%A5%BF%E4%BB%96%E6%BB%A8) | Gemcitabine                                              |       | <img src="C/95058-81-4 Gemcitabine.svg" alt="95058-81-4 Gemcitabine" />       | C 的 2' 改成两个 F                              |
| [脱氧胞苷](https://zh.wikipedia.org/wiki/%E8%84%AB%E6%B0%A7%E8%83%9E%E8%8B%B7) | **Deoxycytidine**                                        | dC    | <img src="C/951-77-9 Deoxycytidine.svg" alt="951-77-9 Deoxycytidine" />       |                                                 |
|                                                                                | [Zalcitabine](https://en.wikipedia.org/wiki/Zalcitabine) | ddC   | <img src="C/7481-89-2 Dideoxycytidine.svg" alt="7481-89-2 Dideoxycytidine" /> | dC 上再去掉一个羟基                             |
| [拉米夫定](https://zh.wikipedia.org/wiki/%E6%8B%89%E7%B1%B3%E5%A4%AB%E5%AE%9A) | Lamivudine                                               | 3TC   | <img src="C/134678-17-4 Lamivudine.svg" alt="134678-17-4 Lamivudine" />       | dC 的基础上把羟基改成环上的 S，并且手性变了两个 |
| [恩曲他滨](https://zh.wikipedia.org/wiki/%E6%81%A9%E6%9B%B2%E4%BB%96%E6%BB%A8) | Emtricitabine                                            | FTC   | <img src="C/143491-57-0 Emtricitabine.svg" alt="143491-57-0 Emtricitabine" /> | 3TC 的基础上加一个 F                            |

**鸟苷类**

| 中文名                                                       | 英文名             | 缩写 | 结构式                                                       | 描述                                                         |
| ------------------------------------------------------------ | ------------------ | ---- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [鸟苷](https://zh.wikipedia.org/wiki/%E9%B3%A5%E8%8B%B7)     | **Guanosine**      | G    | <img src="G/118-00-3 Guanosine.svg" alt="118-00-3 Guanosine" /> |                                                              |
| [利巴韦林](https://zh.wikipedia.org/wiki/%E5%88%A9%E5%B7%B4%E9%9F%A6%E6%9E%97) | Ribavirin          |      | <img src="G/36791-04-5 Ribavirin.svg" alt="36791-04-5 Ribavirin" /> | G 的嘌呤变成三氮唑+酰胺                                      |
| [阿昔洛韦](https://zh.wikipedia.org/wiki/%E9%98%BF%E6%98%94%E6%B4%9B%E9%9F%A6) | Acyclovir          | ACV  | <img src="G/59277-89-3 Acyclovir.svg" alt="59277-89-3 Acyclovir" /> | G 的 2' 3' 碳去掉                                            |
| [更昔洛韦](https://zh.wikipedia.org/wiki/%E6%9B%B4%E6%98%94%E6%B4%9B%E9%9F%A6) | Ganciclovir        | GCV  | <img src="G/82410-32-0 Ganciclovir.svg" alt="82410-32-0 Ganciclovir" /> | Acyclovir 再加一条链                                         |
| [恩替卡韦](https://zh.wikipedia.org/wiki/%E6%81%A9%E6%9B%BF%E5%8D%A1%E9%9F%A6) | Entecavir          | ETV  | <img src="G/142217-69-4 Entecavir.svg" alt="142217-69-4 Entecavir" /> | dG 糖上的 O 变成双键                                         |
| [阿巴卡韦](https://zh.wikipedia.org/wiki/%E9%98%BF%E5%B7%B4%E5%8D%A1%E7%B6%AD) | Abacavir           | ABC  | <img src="G/136470-78-5 Abacavir.svg" alt="136470-78-5 Abacavir" /> | G 的嘌呤的 O 变成了环丙基 NH，糖部分类似 d4T（变成双键），并且 O 变成 C |
| [脱氧鸟苷](https://zh.wikipedia.org/wiki/%E8%84%AB%E6%B0%A7%E9%B3%A5%E8%8B%B7) | **Deoxyguanosine** | dG   | <img src="G/961-07-9 Deoxyguanosine.svg" alt="961-07-9 Deoxyguanosine" /> |                                                              |

**胸苷、尿苷类**

| 中文名                                                                         | 英文名           | 缩写     | 结构式                                                                                  | 描述                      |
| ------------------------------------------------------------------------------ | ---------------- | -------- | --------------------------------------------------------------------------------------- | ------------------------- |
| [胸苷](https://zh.wikipedia.org/wiki/%E8%83%B8%E8%8B%B7)<br />脱氧胸苷         | **Thymidine**    | dT       | <img src="T/50-89-5 Thymidine.svg" alt="50-89-5 Thymidine" />                           |                           |
| [碘苷](https://zh.wikipedia.org/wiki/%E7%A2%98%E8%8B%B7)                       | Idoxuridine      |          | <img src="T/54-42-2 5-Iodo-2′-deoxyuridine.svg" alt="54-42-2 5-Iodo-2′-deoxyuridine" /> | dT 的甲基变成 I           |
| [替比夫定](https://zh.wikipedia.org/wiki/%E6%9B%BF%E6%AF%94%E5%A4%AB%E5%AE%9A) | Telbivudine      |          | <img src="T/3424-98-4 Telbivudine.svg" alt="3424-98-4 Telbivudine" />                   | dT 环沉下去了（类似 3TC） |
| [司他夫定](https://zh.wikipedia.org/wiki/%E5%8F%B8%E4%BB%96%E5%A4%AB%E5%AE%9A) | Stavudine        | d4T      | <img src="T/3056-17-5 Stavudine.svg" alt="3056-17-5 Stavudine" />                       | dT 上 2', 3' 变成双键     |
| [齐多夫定](https://zh.wikipedia.org/wiki/%E9%BD%90%E5%A4%9A%E5%A4%AB%E5%AE%9A) | Zidovudine       | ZDV, AZT | <img src="T/30516-87-1 Azidothymidine.svg" alt="30516-87-1 Azidothymidine" />           | dT 上的 3' 变成叠氮       |
| [脱氧尿苷](https://zh.wikipedia.org/wiki/%E8%84%B1%E6%B0%A7%E5%B0%BF%E8%8B%B7) | **Deoxyuridine** | dU       | <img src="T/951-78-0 Deoxyuridine.svg" alt="951-78-0 Deoxyuridine" />                   |                           |
| [三氟胸苷](https://zh.wikipedia.org/wiki/%E4%B8%89%E6%B0%9F%E8%83%B8%E8%8B%B7) | Trifluridine     | TFT      | <img src="T/70-00-8 Trifluridine.svg" alt="70-00-8 Trifluridine" />                     | 胸苷的甲基变成 CF3        |

</details>

<br>

## 2. dNK 与 Acyclovir

在讨论 Acyclovir 之前，需要先介绍核苷激酶 dNK。

### 2.1 核苷激酶 dNK

核苷激酶（dNK）可以向脱氧胸苷（dN）上加一个磷酸：

$$
\ce{\text{dN} + ATP ->[\text{dNK}] \text{dNMP} + ADP}
$$

后面变成 dNTP 还需要两个激酶（本文不讨论）：

$$
\begin{gather*}
\ce{\text{dNMP} + ATP ->[\text{dNMP kinase}] \text{dNDP} + ADP} \\
\ce{\text{d}NDP + ATP ->[NDPK] \text{d}NTP + ADP}
\end{gather*}
$$


dNK 的种类：
- 人 17 号染色体上的 TK1，几乎只磷酸化 dT
- 人线粒体上的 TK2，可以磷酸化 dT、dC
- 单纯疱疹病毒的 HSV1-TK 可以磷酸化 dT、dC、dG


> 人体除了 TK1、TK2，还有脱氧胞苷激酶（hdCK）、线粒体中的脱氧鸟苷激酶（hdGK）。似乎还有 UCK2。

正常人 S 期细胞表达 TK1，通过补救途径合成 dTTP，用于 DNA 复制。

#### HSV 为什么有胸苷激酶

> [疱疹病毒没有胸苷激酶能活吗？](https://www.zhihu.com/question/624272950/answer/3600444022)

HSV 的 TK 基因对于饥饿细胞是必要的，但对于活跃细胞中的病毒复制不是必需的 [2]。 \
HSV 缺失 TK 基因后毒力（致病力，Virulence）降低 [3]。

所以，HSV 自备一个 TK 基因能够在任何细胞周期保证核苷激酶的活性，有更高的传播速率。

> [2] [Induction of Both Thymidine and Deoxycytidine Kinase Activity by Herpes Viruses](https://www.microbiologyresearch.org/content/journal/jgv/10.1099/0022-1317-24-3-465) \
> [3] [Alpha-Herpesvirus Thymidine Kinase Genes Mediate Viral Virulence and Are Potential Therapeutic Targets](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC6517553)

#### 催化机制

脱氧核苷激酶具有保守的 **Glu-Arg pair**。

- 谷氨酸（Glu）是酸性氨基酸（等电点 pI = 3.15）
- 精氨酸（Arg）是碱性氨基酸（等电点 pI = 10.76）

两者之间可以形成 [蛋白质中的盐桥](https://en.wikipedia.org/wiki/Salt_bridge_(protein_and_supramolecular))。

估计然后是：碱性氨基酸带正电，酸性氨基酸带负电，可以形成电场，让氢离子更容易飞掉。dNK 的 P-loop 与 ATP 的磷酸基团结合，dN 和 ATP 发生 **SN2** 亲核取代反应（[NTR 取代](https://www.bilibili.com/video/BV1SG411v7xa)）。

#### dNK 与核苷类似物结合

> [为啥在细胞内，阿昔洛韦（鸟苷类似物）能与脱氧核苷竞争病毒胸腺激酶或细胞激酶？](https://www.zhihu.com/question/384341344/answer/3600864138)

HSV1-TK 的二聚体具有催化活性。更昔洛韦（Ganciclovir）可以结合到 HSV1-TK 上：

<center>
<img src="fig/dNK/Ganciclovir.png" alt="Ganciclovir" style="zoom:50%;" />
</center>


3TC 和 ADP 可以结合到人脱氧胞苷激酶（dCK）上：

<center>
<img src="fig/dNK/3TC-ADP.png" alt="3TC-ADP" style="zoom:50%;" />
</center>


#### dNK 对核苷类似物的催化效果

TK1、TK2、dCK、dGK、HSV1-TK 催化各种底物的 Kcat/Km：

<center>
<img src="fig/dNK/dNK.png" alt="dNK" style="zoom:50%;" />
</center>


> 在米氏方程中，Kcat/Km 用于衡量酶对底物的偏好程度。

HSV1-TK 处理 **Acyclovir** 的 Kcat/Km 大约是 10^2 的量级，勉强能反应。而人的 TK1、TK2 无法催化 Acyclovir。

鸟苷（G）在环上多了一个 OH，形成了氢键，所以亲和力高。估计 Ganciclovir 也是因为多了一个 OH，Kcat/Km 有提升。

> **[4]** [Human and viral nucleoside/nucleotide kinases involved in antiviral drug activation: Structural and catalytic properties](https://www.sciencedirect.com/science/article/pii/S0166354210000306)

<br>

### 2.2 Acyclovir 的药理

Acyclovir 在被消化系统**吸收**后，会进入血液，分布到组织中，然后可能是通过核苷转运蛋白易化扩散**进入细胞**。在细胞内，进行三次**磷酸化**，形成活性的三磷酸核苷，然后与**病毒的复制酶结合**，加入到 DNA 链上，阻止 DNA 链的延伸。

因为 ACV 的第一次磷酸化只能由 HSV1-TK 进行，而不能被人体核苷激酶进行，所以 Acyclovir 只作用于被 HSV 感染的细胞。

并且，相比细胞 DNA 聚合酶，ACV-TP 更倾向于结合病毒 DNA 聚合酶，终止 DNA 链。

> **[5]** 简明药物化学

<br>

## 3. 聚合酶与 Lamivudine

先谈聚合酶，解释为什么复制时 DNAP 对 dNTP、转录时 RNAP 对 NTP 具有选择性。再谈 Lamivudine 的构型、结合方式。

聚合酶的作用是将核苷酸链合成 DNA 或 RNA：

$$
\begin{gather*}
\ce{DNA_{n} + \text{dNTP} ->[\text{DNAP}] DNA_{n+1} + PP_{i}} \\
\ce{RNA_{n} + NTP ->[\text{RNAP}] RNA_{n+1} + PP_{i}}
\end{gather*}
$$

### 3.1 DNAP、RNAP 如何区分 dNTP、NTP


> [转录时，RNA聚合酶如何分辨脱氧核苷酸和核糖核苷酸？](https://www.zhihu.com/question/528924457/answer/3605035908)

**TLDR**：RNAP 依靠**氢键**之类的相互作用、**NTP 的浓度**区分 dNTP 和 NTP，而 DNAP 则依靠**位阻**区分两者。

#### DNA 聚合酶（DNAP、DdDp）

> 真核生物中，细胞核 DNA 复制主要由 DNAP α、δ、ε 完成。

在酿酒酵母菌中，核糖核苷三磷酸（NTP）的浓度是 500-3000 μM，脱氧核苷三磷酸（dNTP） 的浓度是 12-30 μM，NTP/dNTP 介于 36-190 之间。它的 DNA 聚合酶ε 可能会把 NTP 当作 dNTP 使用。[6]

人类线粒体中 DNAP γ 对 dTTP/UTP 的选择性大约为 77000，而对 dGTP/GTP 的选择性大约为 1100 [7]。

DNA 聚合酶主要通过较大的侧链残基（如 Glu、Phe、Tyr），构成 steric gate，在空间上阻断核糖环的 2'-OH [8]。


一些生物的 DNAP 和 ddNTP 结合的位点（C2' 表示核糖环的 2' 位碳原子）：

<center>
<img src="fig/DNAP-RNAP/DNAP-ddNTP.png" alt="DNAP-ddNTP" style="zoom:50%;" />
</center>


对于聚合酶 Dpo4，酪氨酸比较大，ATP 的 2'-OH 会被排除，只允许 dATP 反应。Y12A 突变后，变成了丙氨酸，NTP 可以进入。

<center>
<img src="fig/DNAP-RNAP/DNAP.png" alt="DNAP" style="zoom: 25%;" />
</center>


> [6] [Abundant ribonucleotide incorporation into DNA by yeast replicative polymerases](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC2841928) \
> [7] [Ribonucleotide Discrimination and Reverse Transcription by the Human Mitochondrial DNA Polymerase](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3173122) \
> **[8]** [Unlocking the Sugar ‘Steric Gate’ of DNA Polymerases](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3040255)


#### RNA 聚合酶（RNAP、DdRp）

RNA 聚合酶 II 主要用于合成 mRNA。

RNAP 的选择性一方面和细胞内 NTP 的浓度有关（上文已说明），另一方面和 RNAP 的结构有关。

##### T7 噬菌体的 RNAP

T7 噬菌体的 RNAP 是单亚基的，比较简单。它使用 NTP 的效率是 dNTP 的 70-80 倍。[9]

Y639F 突变导致 dNTP 掺入量增加 20 倍，说明 **639 位的 Tyr** 对区分 NTP 和 dNTP 有重要作用 [10] [11]。

Tyr 和 NTP 之间夹了一个 $\ce{Mg^2+}$，猜测是两个羟基上的氧原子带负电荷，可以与 $\ce{Mg^2+}$ 吸引 [10]。

<center>
<img src="fig/DNAP-RNAP/RNAP.png" alt="RNAP" style="zoom:50%;" />
</center>

> [9] [Mechanism of Ribose 2‘-Group Discrimination by an RNA Polymerase](https://pubs.acs.org/doi/10.1021/bi962674l) \
> **[10]** [Structural Basis for Substrate Selection by T7 RNA Polymerase](https://www.cell.com/fulltext/S0092-8674(04)00059-5) \
> [11] [The highly efficient T7 RNA polymerase: A wonder macromolecule in biological realm](https://www.sciencedirect.com/science/article/pii/S014181301830415X)


##### 真核生物的 RNAP

多亚基 RNAP 选择性的具体机制不太清楚，可能是通过 β′Arg425、β′Asn458、β′Gln929 的氢键来区分 NTP 和 dNTP 的 [7]。

<center>
<img src="fig/DNAP-RNAP/真核RNAP.png" alt="真核RNAP" style="zoom: 25%;" />
</center>

> 这里又说 T7 RNAP 是通过氢键来区分的，而不是和 $\ce{Mg^2+}$ 的配位键。不知道哪个对。

> **[12]** [The mechanism of the nucleo-sugar selection by multi-subunit RNA polymerases](https://www.nature.com/articles/s41467-021-21005-w)

<br>

### 3.2 Lamivudine

#### 回答问题 1：Lamivudine 的构型（手性）

**问题 1**：为什么相比 dC，Lamivudine 的手性是反的？

在 [表格](#13-核苷类似物) 中，可以发现其他药物也有类似的设计（~~说明不是打印错了~~）：

- 替比夫定和 dT 除了手性没有区别
- FTC 和 3TC 只有嘧啶上的 F 的区别

> Lamivudine 的化学名是 (-)-L-2′,3′-dideoxy-3′-thiacytidine。

(+)-3TC 被 HIV RT 掺杂进入 DNA 的效率更高。但实际使用 (-)-3TC，可能是因为**毒性更低、RNase H 降解速率低、稳定性高** [13] [14]

> [13] [Mechanistic Studies Comparing the Incorporation of (+) and (−) Isomers of 3TCTP by HIV-1 Reverse Transcriptase](https://pubs.acs.org/doi/10.1021/bi982340r) \
> [14] [Perspectives on the molecular mechanism of inhibition and toxicity of nucleoside analogs that target HIV-1 reverse transcriptase](https://www.sciencedirect.com/science/article/pii/S0925443902000923)

上面说了 (-)-3TC 更有效，但没有解释原因。下面详细说明 Lamivudine 对 HBV/HIV RT 的**结合方式**相比 dC 有什么不同。

#### Lamivudine 如何与 HBV/HIV RT 结合

> [恩替卡韦容易耐药吗？](https://www.zhihu.com/question/39953567/answer/102425668)

3TC、恩替卡韦 ETV、Tenofovir 是常见的 HBV 药物。HBV 一个 M204V/I、HIV 一个 M184V/I 突变就能把 Lamivudine 打趴下（耐药），这类突变也会让 ETV 部分耐药。

HBV RT 溶解度差，蛋白质结构解析不出来。所以在 HIV 上做一些突变得到 HIV 3MB，来模拟 HBV。然后用 X 射线衍射计算蛋白质结构。

<center>
<img src="fig/3TC-ETV-RT-1.webp" alt="3TC-ETV-RT-1" style="zoom: 25%;" />
</center>

发现 3TC 与 HIV 3MB 的结合有以下特点：


- 与 RT 的 $\ce{Mg^2+}$ 结合：ETV-TP 结合方式和 dNTP 相同，磷酸占了  $\ce{Mg^2+}$ 八面体配位的三个方向。而 3TC-TP 前两个磷酸是倒过来结合的。
- 与 DNA 的作用：因为 3TC-TP 的 ɑ 磷酸距离 DNA 的 3'-OH 比较远，所以不能掺杂进入 DNA，所以是竞争抑制剂，而不是链终止剂。
- 与 RT Met184 的位阻：因为 3TC 的 S 原子、ETV 的双键的位阻作用，Met184 的构象被推了出去。如果 HIV 3MB 发生了 M184V/I 突变，就会有位阻，无法有效 dock（似乎这里 docking 才是更规范的写法）。

Tenofovir 不含核糖环（类似 Acyclovir），所以对 M184V 不敏感。

![3TC-ETV-RT-2](fig/3TC-ETV-RT-2.webp)

> **[15]** [Structural features in common of HBV and HIV-1 resistance against chirally-distinct nucleoside analogues entecavir and lamivudine](https://www.nature.com/articles/s41598-020-59775-w)

HIV RT M184V 突变后 Kp/Kd 显著减小，对 (-)-3TC (-)-FTC 的亲和力显著减小。突变后距离太近，有位阻。

![M184V](fig/M184V.webp)

> [16] [Elucidating molecular interactions of L-nucleotides with HIV-1 reverse transcriptase and mechanism of M184V-caused drug resistance](https://www.nature.com/articles/s42003-019-0706-x)

#### 回答问题 2

**问题 2**：为什么 Lamivudine、Tenofovir 既能抑制 DNA 病毒，又被认为能抑制 RNA 病毒？

先纠正一点：HIV 是 RNA 病毒；HBV 虽然是 DNA 病毒，但是有 RT。

至于有人说 [Lamivudine、Tenofovir 可以和 SARS-CoV-2 的 RdRp 结合](https://zhuanlan.zhihu.com/p/594961987)。个人认为是可能的，前面问题 1 手性不一样都能塞进去，只能说**能 dock 上就行**。

> 有些药物有多种看起来更不相关的作用。比如甲氨蝶呤能抗癌，也能用来治疗类风湿性关节炎（RA）。

<br>

## 4. 循证医学

至于 Lamivudine、TDF 对于 COVID 临床是否有效，需要循证医学。 \
现有的指南没有提到这两种药物，所以需要查综述（系统评价、荟萃分析），看 RCT（随机对照实验）。

综述说 Lamivudine 和 Tenofovir 可能有用。

> [17] [Effectiveness of Drug Repurposing and Natural Products Against SARS-CoV-2: A Comprehensive Review](https://www.tandfonline.com/doi/full/10.2147/CPAA.S429064) \
> [18] [Navigating the COVID-19 Therapeutic Landscape: Unveiling Novel Perspectives on FDA-Approved Medications, Vaccination Targets, and Emerging Novel Strategies](https://www.mdpi.com/1420-3049/29/23/5564)

我不太会看 RCT，感觉就是有的说 Tenofovir 在某些情况下有效果，有的说不显著。具体而言，一个 RCT 说 TDF/FTC、秋水仙碱、瑞舒伐他汀联合用药有治疗效果 [19]，另一个说 TDF/FTC 对于预防 COVID 效果不显著 [20]。

> [19] [Effectiveness of rosuvastatin plus colchicine, emtricitabine/tenofovir and combinations thereof in hospitalized patients with COVID-19: a pragmatic, open-label randomized trial](https://pmc.ncbi.nlm.nih.gov/articles/PMC8686571/) \
> [20] [Daily tenofovir disoproxil fumarate/emtricitabine and hydroxychloroquine for pre-exposure prophylaxis of COVID-19: a double-blind placebo-controlled randomized trial in healthcare workers](https://www.sciencedirect.com/science/article/pii/S1198743X22003706)



**2025-11 更新：**

找到一篇新的系统评价，得出了相近的结论：关于使用 Tenofovir 对 COVID 的治疗，仅筛选出 3 项 RCT，且结果相互冲突；现有证据不支持将 Tenofovir 用于 COVID-19 的常规治疗，仍需更多高质量 RCT [21]。

> [21] [Efficacy of tenofovir on clinical outcomes of COVID-19 patients: a systematic review](https://pmc.ncbi.nlm.nih.gov/articles/PMC12312381/)



<br>

## 5. 总结

- **Acyclovir**：ACV 可以被病毒激酶 HSV1-TK 变成 ACV-MP，从而只在病毒感染的细胞中发挥链终止剂的作用。
- **DNAP、RNAP**：RNA 聚合酶依靠氢键之类的相互作用、NTP 的浓度区分 dNTP 和 NTP，而 DNA 聚合酶依靠位阻区分两者。
- **Lamivudine**：因为 3TC 是 L 构型，3TC-TP 与 HBV RT 的结合方式与 dCTP 的结合方式不同，降低了毒性，但也导致更容易耐药（M184V 突变）。
- **循证医学**：目前的指南、综述不认为 3TC、TDF 对 COVID 有效。

<br>

## 6. 杂谈

当时因为 COVID，我开始了解药物。在学习一些药理学、有机化学、生物化学、药物化学、循证医学之后，终于能对当初困惑的问题给出完整的解答。

### 关于新冠

在上一篇 blog 之后，出现了一些新的毒株。

#### 新毒株

> 毒株的代号可以在 [lineage_notes.txt](https://github.com/cov-lineages/pango-designation/blob/master/lineage_notes.txt) 中查看。
> 病毒学家在 [cov-lineages/pango-designation](https://github.com/cov-lineages/pango-designation) 仓库中讨论命名过程，[sars-cov-2-variants/lineage-proposals](https://github.com/sars-cov-2-variants/lineage-proposals) 似乎也用于讨论。

在上次中国 2022 年 12 月 BA.5.2, BF.7 流行之后，

> **BA** = B.1.1.529 = Omicron \
> **BF** = BA.5.2.1

2023 年 5 月 - 9 月，流行 XBB 和 EG.5 及其亚分支。

> BJ = BA.2.10.1 \
> BM = BA.2.75.3 \
> **EG** = XBB.1.9.2 \
> **XBB** = BJ.1, BM.1.1.1

后来，2023 年 8 月出现了 [BA.2.86](https://github.com/cov-lineages/pango-designation/issues/2183)，2024 年 3 月流行 JN.1，而 2024 年 8 月主要是 KP.2

> **JN** = BA.2.86.1 \
> **KP** = JN.1.11.1

<center>
<img src="fig/positive.png" alt="positive" style="zoom:50%;" />
</center>


<center>
<img src="fig/variant.png" alt="variant" style="zoom:50%;" />
</center>

> 参考：[全国新型冠状病毒感染疫情情况（2025年1月）](https://www.chinacdc.cn/jksj/xgbdyq/202502/t20250213_304257.html)

听说最近南非又出了一个 [BA.3.2](https://github.com/cov-lineages/pango-designation/issues/2909)，或许几个月后会流行。

#### 如何应对

新冠主要通过接触、飞沫、气溶胶传播。

个人可以采取的措施：戴口罩、洗手（尤其是高峰期），空气净化器、CR box。

我制作过 [CR box](https://zh.wikipedia.org/wiki/%E7%A7%91%E8%A5%BF-%E7%BD%97%E6%A3%AE%E5%A1%94%E5%B0%94%E7%9B%92%E5%AD%90)、CO2 浓度检测仪（下次可以写一个 blog）。 \
但学校人太多了，挺困难的，很多时候我也做不好。

#### 公共卫生

有时候感觉很无力。个人认为采取公共卫生措施预防呼吸道传染病，而不是交给临床医学来治病，才是（对个人）更健康、（对社会）更经济的选择。不过像 2022 年的严密封控也是不太能接受的。并且现在大部分国家都躺平了。

可能只能说有点过于现代化了，交通过于发达了，公共卫生没有跟上经济发展的步伐。

<br>

### 关于化学

再次遇见化学，

- 我想起了初中时烧白磷的化学实验（当然高中就没有了）；
- 想起了大约 10 年前看有机化学书，知道苯环上的定位基效应的那个夜晚；
- 想起高中时准备过三个星期的化学竞赛，当然结果是考不出来；
- 想起大一的时候搞不懂无机化学中的多原子分子轨道。

<br>

### 关于生物化学

感谢刘不言老师的生物化学。他是一名协和医学博士，后投身医学教育。在他的文字、视频中，有时会出现关于生死的讨论：

> 我说过去的一年，我有三位同学去世了。没准过几年，你听到我也会出什么事。——[医生或护士从医院辞职后，过得还好吗？](https://www.zhihu.com/question/37816500/answer/1357517678)

一语成谶，他预言了自己的死亡——2021 年在安徽因为车祸去世。默哀。

<br>

很多疾病和生物化学有关。比如

- 蚕豆病是因为磷酸戊糖途径中的 G6PD 酶不能正常工作，容易溶血。
- 镰刀型细胞贫血症是因为基因突变导致珠蛋白上有疏水基团，疏水基团相互之间结合。

一些生活中的现象也与生物化学有关，比如

- 酸奶是粘稠的，是因为酪蛋白的等电点是酸性的。
- 绿叶菜烧熟后会变成深绿色，和去镁叶绿素有关。

<br>

### 关于药物化学

很多时候，药物的分子结构决定了其药动、药效性质：

- 常见的抗生素头孢呋辛酯，做成酯能够提高脂溶性，更容易吸收。
- 鼻炎会用到的丙酸氟替卡松，经肝脏代谢后硫酯键断开，快速失活。
- 雷洛昔芬可以被用于治疗乳腺癌。它的侧链使得雌激素受体的共激活因子无法盖住，无法转录。

<br>

想必看完了本文，了解了生物化学、药物化学，应该能知道下图讲的是一个 Morphine 酱和受体 dock 的故事吧。

![Morphine-and-receptor](fig/Morphine-and-receptor.png)

> 插画：[morphine-chan is working](https://www.pixiv.net/artworks/126454072)
>
> [22] [Molecular recognition of morphine and fentanyl by the human μ-opioid receptor](https://www.cell.com/cell/fulltext/S0092-8674(22)01260-0)

<br>

## 时间

资料查阅：

- 生物化学：2024 年 8 月
- 循证医学：2024 年 11 月
- 药物化学：2024 年 12 月，2025 年 1 月

发布：2025 年 3 月
