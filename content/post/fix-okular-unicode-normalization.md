---
title: "修复 Okular 的中文标点复制问题"
date: 2024-05-28
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
mathjax: false
---

修复 PDF 阅读器 Okular 久远的 bug——中文标点复制出来是英文标点的问题。

<!--more-->

## 问题描述

Okular 是 KDE 桌面环境自带的 PDF 查看器。

Okular 有一个久远的 bug，复制中文标点时会变成英文标点。印象中 2021 年上半年我还在用 Windows 的时候就存在这个问题。

前段时间忽然想到了这个问题，于是去 debug 了一下。

环境：
- Debian 12
- Okular 22.12.3

## poppler？

找到一个相关的 issue：[evince issue 384](https://gitlab.gnome.org/GNOME/evince/-/issues/384)，后续认为是 poppler 的问题 [poppler issue 582](https://gitlab.freedesktop.org/poppler/poppler/-/issues/582)。

Poppler 是一个 PDF 渲染库，是 Okular 的后端。Debian 下 Okular 软件包依赖的库是 `libpoppler-qt5-1`

### 编译

因为每次都输 CMake 项目的编译命令太麻烦了，所以我在 `~/.zshrc` 中写了一个函数

```bash
cmakemake() {
    [[ $PWD == *build ]] && cd ..
    [ -d build ] && /bin/rm -r build
    ([ -e CMakeLists.txt ] && mkdir build && cd build && cmake $@ .. && make -j12 && cd ..)
}
```

先把 poppler `reset` 到当前版本，然后以 Debug 模式**编译**

```bash
git clone https://gitlab.freedesktop.org/poppler/poppler.git
git reset --hard poppler-22.12.0
cmakemake -DCMAKE_CXX_FLAGS="-g -O0" -DCMAKE_C_FLAGS="-g -O0"
```

> 可以用 `nm` 确认编译出的文件有调试符号

然后设置环境变量 `LD_LIBRARY_PATH`，要求系统优先加载编译好的库

```bash
export LD_LIBRARY_PATH=$PWD/build/qt5/src:$LD_LIBRARY_PATH
```


也可以暴力一些，替换掉系统的库

```bash
cd /usr/lib/x86_64-linux-gnu
sudo /bin/rm libpoppler-qt5.so.1
sudo ln -s /home/gwd/文档/GitHub/okular-copy/poppler/build/qt5/src/libpoppler-qt5.so.1.31.0 libpoppler-qt5.so.1
ll /lib/x86_64-linux-gnu/libpoppler*
```

### 调试

先在这个库上停下来

```
r
rbreak ^Poppler::Page::
```

最先执行的是 `Poppler::Page::Page`

`bt` 查看调用栈，Poppler 库中最先执行的是 `Poppler::Document::page`

```
#0  0x00007fffb81b82b0 in Poppler::Page::Page(Poppler::DocumentData*, int)@plt () from /lib/x86_64-linux-gnu/libpoppler-qt5.so.1
#1  0x00007fffb81d2e14 in Poppler::Document::page(int) const () from /lib/x86_64-linux-gnu/libpoppler-qt5.so.1
#2  0x00007fffb8238b8b in PDFGenerator::loadPages (this=0x555555b8d080, pagesVector=..., rotation=0, clear=false) at /home/gwd/文档/GitHub/okular-copy/okular/generators/poppler/generator_pdf.cpp:784
```

Okular 调用 Poppler 中的 `Poppler::Document::page`，然后依次调用 `Poppler::Page::Page`、`Poppler::Document::page`、`Catalog::getPage`、`Catalog::cachePageTree`、`Page::Page`……

<!-- <details>
<summary>调用栈</summary>

```
#0  0x00007fff932ef2d0 in Page::Page(PDFDoc*, int, Object&&, Ref, PageAttrs*, Form*)@plt ()
   from /home/gwd/文档/GitHub/okular-copy/poppler/build/libpoppler.so.126
#1  0x00007fff9335e946 in std::make_unique<Page, PDFDoc*&, unsigned long, Object, Ref, PageAttrs*&, Form*&> ()
    at /usr/include/c++/10/bits/unique_ptr.h:962
#2  0x00007fff9335991c in Catalog::cachePageTree (this=0x555555b0f090, page=1)
    at /home/gwd/文档/GitHub/okular-copy/poppler/poppler/Catalog.cc:317
#3  0x00007fff93358fb7 in Catalog::getPage (this=0x555555b0f090, i=1)
    at /home/gwd/文档/GitHub/okular-copy/poppler/poppler/Catalog.cc:200
#4  0x00007fff9343a1dc in PDFDoc::getPage (this=0x55555572a6a0, page=1)
    at /home/gwd/文档/GitHub/okular-copy/poppler/poppler/PDFDoc.cc:2145
#5  0x00007fffabf4add9 in Poppler::Page::Page (this=0x555555aaf2f0, doc=0x555555b02bf0, index=0)
    at /home/gwd/文档/GitHub/okular-copy/poppler/qt5/src/poppler-page.cc:422
#6  0x00007fffabf29474 in Poppler::Document::page (this=0x555555c4adf0, index=0)
    at /home/gwd/文档/GitHub/okular-copy/poppler/qt5/src/poppler-document.cc:118
#7  0x00007fffc0038b8b in PDFGenerator::loadPages (this=0x555555c329c0, pagesVector=..., rotation=0, clear=false)
    at /home/gwd/文档/GitHub/okular-copy/okular/generators/poppler/generator_pdf.cpp:784
#8  0x00007fffc0038734 in PDFGenerator::init (this=0x555555c329c0, pagesVector=..., password=...)
    at /home/gwd/文档/GitHub/okular-copy/okular/generators/poppler/generator_pdf.cpp:715
#9  0x00007fffc003830a in PDFGenerator::loadDocumentWithPassword (this=0x555555c329c0, filePath=..., pagesVector=..., password=...)
    at /home/gwd/文档/GitHub/okular-copy/okular/generators/poppler/generator_pdf.cpp:657
#10 0x00007fffea32edd0 in Okular::DocumentPrivate::openDocumentInternal (this=0x555555859ed0, offer=..., isstdin=isstdin@entry=false,
    docFile=..., filedata=..., password=...) at ./core/document.cpp:937
#11 0x00007fffea32f470 in Okular::Document::openDocument (this=this@entry=0x555555859e50, docFile=..., url=..., _mime=..., password=...)
    at ./core/document.cpp:2452
#12 0x00007fffe86dfc72 in Okular::Part::doOpenFile (this=this@entry=0x555555857090, mimeA=..., fileNameToOpenA=...,
    isCompressedFile=isCompressedFile@entry=0x7fffffffcda7) at ./part/part.cpp:1471
#13 0x00007fffe86e0a34 in Okular::Part::openFile (this=0x555555857090) at ./part/part.cpp:1605
#14 0x00007ffff7f47785 in ?? () from /lib/x86_64-linux-gnu/libKF5Parts.so.5
#15 0x00007ffff7f4887e in KParts::ReadOnlyPart::openUrl(QUrl const&) () from /lib/x86_64-linux-gnu/libKF5Parts.so.5
#16 0x00007fffe86d023b in Okular::Part::openUrl (this=<optimized out>, _url=..., swapInsteadOfOpening=<optimized out>)
    at ./part/part.cpp:1831
#17 0x000055555557017e in Shell::openUrl (this=<optimized out>, url=..., serializedOptions=...) at ./shell/shell.cpp:321
#18 0x000055555557256d in operator() (__closure=<optimized out>, url=...) at ./shell/shell.cpp:106
#19 QtPrivate::FunctorCall<QtPrivate::IndexesList<0>, QtPrivate::List<const QUrl&>, void, Shell::Shell(const QString&)::<lambda(const QUrl&)> >::call (arg=<optimized out>, f=...) at /usr/include/x86_64-linux-gnu/qt5/QtCore/qobjectdefs_impl.h:146
#20 QtPrivate::Functor<Shell::Shell(const QString&)::<lambda(const QUrl&)>, 1>::call<QtPrivate::List<QUrl const&>, void> (
    arg=<optimized out>, f=...) at /usr/include/x86_64-linux-gnu/qt5/QtCore/qobjectdefs_impl.h:256
#21 QtPrivate::QFunctorSlotObject<Shell::Shell(const QString&)::<lambda(const QUrl&)>, 1, QtPrivate::List<const QUrl&>, void>::impl(int, QtPrivate::QSlotObjectBase *, QObject *, void **, bool *) (which=<optimized out>, this_=<optimized out>, r=<optimized out>,
    a=<optimized out>, ret=<optimized out>) at /usr/include/x86_64-linux-gnu/qt5/QtCore/qobjectdefs_impl.h:443
#22 0x00007ffff64e8f8f in ?? () from /lib/x86_64-linux-gnu/libQt5Core.so.5
#23 0x0000555555567445 in WelcomeScreen::recentItemClicked (this=<optimized out>, _t1=...)
    at ./obj-x86_64-linux-gnu/shell/okular_autogen/EWIEGA46WW/moc_welcomescreen.cpp:207
#24 0x00007ffff64e8f8f in ?? () from /lib/x86_64-linux-gnu/libQt5Core.so.5
#25 0x00007ffff73cee15 in QAbstractItemView::activated(QModelIndex const&) () from /lib/x86_64-linux-gnu/libQt5Widgets.so.5
#26 0x00007ffff73d2e55 in QAbstractItemView::mouseReleaseEvent(QMouseEvent*) () from /lib/x86_64-linux-gnu/libQt5Widgets.so.5
#27 0x00007ffff741a84f in QListView::mouseReleaseEvent(QMouseEvent*) () from /lib/x86_64-linux-gnu/libQt5Widgets.so.5
#28 0x00007ffff71a4db8 in QWidget::event(QEvent*) () from /lib/x86_64-linux-gnu/libQt5Widgets.so.5
#29 0x00007ffff725048e in QFrame::event(QEvent*) () from /lib/x86_64-linux-gnu/libQt5Widgets.so.5
#30 0x00007ffff64b14bb in QCoreApplicationPrivate::sendThroughObjectEventFilters(QObject*, QEvent*) ()
   from /lib/x86_64-linux-gnu/libQt5Core.so.5
#31 0x00007ffff7162f9e in QApplicationPrivate::notify_helper(QObject*, QEvent*) () from /lib/x86_64-linux-gnu/libQt5Widgets.so.5
#32 0x00007ffff716b552 in QApplication::notify(QObject*, QEvent*) () from /lib/x86_64-linux-gnu/libQt5Widgets.so.5
#33 0x00007ffff64b1738 in QCoreApplication::notifyInternal2(QObject*, QEvent*) () from /lib/x86_64-linux-gnu/libQt5Core.so.5
#34 0x00007ffff716965e in QApplicationPrivate::sendMouseEvent(QWidget*, QMouseEvent*, QWidget*, QWidget*, QWidget**, QPointer<QWidget>&, bool, bool) () from /lib/x86_64-linux-gnu/libQt5Widgets.so.5
#35 0x00007ffff71bdbd8 in ?? () from /lib/x86_64-linux-gnu/libQt5Widgets.so.5
#36 0x00007ffff71c0f60 in ?? () from /lib/x86_64-linux-gnu/libQt5Widgets.so.5
#37 0x00007ffff7162fae in QApplicationPrivate::notify_helper(QObject*, QEvent*) () from /lib/x86_64-linux-gnu/libQt5Widgets.so.5
#38 0x00007ffff64b1738 in QCoreApplication::notifyInternal2(QObject*, QEvent*) () from /lib/x86_64-linux-gnu/libQt5Core.so.5
#39 0x00007ffff693d42d in QGuiApplicationPrivate::processMouseEvent(QWindowSystemInterfacePrivate::MouseEvent*) ()
   from /lib/x86_64-linux-gnu/libQt5Gui.so.5
#40 0x00007ffff6911cec in QWindowSystemInterface::sendWindowSystemEvents(QFlags<QEventLoop::ProcessEventsFlag>) ()
   from /lib/x86_64-linux-gnu/libQt5Gui.so.5
#41 0x00007ffff12fbeca in ?? () from /lib/x86_64-linux-gnu/libQt5XcbQpa.so.5
#42 0x00007ffff4a627a9 in g_main_context_dispatch () from /lib/x86_64-linux-gnu/libglib-2.0.so.0
#43 0x00007ffff4a62a38 in ?? () from /lib/x86_64-linux-gnu/libglib-2.0.so.0
#44 0x00007ffff4a62acc in g_main_context_iteration () from /lib/x86_64-linux-gnu/libglib-2.0.so.0
#45 0x00007ffff6509876 in QEventDispatcherGlib::processEvents(QFlags<QEventLoop::ProcessEventsFlag>) ()
   from /lib/x86_64-linux-gnu/libQt5Core.so.5
#46 0x00007ffff64b01bb in QEventLoop::exec(QFlags<QEventLoop::ProcessEventsFlag>) () from /lib/x86_64-linux-gnu/libQt5Core.so.5
#47 0x00007ffff64b8316 in QCoreApplication::exec() () from /lib/x86_64-linux-gnu/libQt5Core.so.5
#48 0x0000555555566215 in main (argc=<optimized out>, argc@entry=1, argv=argv@entry=0x7fffffffe218) at ./shell/main.cpp:111
#49 0x00007ffff604624a in __libc_start_call_main (main=main@entry=0x555555565a50 <main(int, char**)>, argc=argc@entry=1,
    argv=argv@entry=0x7fffffffe218) at ../sysdeps/nptl/libc_start_call_main.h:58
#50 0x00007ffff6046305 in __libc_start_main_impl (main=0x555555565a50 <main(int, char**)>, argc=1, argv=0x7fffffffe218,
    init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7fffffffe208) at ../csu/libc-start.c:360
#51 0x00005555555671f1 in _start ()
```

</details> -->

这样查下去没完没了了，而且这里好像只是初始化。

偶然间发现一个有趣的函数 `TextBox::text()`。

```cpp
QString TextBox::text() const
{
+   printf("TextBox::text()   m_data->text: %s\n", m_data->text.toStdString().c_str());
    return m_data->text;
}
```

添加 `printf` 并编译。打开有中文标点的 PDF，使这个函数执行。

输出的是中文标点，说明 Poppler 读取文件是没问题的，那只能是在 Okular 复制的时候出了问题。


```
#0  Poppler::TextBox::text (this=0x7fff840134c0) at /home/gwd/文档/GitHub/okular-copy/poppler/qt5/src/poppler-textbox.cc:40
#1  0x00007fffb823dcf2 in PDFGenerator::abstractTextPage (this=0x555555ca6570, text=..., height=841.88976377952804,
    width=595.30393700787408, rot=0) at /home/gwd/文档/GitHub/okular-copy/okular/generators/poppler/generator_pdf.cpp:1719
#2  0x00007fffb823b729 in PDFGenerator::textPage (this=0x555555ca6570, request=0x555555ae61f0)
    at /home/gwd/文档/GitHub/okular-copy/okular/generators/poppler/generator_pdf.cpp:1352
#3  0x00007fffea341405 in Okular::TextPageGenerationThread::run (this=0x555555ae61d0) at ./core/generator_p.cpp:127
#4  0x00007ffff62cbd83 in ?? () from /lib/x86_64-linux-gnu/libQt5Core.so.5
#5  0x00007ffff60a8134 in start_thread (arg=<optimized out>) at ./nptl/pthread_create.c:442
#6  0x00007ffff61287dc in clone3 () at ../sysdeps/unix/sysv/linux/x86_64/clone3.S:81
```

## okular 中的问题

编译 Okular，设置环境变量

```bash
sudo apt build-dep okular
git clone https://invent.kde.org/graphics/okular.git
git reset --hard v22.12.3
cmakemake -DCMAKE_BUILD_TYPE=DEBUGFULL
```

```bash
export LD_LIBRARY_PATH=$PWD/build/bin:$LD_LIBRARY_PATH
```

之前的 `TextBox::text` 函数是在 `PDFGenerator::abstractTextPage` 函数中调用的。

在该函数中添加输出语句，查看各个变量的值。

```cpp
inline void append(Okular::TextPage *ktp, const QString &s, double l, double b, double r, double t)
{
+   qWarning() << "text: " << s << " at (" << l << "," << t << ")x(" << r << "," << b << ")";
    ktp->append(s, new Okular::NormalizedRect(l, t, r, b));
}

Okular::TextPage *PDFGenerator::abstractTextPage(const QList<Poppler::TextBox *> &text, double height, double width, int rot)
{
    Q_UNUSED(rot);
    Okular::TextPage *ktp = new Okular::TextPage;
    Poppler::TextBox *next;
#ifdef PDFGENERATOR_DEBUG
    qCDebug(OkularPdfDebug) << "getting text page in generator pdf - rotation:" << rot;
#endif
    QString s;
    bool addChar;
    for (const Poppler::TextBox *word : text) {
        const int qstringCharCount = word->text().length();
+       printf("qstringCharCount: %d\n", qstringCharCount);
        next = word->nextWord();
        int textBoxChar = 0;
        for (int j = 0; j < qstringCharCount; j++) {
            const QChar c = word->text().at(j);
+           QString str = QString(c);
+           QByteArray ba = str.toUtf8();       // 将 QChar 转换为 UTF-8 编码的 QByteArray
+           const char *c_str = ba.constData(); // 获取指向 QByteArray 数据的指针
+           printf("char: %s\n", c_str);        // 使用 printf 打印
+           printf("code: %x\n", c.unicode());  // 使用 printf 打印

            if (c.isHighSurrogate()) {
                s = c;
                addChar = false;
            } else if (c.isLowSurrogate()) {
                s += c;
                addChar = true;
            } else {
                s = c;
                addChar = true;
            }

            if (addChar) {
                QRectF charBBox = word->charBoundingBox(textBoxChar);
                append(ktp, (j == qstringCharCount - 1 && !next) ? (s + QLatin1Char('\n')) : s, charBBox.left() / width, charBBox.bottom() / height, charBBox.right() / width, charBBox.top() / height);
                textBoxChar++;
            }
        }

        if (word->hasSpaceAfter() && next) {
            // TODO Check with a document with vertical text
            // probably won't work and we will need to do comparisons
            // between wordBBox and nextWordBBox to see if they are
            // vertically or horizontally aligned
            QRectF wordBBox = word->boundingBox();
            QRectF nextWordBBox = next->boundingBox();
            append(ktp, QStringLiteral(" "), wordBBox.right() / width, wordBBox.bottom() / height, nextWordBBox.left() / width, wordBBox.top() / height);
        }
    }
+   std::cout << "ktp->text(): " << ktp->text().toUtf8().constData() << std::endl;
    return ktp;
}
```


`PDFGenerator::abstractTextPage` 函数读出的是 `0xff0c`，是正确的。但是最后的 `ktp` 不对，说明是 `ktp->append` 有问题。

```
TextBox::text()   m_data->text: 这是一个简单的句子，包含了一个逗号

char: ，
code: ff0c
text:  "，"  at ( 0.274493 , 0.063232 )x( 0.290889 , 0.0933557 )

ktp->text(): 这是一个简单的句子,包含了一个逗号
```

问题出现在 `TextPage::append` 函数中，在将新的文本和上一个文本合并的时候，使用了 NFKC 标准化。

```cpp
void TextPage::append(const QString &text, NormalizedRect *area)
{
    if (!text.isEmpty()) {
        if (!d->m_words.isEmpty()) {
            TinyTextEntity *lastEntity = d->m_words.last();
            const QString concatText = lastEntity->text() + text.normalized(QString::NormalizationForm_KC);
            if (concatText != concatText.normalized(QString::NormalizationForm_KC)) {
                // If this happens it means that the new text + old one have combined, for example A and ◌̊  form Å
                NormalizedRect newArea = *area | lastEntity->area;
                delete area;
                delete lastEntity;
                d->m_words.removeLast();
                d->m_words.append(new TinyTextEntity(concatText.normalized(QString::NormalizationForm_KC), newArea));
                return;
            }
        }

        d->m_words.append(new TinyTextEntity(text.normalized(QString::NormalizationForm_KC), *area));
    }
    delete area;
}
```



## Unicode 标准化

[Unicode 规范](https://unicode.org/reports/tr15/) 定义了四种标准化形式：

| 标准化形式 | 描述                 |
| ---------- | -------------------- |
| NFD        | 规范分解             |
| NFC        | 规范分解，再规范组合 |
| NFKD       | 兼容分解             |
| NFKC       | 兼容分解，再兼容组合 |

相比于 NFC，NFKC 更激进，会将一些特殊字符转换为普通字符：

```python
import unicodedata

s = '：，！⑥'

nfc = unicodedata.normalize('NFC', s)
nfd = unicodedata.normalize('NFD', s)
nfkc = unicodedata.normalize('NFKC', s)
nfkd = unicodedata.normalize('NFKD', s)

print(f"origin:\t\t{s}\t\t{s.encode('utf-8')}")
print(f"NFC:\t\t{nfc}\t\t{nfc.encode('utf-8')}")
print(f"NFD:\t\t{nfd}\t\t{nfd.encode('utf-8')}")
print(f"NFKC:\t\t{nfkc}\t\t{nfkc.encode('utf-8')}")
print(f"NFKD:\t\t{nfkd}\t\t{nfkd.encode('utf-8')}")
```

```
origin:         ：，！⑥         b'\xef\xbc\x9a\xef\xbc\x8c\xef\xbc\x81\xe2\x91\xa5'
NFC:            ：，！⑥         b'\xef\xbc\x9a\xef\xbc\x8c\xef\xbc\x81\xe2\x91\xa5'
NFD:            ：，！⑥         b'\xef\xbc\x9a\xef\xbc\x8c\xef\xbc\x81\xe2\x91\xa5'
NFKC:           :,!6            b':,!6'
NFKD:           :,!6            b':,!6'
```

使用 NFKC 标准化后，中文标点变成了英文标点。

看了一下 commit 记录，这个 bug 好像有十年的历史了。

> Gnome 的 evince 阅读器也因为标准化出过问题 [issue 1085](https://gitlab.gnome.org/GNOME/evince/-/issues/1085)

## 解决方案

Okular 里共使用了 10 次 NFKC：

| 函数                                                        | NFKC 出现次数 | 函数功能                           |
| ----------------------------------------------------------- | ------------- | ---------------------------------- |
| `TextPage::append(text, area)`                              | 4             | 将新的文本和上一个文本合并         |
| `TextPagePrivate::findTextInternalForward`                  | 1             | 查找文本                           |
| `TextPagePrivate::findTextInternalBackward`                 | 1             | 反向查找文本                       |
| `makeWordFromCharacters(characters, pageWidth, pageHeight)` | 3             | 将一系列 `TinyTextEntity` 组成单词 |
| `PDFGenerator::exportTo(fileName, format)`                  | 1             | 将 PDF 导出为文本                  |

### 全部替换为 NFC

一种简单的修复方案是把所有的 `QString::NormalizationForm_KC` 替换为 `QString::NormalizationForm_C`。


于是找了两个相关的 issue：[bug 466521](https://bugs.kde.org/show_bug.cgi?id=466521) 和 [bug 473495](https://bugs.kde.org/show_bug.cgi?id=473495)。提交了一个 [Merge Request 941](https://invent.kde.org/graphics/okular/-/merge_requests/941)

### 查找时使用 NFKC

但是这种简单的改法不能查找到变音符号，例如用 `ff` 查找 `ﬀ`。

所以需要动动脑子，在查找函数中相比于之前的版本做两处修改：

- 用 NFKC 处理查找的 `query`
- 因为前面的修改使得待查找的文本 `str` 以 NFC 的形式存储，在查找前需要经过一个 NFKC

```cpp
RegularAreaRect *TextPagePrivate::findTextInternalForward(int searchID, const QString &_query, TextComparisonFunction comparer, TextEntity::List::ConstIterator start, int start_offset, TextEntity::List::ConstIterator end)
{
    // normalize query search all unicode (including glyphs)
-   const QString query = _query.normalized(QString::NormalizationForm_C);
+   const QString query = _query.normalized(QString::NormalizationForm_KC);

    // j is the current position in our query
    // queryLeft is the length of the query we have left to match
    int j = 0, queryLeft = query.length();

    TextEntity::List::ConstIterator it = start;
    int offset = start_offset;

    TextEntity::List::ConstIterator it_begin = TextEntity::List::ConstIterator();
    int offset_begin = 0; // dummy initial value to suppress compiler warnings

    while (it != end) {
        const TextEntity &curEntity = *it;
-       const QString &str = curEntity.text();
+       const QString &str = curEntity.text().normalized(QString::NormalizationForm_KC);
        const int strLen = str.length();
        const int adjustedLen = stringLengthAdaptedWithHyphen(str, it, m_words.constEnd());
```

> 这样修改似乎并不影响性能。

最终，这个 MR 在三个月后被合并到 master 分支。

## 后记

这是我第一次代码级别的 Pull Request。感谢 [taoky](https://github.com/taoky) 提供的修改建议。

感觉现在 KDE 还是有些问题，就比如鼠标拖拽窗口偶尔会卡死，或者切换窗口显示黑屏。不过这些 bug 不是每次都出现，比较难定位。
相比而言，这种能稳定复现的 bug 还是比较容易解决的。
