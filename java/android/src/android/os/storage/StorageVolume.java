package android.os.storage;

import java.io.File;
import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Parcel;
import android.os.Parcelable;
import android.os.UserHandle;

@SuppressLint("NewApi")
public class StorageVolume implements Parcelable {
    public static final String EXTRA_STORAGE_VOLUME = "storage_volume";

    public StorageVolume(File path, int descriptionId, boolean primary, boolean removable, boolean emulated, int mtpReserveSpace, boolean allowMassStorage, long maxFileSize, UserHandle owner) {
        throw new RuntimeException("Stub!");
    }

    public static StorageVolume fromTemplate(StorageVolume template, File path, UserHandle owner) {
        throw new RuntimeException("Stub!");
    }

    public String getPath() {
        throw new RuntimeException("Stub!");
    }

    public File getPathFile() {
        throw new RuntimeException("Stub!");
    }

    public String getDescription(Context context) {
        throw new RuntimeException("Stub!");
    }

    public int getDescriptionId() {
        throw new RuntimeException("Stub!");
    }

    public boolean isPrimary() {
        throw new RuntimeException("Stub!");
    }

    public boolean isRemovable() {
        throw new RuntimeException("Stub!");
    }

    public boolean isEmulated() {
        throw new RuntimeException("Stub!");
    }

    public int getStorageId() {
        throw new RuntimeException("Stub!");
    }

    public void setStorageId(int index) {
        throw new RuntimeException("Stub!");
    }

    public int getMtpReserveSpace() {
        throw new RuntimeException("Stub!");
    }

    public boolean allowMassStorage() {
        throw new RuntimeException("Stub!");
    }

    public long getMaxFileSize() {
        throw new RuntimeException("Stub!");
    }

    public UserHandle getOwner() {
        throw new RuntimeException("Stub!");
    }

    public void setUuid(String uuid) {
        throw new RuntimeException("Stub!");
    }

    public String getUuid() {
        throw new RuntimeException("Stub!");
    }

    public int getFatVolumeId() {
        throw new RuntimeException("Stub!");
    }

    public void setUserLabel(String userLabel) {
        throw new RuntimeException("Stub!");
    }

    public String getUserLabel() {
        throw new RuntimeException("Stub!");
    }

    public void setState(String state) {
        throw new RuntimeException("Stub!");
    }

    public String getState() {
        throw new RuntimeException("Stub!");
    }

    public boolean equals(Object obj) {
        throw new RuntimeException("Stub!");
    }

    public int hashCode() {
        throw new RuntimeException("Stub!");
    }

    public String toString() {
        throw new RuntimeException("Stub!");
    }

    public static final Creator<StorageVolume> CREATOR = new Creator<StorageVolume>() {
        public StorageVolume createFromParcel(Parcel in) {
            throw new RuntimeException("Stub!");
        }

        public StorageVolume[] newArray(int size) {
            throw new RuntimeException("Stub!");
        }
    };

    public int describeContents() {
        throw new RuntimeException("Stub!");
    }

    public void writeToParcel(Parcel parcel, int flags) {
        throw new RuntimeException("Stub!");
    }
}
