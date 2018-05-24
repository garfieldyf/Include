/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.os.storage;

import java.io.File;
import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Parcel;
import android.os.Parcelable;
import android.os.UserHandle;

/**
 * Description of a storage volume and its capabilities, including the
 * filesystem path where it may be mounted.
 *
 * @hide
 */
@SuppressLint("NewApi")
public class StorageVolume implements Parcelable {
    // StorageVolume extra for ACTION_MEDIA_REMOVED, ACTION_MEDIA_UNMOUNTED, ACTION_MEDIA_CHECKING,
    // ACTION_MEDIA_NOFS, ACTION_MEDIA_MOUNTED, ACTION_MEDIA_SHARED, ACTION_MEDIA_UNSHARED,
    // ACTION_MEDIA_BAD_REMOVAL, ACTION_MEDIA_UNMOUNTABLE and ACTION_MEDIA_EJECT broadcasts.
    public static final String EXTRA_STORAGE_VOLUME = "storage_volume";

    public StorageVolume(File path, int descriptionId, boolean primary, boolean removable,
            boolean emulated, int mtpReserveSpace, boolean allowMassStorage, long maxFileSize,
            UserHandle owner) {
        throw new UnsupportedOperationException();
    }

    public static StorageVolume fromTemplate(StorageVolume template, File path, UserHandle owner) {
        throw new UnsupportedOperationException();
    }

    /**
     * Returns the mount path for the volume.
     *
     * @return the mount path
     */
    public String getPath() {
        throw new UnsupportedOperationException();
    }

    public File getPathFile() {
        throw new UnsupportedOperationException();
    }

    /**
     * Returns a user visible description of the volume.
     *
     * @return the volume description
     */
    public String getDescription(Context context) {
        throw new UnsupportedOperationException();
    }

    public int getDescriptionId() {
        throw new UnsupportedOperationException();
    }

    public boolean isPrimary() {
        throw new UnsupportedOperationException();
    }

    /**
     * Returns true if the volume is removable.
     *
     * @return is removable
     */
    public boolean isRemovable() {
        throw new UnsupportedOperationException();
    }

    /**
     * Returns true if the volume is emulated.
     *
     * @return is removable
     */
    public boolean isEmulated() {
        throw new UnsupportedOperationException();
    }

    /**
     * Returns the MTP storage ID for the volume.
     * this is also used for the storage_id column in the media provider.
     *
     * @return MTP storage ID
     */
    public int getStorageId() {
        throw new UnsupportedOperationException();
    }

    /**
     * Do not call this unless you are MountService
     */
    public void setStorageId(int index) {
        throw new UnsupportedOperationException();
    }

    /**
     * Number of megabytes of space to leave unallocated by MTP.
     * MTP will subtract this value from the free space it reports back
     * to the host via GetStorageInfo, and will not allow new files to
     * be added via MTP if there is less than this amount left free in the storage.
     * If MTP has dedicated storage this value should be zero, but if MTP is
     * sharing storage with the rest of the system, set this to a positive value
     * to ensure that MTP activity does not result in the storage being
     * too close to full.
     *
     * @return MTP reserve space
     */
    public int getMtpReserveSpace() {
        throw new UnsupportedOperationException();
    }

    /**
     * Returns true if this volume can be shared via USB mass storage.
     *
     * @return whether mass storage is allowed
     */
    public boolean allowMassStorage() {
        throw new UnsupportedOperationException();
    }

    /**
     * Returns maximum file size for the volume, or zero if it is unbounded.
     *
     * @return maximum file size
     */
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

    /**
     * Parse and return volume UUID as FAT volume ID, or return -1 if unable to
     * parse or UUID is unknown.
     */
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

    @Override
    public boolean equals(Object obj) {
        throw new UnsupportedOperationException();
    }

    @Override
    public int hashCode() {
        throw new UnsupportedOperationException();
    }

    @Override
    public String toString() {
        throw new UnsupportedOperationException();
    }

    public static final Creator<StorageVolume> CREATOR = new Creator<StorageVolume>() {
        @Override
        public StorageVolume createFromParcel(Parcel in) {
            throw new UnsupportedOperationException();
        }

        @Override
        public StorageVolume[] newArray(int size) {
            throw new UnsupportedOperationException();
        }
    };

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int flags) {
        throw new UnsupportedOperationException();
    }
}
