﻿ VasDolly 命令行
 java -jar vasdolly.jar help

 目前支持以下操作：
 ### 获取指定APK的签名方式
 java -jar vasdolly.jar get -s /home/user/test.apk

 ### 获取指定APK的渠道信息
 java -jar vasdolly.jar get -c /home/user/test.apk

 ### 删除指定APK的渠道信息
 java -jar vasdolly.jar remove -c /home/user/test.apk

 ### 通过指定渠道字符串添加渠道信息
 java -jar vasdolly.jar put -c "channel1,channel2" /home/user/base.apk /home/user/

 ### 通过指定渠道文件添加渠道信息
 java -jar vasdolly.jar put -c channel.txt /home/user/base.apk /home/user/

 ### 为基于V1的多渠道打包添加了多线程支持，满足渠道较多的使用场景
 java -jar vasdolly.jar put -mtc channel.txt /home/user/base.apk /home/user/

 ### 提供了FastMode，生成渠道包时不进行强校验，速度可提升10倍以上
 java -jar vasdolly.jar put -c channel.txt -f /home/user/base.apk /home/user/

 程序中获取 channel (使用 vasdolly-channel.jar 文件)
 String channel = ChannelReaderUtil.getChannel(context);