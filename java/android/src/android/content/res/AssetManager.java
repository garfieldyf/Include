package android.content.res;

import java.io.IOException;
import java.io.InputStream;
import android.util.SparseArray;

public final class AssetManager implements AutoCloseable {
    public static final int ACCESS_UNKNOWN = 0;
    public static final int ACCESS_RANDOM = 1;
    public static final int ACCESS_STREAMING = 2;
    public static final int ACCESS_BUFFER = 3;

    public AssetManager() {
        throw new RuntimeException("Stub!");
    }

    public static AssetManager getSystem() {
        throw new RuntimeException("Stub!");
    }

    public void close() {
        throw new RuntimeException("Stub!");
    }

    public final InputStream open(String fileName) throws IOException {
        throw new RuntimeException("Stub!");
    }

    public final InputStream open(String fileName, int accessMode) throws IOException {
        throw new RuntimeException("Stub!");
    }

    public final AssetFileDescriptor openFd(String fileName) throws IOException {
        throw new RuntimeException("Stub!");
    }

    public native final String[] list(String path) throws IOException;

    public final InputStream openNonAsset(String fileName) throws IOException {
        throw new RuntimeException("Stub!");
    }

    public final InputStream openNonAsset(String fileName, int accessMode) throws IOException {
        throw new RuntimeException("Stub!");
    }

    public final InputStream openNonAsset(int cookie, String fileName) throws IOException {
        throw new RuntimeException("Stub!");
    }

    public final InputStream openNonAsset(int cookie, String fileName, int accessMode) throws IOException {
        throw new RuntimeException("Stub!");
    }

    public final AssetFileDescriptor openNonAssetFd(String fileName) throws IOException {
        throw new RuntimeException("Stub!");
    }

    public final AssetFileDescriptor openNonAssetFd(int cookie, String fileName) throws IOException {
        throw new RuntimeException("Stub!");
    }

    public final XmlResourceParser openXmlResourceParser(String fileName) throws IOException {
        throw new RuntimeException("Stub!");
    }

    public final XmlResourceParser openXmlResourceParser(int cookie, String fileName) throws IOException {
        throw new RuntimeException("Stub!");
    }

    public final class AssetInputStream extends InputStream {
        public final int getAssetInt() {
            throw new RuntimeException("Stub!");
        }

        public final long getNativeAsset() {
            throw new RuntimeException("Stub!");
        }

        public final int read() throws IOException {
            throw new RuntimeException("Stub!");
        }

        public final boolean markSupported() {
            throw new RuntimeException("Stub!");
        }

        public final int available() throws IOException {
            throw new RuntimeException("Stub!");
        }

        public final void close() throws IOException {
            throw new RuntimeException("Stub!");
        }

        public final void mark(int readlimit) {
            throw new RuntimeException("Stub!");
        }

        public final void reset() throws IOException {
            throw new RuntimeException("Stub!");
        }

        public final int read(byte[] b) throws IOException {
            throw new RuntimeException("Stub!");
        }

        public final int read(byte[] b, int off, int len) throws IOException {
            throw new RuntimeException("Stub!");
        }

        public final long skip(long n) throws IOException {
            throw new RuntimeException("Stub!");
        }
    }

    public final int addAssetPath(String path) {
        throw new RuntimeException("Stub!");
    }

    public native final int addOverlayPath(String idmapPath);

    public final int[] addAssetPaths(String[] paths) {
        throw new RuntimeException("Stub!");
    }

    public native final boolean isUpToDate();

    public native final void setLocale(String locale);

    public native final String[] getLocales();

    public native final void setConfiguration(int mcc, int mnc, String locale, int orientation, int touchscreen, int density, int keyboard, int keyboardHidden, int navigation, int screenWidth, int screenHeight, int smallestScreenWidthDp, int screenWidthDp, int screenHeightDp, int screenLayout, int uiMode, int majorVersion);

    public native final String getCookieName(int cookie);

    public native final SparseArray<String> getAssignedPackageIdentifiers();

    public native static final int getGlobalAssetCount();

    public native static final String getAssetAllocations();

    public native static final int getGlobalAssetManagerCount();
}
