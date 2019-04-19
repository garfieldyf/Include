package android.os.storage;

import java.io.File;
import android.content.ContentResolver;
import android.content.Context;
import android.os.Looper;
import android.os.RemoteException;

public class StorageManager {
    public static StorageManager from(Context context) {
        throw new RuntimeException("Stub!");
    }

    public StorageManager(ContentResolver resolver, Looper tgtLooper) throws RemoteException {
        throw new RuntimeException("Stub!");
    }

    public void registerListener(StorageEventListener listener) {
        throw new RuntimeException("Stub!");
    }

    public void unregisterListener(StorageEventListener listener) {
        throw new RuntimeException("Stub!");
    }

    public void enableUsbMassStorage() {
        throw new RuntimeException("Stub!");
    }

    public void disableUsbMassStorage() {
        throw new RuntimeException("Stub!");
    }

    public boolean isUsbMassStorageConnected() {
        throw new RuntimeException("Stub!");
    }

    public boolean isUsbMassStorageEnabled() {
        throw new RuntimeException("Stub!");
    }

    public boolean mountObb(String rawPath, String key, OnObbStateChangeListener listener) {
        throw new RuntimeException("Stub!");
    }

    public boolean unmountObb(String rawPath, boolean force, OnObbStateChangeListener listener) {
        throw new RuntimeException("Stub!");
    }

    public boolean isObbMounted(String rawPath) {
        throw new RuntimeException("Stub!");
    }

    public String getMountedObbPath(String rawPath) {
        throw new RuntimeException("Stub!");
    }

    public String getVolumeState(String mountPoint) {
        throw new RuntimeException("Stub!");
    }

    public StorageVolume[] getVolumeList() {
        throw new RuntimeException("Stub!");
    }

    public String[] getVolumePaths() {
        throw new RuntimeException("Stub!");
    }

    public StorageVolume getPrimaryVolume() {
        throw new RuntimeException("Stub!");
    }

    public static StorageVolume getPrimaryVolume(StorageVolume[] volumes) {
        throw new RuntimeException("Stub!");
    }

    public long getStorageBytesUntilLow(File path) {
        throw new RuntimeException("Stub!");
    }

    public long getStorageLowBytes(File path) {
        throw new RuntimeException("Stub!");
    }

    public long getStorageFullBytes(File path) {
        throw new RuntimeException("Stub!");
    }

    public static final int CRYPT_TYPE_PASSWORD = 0;
    public static final int CRYPT_TYPE_DEFAULT = 1;
    public static final int CRYPT_TYPE_PATTERN = 2;
    public static final int CRYPT_TYPE_PIN = 3;
    public static final String SYSTEM_LOCALE_KEY = "SystemLocale";
    public static final String OWNER_INFO_KEY = "OwnerInfo";
    public static final String PATTERN_VISIBLE_KEY = "PatternVisible";
}
