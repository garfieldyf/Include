GIF lib
src : http://sourceforge.net/projects/giflib/
doc : http://giflib.sourceforge.net/gif_lib.html

regex
doc : http://pubs.opengroup.org/onlinepubs/000095399/functions/regcomp.html

blur bitmap
git : https://github.com/kikoso/android-stackblur

zxing
git : https://github.com/zxing/zxing.git

zxing-cpp
git : https://github.com/glassechidna/zxing-cpp.git

android-open-project
git : https://github.com/Trinea/android-open-project

android-utilities
git : https://github.com/garfield-git/android-utilities.git

android sdk
mirrors.opencas.cn
mirrors.neusoft.edu.cn
ubuntu.buct.edu.cn
ubuntu.buct.cn

mirrors.neusoft.edu.cn:80

android ROM
mount -o remount /system
adb shell dumpsys meminfo -a tv.fun.children
D:\Google\android-sdk-windows\platform-tools\hprof-conv E:\hprof\com.tencent.test.hprof E:\hprof\com.tencent.test.android.hprof

build keystore
openssl pkcs8 -in platform.pk8 -inform DER -outform PEM -out platform.priv.pem -nocrypt
openssl pkcs12 -export -in platform.x509.pem -inkey platform.priv.pem -out platform.pk12 -name androiddebugkey
password: android
/D/Java/jdk1.8.0_51/bin/keytool -importkeystore -deststorepass android -destkeypass android -destkeystore debug.keystore -srckeystore platform.pk12 -srcstoretype PKCS12 -srcstorepass android -alias androiddebugkey
