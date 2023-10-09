---
title: G711编解码--C语言实现
date: 2023-10-08 19:00:09
tags:
    - ffmpeg
    - C
    - 音视频
description:
top_img:
cover: https://file.yoyolex.tk/2023/10/G711.png
---

# 前言
G.711是一种窄带音频编解码器，最初设计用于电话，可提供 64 kbit/s 的长途质量音频。它是音频编码的ITU-T标准（推荐），名为语音脉冲编码调制 (PCM)，于 1972 年发布并使用。

它使用两种不同的对数压扩算法之一：μ-law（主要在北美和日本使用）和A-law（在北美以外的大多数其他国家/地区使用）。每个压扩样本被量化为 8 位，从而产生 64 kbit/s 的比特率。
[1] [Wikipedia](https://en.wikipedia.org/wiki/G.711)

# 压缩原理
G711按采样点进行压缩，舍弃低位，高位分段，将16bit的一个采样点数据压缩为8bit，所以压缩比固定为 8/16 = 50%。G.711a对高13位进行压缩，主要运用于欧洲和世界其他地区；G.711u对高14位进行压缩，主要运用于北美和日本

# G.711a编码
1. 取符号位并取反得到$\overline s$
2. 获取强度位 eee
3. 获取高位样本位 wxyz
4. 组合为 $\overline {s}eeewxyz$ 
5. 将偶数位取反（减少连续的0或1，降低传输误码率）

|input sample |encode                   |decode       |
|-            |-                        |-            |
|s0000000wxyza| $\overline {s}000wxyz$  |s0000000wxyz1|
|s0000001wxyza| $\overline {s}001wxyz$  |s0000001wxyz1|
|s000001wxyzab| $\overline {s}010wxyz$  |s000001wxyz10|
|s00001wxyzabb| $\overline {s}011wxyz$  |s00001wxyz100|
|s0001wxyzabbb| $\overline {s}100wxyz$  |s0001wxyz1000|
|s001wxyzabbbb| $\overline {s}101wxyz$  |s001wxyz10000|
|s01wxyzabbbbb| $\overline {s}110wxyz$  |s01wxyz100000|
|s1wxyzabbbbbb| $\overline {s}111wxyz$  |s1wxyz1000000|

例子：
输入pcm = 1234 (0x04D2 二进制为0000 0100 1101 0010 --> 转换格式 0 00001 0011 010010)

1. 取符号位s=0
2. 强度位eee=011
3. 高样本位wxyz=0011
4. 组合为 $\overline {s}eeewxyz$ = 10110011
5. 偶数位取反11100110 (16进制0xE6)

# G.711u编码
1. 取符号位 s
2. 获取强度位 eee
3. 获取高位样本位 wxyz
4. 组合为 seeewxyz
5. 取反

|input sample  |encode                   |decode        |
|-             |-                        |-             |
|s00000001wxyzb| $\overline {s000wxyz}$  |s00000001wxyz1|
|s0000001wxyzbb| $\overline {s001wxyz}$  |s0000001wxyz10|
|s000001wxyzbbb| $\overline {s010wxyz}$  |s000001wxyz100|
|s00001wxyzbbbb| $\overline {s011wxyz}$  |s00001wxyz1000|
|s0001wxyzbbbbb| $\overline {s100wxyz}$  |s0001wxyz10000|
|s001wxyzbbbbbb| $\overline {s101wxyz}$  |s001wxyz100000|
|s01wxyzbbbbbbb| $\overline {s110wxyz}$  |s01wxyz1000000|
|s1wxyzbbbbbbbb| $\overline {s111wxyz}$  |s1wxyz10000000|

例子：
输入pcm = 1234 (二进制为0000 0100 1101 0010 --> 转换格式 0 00001 0011 010010)

1. 取符号位s=0
2. 强度位eee=011
3. 高样本位wxyz=0011
4. 组合为seeewxyz = 00110011
5. 偶数位取反11001100 (16进制0xCC)
