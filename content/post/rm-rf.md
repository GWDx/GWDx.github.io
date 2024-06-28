---
title: "rm -rf 的教训"
date: 2023-12-19
lastmod: 2023-12-19
keywords: ["linux", "文件系统"]
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
katex: false
---


11 月的时候，我在不清醒的情况下，不小心 `rm -rf` 了，ctrl+c 中断后，发现删掉了 300GB 的文件。

另外，`rsync --delete` 也是个危险的命令。


<!--more-->

## 起因

GeekGame 2023 中有一道名为[“未来磁盘”](https://github.com/PKU-GeekGame/geekgame-3rd/tree/master/official_writeup/prob21-gzip)的题目，需要解压 7TB 的文件。
有[题解](https://blog.taoky.moe/2023-10-22/geekgame-v3-wp-outline.html)说使用 btrfs 文件系统的透明压缩功能，可以解出“未来磁盘”一题的 flag。我试了一下，对于这题的文件，压缩率可以达到 4%。

我的电脑里面有很多文本文件很占空间，于是打算把文件系统从 ext4 换成 btrfs，但是直接用启动 U 盘安装 btrfs 的 debian 系统后无法正常开机（后来发现需要关闭 `/etc/fstab` 中的一些选项）。

我使用启动 U 盘，创建了 btrfs 的分区，在 `/media/` 下挂载两个文件系统。然后 `cp -rv /mnt/ext /mnt/btrfs`，发现这样不能复制隐藏文件，于是打算删除已经复制过去的文件，重新复制。但是没看清当前目录，直接 `rm -rf *`，当发现提示 `Device or resource busy` 时，已经删除了将近 300GB 的文件。

## 试图恢复

> 感谢 [RTXUX](https://github.com/RTXUX) 和 [taoky](https://github.com/taoky) 在数据恢复过程中提供的建议

[File recovery - ArchWiki](https://wiki.archlinux.org/title/file_recovery) 中提到了几种恢复文件的方法。
+ Ext4Magic 可以获取 ext4 文件系统的日志等信息
+ Photorec 可以通过读取磁盘的方式恢复文件

### 准备工作

使用 U 盘启动系统，以**只读**方式挂载磁盘

```bash
sudo mount -r /dev/nvme0n1p5 /mnt
```

制作磁盘镜像，以避免数据损坏

```bash
sudo dd if=/dev/nvme0n1p5 of=~/nvme0n1p5.img
```

debian 软件源中的 ext4magic 有 bug，需要打 patch。使用 [ext4magic-patch-extent-free](https://aur.archlinux.org/packages/ext4magic-patch-extent-free) 中的代码及 patch，然后编译

### 导出日志

> `debugfs` 是 linux 内核中的一个文件系统调试工具，可以用来查看 ext 文件系统的信息。默认是只读的，不会对文件系统产生影响

使用 debugfs 导出文件系统日志

```bash
sudo debugfs -R "dump <8> ~/nvme.journal" /dev/nvme0n1p5
```

> 其中 8 是文件系统日志所在的 inode

```bash
ext4magic /dev/nvme0n1p5 -a "$(date -d "-10hours" +%s)" -f deleted/folders/root -j nvme.journal -l
```

然而使用这个命令，只恢复了 22MB 的文件。

### 详细分析

查看分区的统计信息

```bash
debugfs nvme0n1p5.img
stats
stat <8>
```

得到文件系统的元信息


```
debugfs 1.47.0 (5-Feb-2023)
debugfs:  stats
Filesystem volume name:   Linux
Last mounted on:          /media/user/Linux
Filesystem UUID:          79965035-286b-4ffe-964a-c7adb579bae3
Filesystem magic number:  0xEF53
Filesystem revision #:    1 (dynamic)
Filesystem features:      has_journal ext_attr resize_inode dir_index filetype extent 64bit flex_bg sparse_super large_file huge_file dir_nlink extra_isize metadata_csum
Filesystem flags:         signed_directory_hash
Default mount options:    user_xattr acl
Filesystem state:         clean
Errors behavior:          Continue
Filesystem OS type:       Linux
Inode count:              38084608
Block count:              152308224
Reserved block count:     7611432
Overhead clusters:        2671494
Free blocks:              87617839
Free inodes:              35252014
First block:              0
Block size:               4096
Fragment size:            4096
Group descriptor size:    64
Reserved GDT blocks:      1024
Blocks per group:         32768
Fragments per group:      32768
Inodes per group:         8192
Inode blocks per group:   512
Flex block group size:    16
Filesystem created:       Wed Aug 24 00:21:21 2022
Last mount time:          Thu Nov 16 23:23:14 2023
Last write time:          Thu Nov 16 23:23:35 2023
Mount count:              7
Maximum mount count:      -1
Last checked:             Thu Nov 16 20:27:44 2023
Check interval:           0 (<none>)
Lifetime writes:          9 TB
Reserved blocks uid:      0 (user root)
Reserved blocks gid:      0 (group root)
First inode:              11
Inode size:               256
Required extra isize:     32
Desired extra isize:      32
Journal inode:            8
Default directory hash:   half_md4
Directory Hash Seed:      0f035771-06dc-43d4-a2f6-043003c31d06
Journal backup:           inode blocks
Checksum type:            crc32c
Checksum:                 0x86c5f28c
Directories:              312477
 Group  0: block bitmap at 1113, inode bitmap at 1129, inode table at 1145
           16543 free blocks, 269 free inodes, 911 used directories, 0 unused inodes
           [Checksum 0xf40f]
 Group  1: block bitmap at 1114, inode bitmap at 1130, inode table at 1657
           20108 free blocks, 31 free inodes, 92 used directories, 0 unused inodes
           [Checksum 0x5d6c]
```

得到日志信息

```
Inode: 8   Type: regular    Mode:  0600   Flags: 0x80000
Generation: 0    Version: 0x00000000:00000000
User:     0   Group:     0   Project:     0   Size: 1073741824
File ACL: 0
Links: 1   Blockcount: 2097160
Fragment:  Address: 0    Number: 0    Size: 0
 ctime: 0x6304fe84:00000000 -- Wed Aug 24 00:21:24 2022
 atime: 0x6304fe84:00000000 -- Wed Aug 24 00:21:24 2022
 mtime: 0x6304fe84:00000000 -- Wed Aug 24 00:21:24 2022
crtime: 0x6304fe84:00000000 -- Wed Aug 24 00:21:24 2022
Size of extra inode fields: 32
Inode checksum: 0x9617a9cc
EXTENTS:
(ETB0):91258879, (0-32767):91258880-91291647, (32768-65535):91291648-91324415, (65536-98303):91324416-91357183, (98304-131071):91357184-91389951, (131072-163839):91389952-91422719, (163840-196607):91422720-91455487, (196608-229375):91455488-91488255, (229376-262143):91488256-91521023
```

之前导出的日志 `nvme.journal` 的大小正是 1GB。


根据 [Ext4magic-Journal](https://ext4magic.sourceforge.net/journal_en.html)，ext4magic 可以导出所有 trasaction 的信息

```bash
ext4magic /dev/nvme0n1p5 -T -x > block-journal
```

```
Filesystem in use: /dev/nvme0n1p5

Using  internal Journal at Inode 8

Found 18493 copy of Filesystemblock in Journal
FS-Block	 Journal	Transact	Time in sec	Time of Transaction
           0	       2	20795182
    87032057	       5	20795183 	1700148205	Thu Nov 16 15:23:25 2023
   104858312	      41	20795045 	1700140243	Thu Nov 16 13:10:43 2023
  8589934592	      42	20795045
```

导出的内容中，每一列分别表示文件系统块、日志块、事务号、时间戳、时间。

下面是刚开始删除时的日志元信息：

```
    26214432	   55211	20795098 	1700141027	Thu Nov 16 13:23:47 2023
     3148592	   55719	20795098 	1700141027	Thu Nov 16 13:23:47 2023
     3148463	   55973	20795098 	1700141027	Thu Nov 16 13:23:47 2023
     3147398	   56227	20795098 	1700141027	Thu Nov 16 13:23:47 2023
     3146488	   56231	20795098 	1700141028	Thu Nov 16 13:23:48 2023
     3146325	   56481	20795098 	1700141028	Thu Nov 16 13:23:48 2023
     3146940	   56735	20795098 	1700141027	Thu Nov 16 13:23:47 2023
     3147016	   56989	20795098 	1700141027	Thu Nov 16 13:23:47 2023
```


以 3148592 为例，使用 `block_dump` 可以查看块中的内容


```bash
debugfs /dev/nvme0n1p5
block_dump 3148592
```

```
0000  a481 0000 0000 0000 d276 8363 e317 5665  .........v.c..Ve
0020  e317 5665 e317 5665 0000 0000 0000 0000  ..Ve..Ve........
0040  0000 0800 0400 0000 0af3 0000 0400 0000  ................
0060  0000 0000 0000 0000 0000 0000 0000 0000  ................
*
0140  0000 0000 63fa 2df1 0000 0000 0000 0000  ....c.-.........
0160  0000 0000 0000 0000 0000 0000 14c2 0000  ................
0200  2000 db32 5c0d cb7f 5c0d cb7f 0000 0000   ..2\...\.......
0220  d276 8363 dc20 f5b8 0000 0000 0000 0000  .v.c. ..........
0240  0000 0000 0000 0000 0000 0000 0000 0000  ................
*
0400  a481 0000 0000 0000 d276 8363 e317 5665  .........v.c..Ve
0420  e317 5665 e317 5665 0000 0000 0000 0000  ..Ve..Ve........
0440  0000 0800 0400 0000 0af3 0000 0400 0000  ................
0460  0000 0000 0000 0000 0000 0000 0000 0000  ................
```


以 4kB 为单位，每一行表示一个块，每一行的前 4 个字节表示块的状态，后面的内容是块的内容。

而日志存储在 nvme.journal 的 55719 的日志块中

```bash
split -d -a 6 -b 4K nvme0n1p5.journal
hexdump -C x055719
```

```
00000000  a4 81 00 00 00 00 00 00  d2 76 83 63 e3 17 56 65  |.........v.c..Ve|
00000010  e3 17 56 65 e3 17 56 65  00 00 00 00 00 00 00 00  |..Ve..Ve........|
00000020  00 00 08 00 04 00 00 00  0a f3 00 00 04 00 00 00  |................|
00000030  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000060  00 00 00 00 63 fa 2d f1  00 00 00 00 00 00 00 00  |....c.-.........|
00000070  00 00 00 00 00 00 00 00  00 00 00 00 14 c2 00 00  |................|
00000080  20 00 db 32 5c 0d cb 7f  5c 0d cb 7f 00 00 00 00  | ..2\...\.......|
00000090  d2 76 83 63 dc 20 f5 b8  00 00 00 00 00 00 00 00  |.v.c. ..........|
000000a0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000100  a4 81 00 00 00 00 00 00  d2 76 83 63 e3 17 56 65  |.........v.c..Ve|
00000110  e3 17 56 65 e3 17 56 65  00 00 00 00 00 00 00 00  |..Ve..Ve........|
00000120  00 00 08 00 04 00 00 00  0a f3 00 00 04 00 00 00  |................|
00000130  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
```

日志块中的文件与磁盘块中的文件内容一致。

查阅资料，发现 ext4 文件系统使用的是 **REDO** 日志，即日志中记录的是修改后的块的内容，而不是修改前的内容。这意味着在 commit 之后，几乎不可能恢复出 inode 的原始内容。

> 在删除操作后还有一些日志，可能是当时没没有读挂载，导致文件访问时间被修改了。不过即使没有这些写入，也很难恢复出原始的目录结构。

### 通过读取磁盘恢复一些文件


Photorec 是通过读取磁盘的方式恢复文件的


```bash
sudo photorec /dev/nvme0n1p5
```

ext4magic 也支持这种方式。它将不同类型的文件分开放了，感觉更好用一些

```bash
ext4magic /dev/nvme0n1p5 -f / -r
```

不过也只能扫描出一部分文件，而且文件名都是未知的。

### 丢失的文件

我平时使用 [FSearch](https://github.com/cboxdoerfer/fsearch) 来索引文件，恰好 `~/.local/share/fsearch/fsearch.db` 没有被删掉，可以通过它来查看丢失了哪些文件。

丢失的大部分是数据集、文本文件。不过还是丢了一些代码。

## 其他可能丢失文件的操作

> 这些都是亲身经历

### 图形界面的删除

shift+delete 删除文件时，确认得太快。后来意识到可能还有用

### 命令行下 `rsync --delete`

#### 情况一：没有检查参数

使用 zsh 的 auto complete 功能，可以快速补全命令，但是如果有 `--delete` 参数，没仔细看直接 ctrl+f 补全完 enter，那么可能损失一些文件。

> 平时也要注意一些便捷的功能可能导致误执行命令

#### 情况二：目录后面加了 `/`

你能分清楚这两条命令的区别吗？如果不能，就不要用 `--delete` 了。

```
rsync --recursive A/ B
rsync --recursive A B
```

答案：

+ `A/ B/` 和 `A/ B`：将 `A` 内部的内容同步到 `B`（如果不存在会创建文件夹）
+ `A B/` 和 `A B`：将整个 A 目录复制到 `B` 内，形成 `B/A`

`A` 后面加不加 `/` 会影响 `rsync` 的行为，如果不搞清楚这一点，并且选项中含有 `--delete`，就可能错误地删除文件。

## 教训

### 1. 不要在不清醒的情况下操作

### 2. 做好备份

### 3. 通过设置避免误删文件

#### 设置 `rm`

在 `~/.zshrc` 中设置禁用 `rm`，使用 `trash` 代替，帮助戒掉 `rm` 的习惯

```bash
alias sudo='sudo '
alias rm='echo "rm is disabled, use trash instead"'
```

> 其中，设置 `alias sudo='sudo '` 是为了让下一个单词也被 alias，否则 `sudo rm` 的 `rm` 不会触发 alias。

也要避免 `su` 模式下使用这些命令。

```bash
sudo ln -s ~/.zshrc /root/.zshrc
```

#### 设置 `rsync`

设置 `rsync`，禁用 `--delete` 参数

```bash
rsync(){
    # check if --delete is in the arguments
    if [[ "$@" == *--del* ]]; then
        echo "rsync --delete is not allowed"
        return 1
    fi
    # else run rsync
    /bin/rsync "$@"
}
```

另外，为了让 `su` 切换到 `root` 用户时也禁用 `rm`，设置 `/root/.zshrc` 文件：

```
sudo ln -s ~/.zshrc /root/.zshrc
```

#### 设置图形界面

设置 shift+delete 删除文件时，要求确认

### 4. 如果误删了一个文件

> 对于 `ext4` 文件系统

立刻停止文件操作，但不要 `umount` 磁盘。

用 `df -Th` 查看文件系统，使用 `debugfs` 查看磁盘信息

```
sudo debugfs /dev/nvme0n1p5
```

查看是否有还未写入日志的记录

```
lsdel
```

使用 `undel` 恢复文件

但如果已经写入日志，那就只能通过读取磁盘的方式找文件了（注意这种情况下最好 `umount` 磁盘，改成只读挂载，避免写入）



## 结论

+ `rm -r` 和 `rsync --delete` 是危险的命令，谨慎使用
+ ext4 在 commit 后，几乎不可能恢复出 inode 的原始内容
