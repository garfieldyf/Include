package android.os.storage;

import java.io.File;
import android.content.ContentResolver;
import android.content.Context;
import android.os.Looper;
import android.os.RemoteException;

public class StorageManager {
    public static StorageManager from(Context context) {
        throw new UnsupportedOperationException();
    }

    public StorageManager(ContentResolver resolver, Looper tgtLooper) throws RemoteException {
        throw new UnsupportedOperationException();
    }

    public void registerListener(StorageEventListener listener) {
        throw new UnsupportedOperationException();
    }

    public void unregisterListener(StorageEventListener listener) {
        throw new UnsupportedOperationException();
    }

    public void enableUsbMassStorage() {
        throw new UnsupportedOperationException();
    }

    public void disableUsbMassStorage() {
        throw new UnsupportedOperationException();
    }

    public boolean isUsbMassStorageConnected() {
        throw new UnsupportedOperationException();
    }

    public boolean isUsbMassStorageEnabled() {
        throw new UnsupportedOperationException();
    }

    public boolean mountObb(String rawPath, String key, OnObbStateChangeListener listener) {
        throw new UnsupportedOperationException();
    }

    public boolean unmountObb(String rawPath, boolean force, OnObbStateChangeListener listener) {
        throw new UnsupportedOperationException();
    }

    public boolean isObbMounted(String rawPath) {
        throw new UnsupportedOperationException();
    }

    public String getMountedObbPath(String rawPath) {
        throw new UnsupportedOperationException();
    }

    public String getVolumeState(String mountPoint) {
        throw new UnsupportedOperationException();
    }

    public StorageVolume[] getVolumeList() {
        throw new UnsupportedOperationException();
    }

    public String[] getVolumePaths() {
        throw new UnsupportedOperationException();
    }

    public StorageVolume getPrimaryVolume() {
        throw new UnsupportedOperationException();
    }

    public static StorageVolume getPrimaryVolume(StorageVolume[] volumes) {
        throw new UnsupportedOperationException();
    }

    public long getStorageBytesUntilLow(File path) {
        throw new UnsupportedOperationException();
    }

    public long getStorageLowBytes(File path) {
        throw new UnsupportedOperationException();
    }

    public long getStorageFullBytes(File path) {
        throw new UnsupportedOperationException();
    }

    public static final int CRYPT_TYPE_PASSWORD = 0;
    public static final int CRYPT_TYPE_DEFAULT = 1;
    public static final int CRYPT_TYPE_PATTERN = 2;
    public static final int CRYPT_TYPE_PIN = 3;
    public static final String SYSTEM_LOCALE_KEY = "SystemLocale";
    public static final String OWNER_INFO_KEY = "OwnerInfo";
    public static final String PATTERN_VISIBLE_KEY = "PatternVisible";
}
