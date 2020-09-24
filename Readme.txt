## GIF lib
src : http://sourceforge.net/projects/giflib/
doc : http://giflib.sourceforge.net/gif_lib.html

## regex
doc : http://pubs.opengroup.org/onlinepubs/000095399/functions/regcomp.html

## zxing
git : https://github.com/zxing/zxing.git

## vasdolly
git : https://github.com/Tencent/VasDolly.git

## GitHub 文件加速
git clone https://g.ioiox.com/项目 git 地址
如：git clone https://g.ioiox.com/https://github.com/garfieldyf/AndroidTest.git

## bookmarks
http://androidxref.com/
http://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/jniTOC.html
http://www.bejson.com/
https://www.kernel.org/doc/man-pages/
http://ant.apache.org/manual/index.html
http://www.cplusplus.com/reference/
https://developer.android.google.cn/index.html
https://www.androidos.net.cn/sourcecode
http://tool.oschina.net/apidocs/apidoc?api=jdk-zh
https://www.cs.usfca.edu/~galles/visualization/Algorithms.html

## android resource
https://lanhuapp.com

## android Tools
urls : https://developer.android.google.cn/
       https://www.androidos.net.cn/sourcecode
       http://androidxref.com/
       http://www.bejson.com/
       https://www.kernel.org/doc/man-pages/
       http://ant.apache.org/manual/index.html
       https://www.cs.usfca.edu/~galles/visualization/Algorithms.html

ps -p -t|grep u0_a60
adb -s 172.17.5.46:5555 shell
adb tcpip 5555
adb shell dumpsys meminfo -a com.tencent.test
adb shell pm clear com.tencent.test
adb shell am force-stop com.tencent.test
adb shell remount
adb shell pm list packages com.tencent.test
adb shell dumpsys package com.tencent.test
mount -o remount /system
mount -o rw,remount /system
D:\Google\android-sdk-windows\build-tools\25.0.3\aapt dump badging E:\TVMaster\TVMaster-2.0.0.0-for-dangbei.apk
D:\Google\android-sdk-windows\build-tools\25.0.3\aapt dump xmltree E:\TVMaster\TVMaster-2.0.0.0-for-dangbei.apk AndroidManifest.xml
D:\Google\android-sdk-windows\platform-tools\hprof-conv E:\hprof\com.tencent.test.hprof E:\hprof\com.tencent.test.android.hprof

## build keystore
openssl pkcs8 -in platform.pk8 -inform DER -outform PEM -out platform.priv.pem -nocrypt
openssl pkcs12 -export -in platform.x509.pem -inkey platform.priv.pem -out platform.pk12 -name androiddebugkey
password: android
/D/Java/jdk1.8.0_51/bin/keytool -importkeystore -deststorepass android -destkeypass android -destkeystore debug.keystore -srckeystore platform.pk12 -srcstoretype PKCS12 -srcstorepass android -alias androiddebugkey

## Build C++ Header
Name : Build C++ Header
Program : $JDKPath$\bin\javah
Arguments : -v -jni -d $ProjectFileDir$ $FileClass$
Working Directoty : $SourcepathEntry$

## Build Java Decompile
Name : Show Class Bytecode
Program : $JDKPath$\bin\javap
Arguments : -p -s -c $FileClass$
Working Directoty : $OutputPath$

## Build android ROM
. build/envsetup.sh
lunch aosp_arbutus-userdebug
make update-api

## 统计目录下面的文件和目录总数
busybox find | busybox wc -l

## Android.mk 引用 library 工程编译
1.在引入的工程下project.properties文件中是否有这句话  android.library=true 没有就添加。
2.在自己的工程目录下 project.properties 是否有 android.library.reference.1=../Library/TopAndroid  目录一定要写对
