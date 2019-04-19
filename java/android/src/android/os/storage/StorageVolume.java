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
        throw new UnsupportedOperationException();
    }

    public static StorageVolume fromTemplate(StorageVolume template, File path, UserHandle owner) {
        throw new UnsupportedOperationException();
    }

    public String getPath() {
        throw new UnsupportedOperationException();
    }

    public File getPathFile() {
        throw new UnsupportedOperationException();
    }

    public String getDescription(Context context) {
        throw new UnsupportedOperationException();
    }

    public int getDescriptionId() {
        throw new UnsupportedOperationException();
    }

    public boolean isPrimary() {
        throw new UnsupportedOperationException();
    }

    public boolean isRemovable() {
        throw new UnsupportedOperationException();
    }

    public boolean isEmulated() {
        throw new UnsupportedOperationException();
    }

    public int getStorageId() {
        throw new UnsupportedOperationException();
    }

    public void setStorageId(int index) {
        throw new UnsupportedOperationException();
    }

    public int getMtpReserveSpace() {
        throw new UnsupportedOperationException();
    }

    public boolean allowMassStorage() {
        throw new UnsupportedOperationException();
    }

    public long getMaxFileSize() {
        throw new UnsupportedOperationException();
    }

    public UserHandle getOwner() {
        throw new UnsupportedOperationException();
    }

    public void setUuid(String uuid) {
        throw new UnsupportedOperationException();
    }

    public String getUuid() {
        throw new UnsupportedOperationException();
    }

    public int getFatVolumeId() {
        throw new UnsupportedOperationException();
    }

    public void setUserLabel(String userLabel) {
        throw new UnsupportedOperationException();
    }

    public String getUserLabel() {
        throw new UnsupportedOperationException();
    }

    public void setState(String state) {
        throw new UnsupportedOperationException();
    }

    public String getState() {
        throw new UnsupportedOperationException();
    }

    public boolean equals(Object obj) {
        throw new UnsupportedOperationException();
    }

    public int hashCode() {
        throw new UnsupportedOperationException();
    }

    public String toString() {
        throw new UnsupportedOperationException();
    }

    public static final Creator<StorageVolume> CREATOR = new Creator<StorageVolume>() {
        public StorageVolume createFromParcel(Parcel in) {
            throw new UnsupportedOperationException();
        }

        public StorageVolume[] newArray(int size) {
            throw new UnsupportedOperationException();
        }
    };

    public int describeContents() {
        throw new UnsupportedOperationException();
    }

    public void writeToParcel(Parcel parcel, int flags) {
        throw new UnsupportedOperationException();
    }
}
