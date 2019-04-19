package android.os.storage;

public abstract class StorageEventListener {
    public void onUsbMassStorageConnectionChanged(boolean connected) {
        throw new RuntimeException("Stub!");
    }

    public void onStorageStateChanged(String path, String oldState, String newState) {
        throw new RuntimeException("Stub!");
    }
}
