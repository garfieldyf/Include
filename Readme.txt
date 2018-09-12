## GIF lib
src : http://sourceforge.net/projects/giflib/
doc : http://giflib.sourceforge.net/gif_lib.html

## regex
doc : http://pubs.opengroup.org/onlinepubs/000095399/functions/regcomp.html

## blur bitmap
git : https://github.com/kikoso/android-stackblur

## zxing
git : https://github.com/zxing/zxing.git

## zxing-cpp
git : https://github.com/glassechidna/zxing-cpp.git

## android-open-project
git : https://github.com/Trinea/android-open-project

## android-utilities
git : https://github.com/garfield-git/android-utilities.git

## android sdk proxy
mirrors.opencas.cn
mirrors.neusoft.edu.cn
ubuntu.buct.edu.cn
ubuntu.buct.cn
mirrors.neusoft.edu.cn:80

## android resource
https://lanhuapp.com

## android ROM
android SDK : https://developer.android.google.cn/
mount -o remount /system
adb shell dumpsys meminfo -a tv.fun.children
D:\Google\android-sdk-windows\platform-tools\hprof-conv E:\hprof\com.tencent.test.hprof E:\hprof\com.tencent.test.android.hprof

## build keystore
openssl pkcs8 -in platform.pk8 -inform DER -outform PEM -out platform.priv.pem -nocrypt
openssl pkcs12 -export -in platform.x509.pem -inkey platform.priv.pem -out platform.pk12 -name androiddebugkey
password: android
/D/Java/jdk1.8.0_51/bin/keytool -importkeystore -deststorepass android -destkeypass android -destkeystore debug.keystore -srckeystore platform.pk12 -srcstoretype PKCS12 -srcstorepass android -alias androiddebugkey

## Build C++ Header
Location : D:\Java\jdk1.8.0_51\bin\javah.exe
Working Directoty : ${project_loc}
Arguments : -classpath D:\Google\android-sdk-windows\platforms\android-22\android.jar;${project_loc}\bin\classes -jni ${java_type_name}

## Build Java Decompile
Location : D:\Java\jdk1.8.0_51\bin\javap.exe
Working Directoty : ${project_loc}\bin\classes
Arguments : -p -s -c -classpath D:\Google\android-sdk-windows\platforms\android-22\android.jar;${project_loc}\bin\classes ${java_type_name}

## Build Java Signature
Location : D:\Java\jdk1.8.0_51\bin\javap.exe
Working Directoty : ${project_loc}\bin\classes
Arguments : -p -s -classpath D:\Google\android-sdk-windows\platforms\android-22\android.jar;${project_loc}\bin\classes ${java_type_name}

## Build android ROM
. build/envsetup.sh
lunch aosp_arbutus-userdebug
make update-api

## /system rw
mount -o remount /system

## Android.mk 引用 library 工程编译
1.在引入的工程下project.properties文件中是否有这句话  android.library=true 没有就添加。
2.在自己的工程目录下 project.properties 是否有 android.library.reference.1=../Library/TopAndroid  目录一定要写对

