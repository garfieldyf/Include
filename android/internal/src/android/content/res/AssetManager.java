package android.content.res;

import java.io.IOException;
import java.io.InputStream;
import android.util.SparseArray;

public final class AssetManager implements AutoCloseable {
    /**
     * Mode for {@link #open(String, int)}: no specific information about how
     * data will be accessed.
     */
    public static final int ACCESS_UNKNOWN = 0;
    /**
     * Mode for {@link #open(String, int)}: Read chunks, and seek forward and
     * backward.
     */
    public static final int ACCESS_RANDOM = 1;
    /**
     * Mode for {@link #open(String, int)}: Read sequentially, with an
     * occasional forward seek.
     */
    public static final int ACCESS_STREAMING = 2;
    /**
     * Mode for {@link #open(String, int)}: Attempt to load contents into
     * memory, for fast small reads.
     */
    public static final int ACCESS_BUFFER = 3;

    /**
     * Create a new AssetManager containing only the basic system assets.
     * Applications will not generally use this method, instead retrieving the
     * appropriate asset manager with {@link Resources#getAssets}. Not for
     * use by applications.
     */
    public AssetManager() {
    }

    /**
     * Return a global shared asset manager that provides access to only
     * system assets (no application assets). {@hide}
     */
    public static AssetManager getSystem() {
        return null;
    }

    /**
     * Close this asset manager.
     */
    public void close() {
    }

    /**
     * Open an asset using ACCESS_STREAMING mode. This provides access to
     * files that have been bundled with an application as assets -- that is,
     * files placed in to the "assets" directory.
     * @param fileName The name of the asset to open. This name can be
     * hierarchical.
     * @see #open(String, int)
     * @see #list
     */
    public final InputStream open(String fileName) throws IOException {
        return open(fileName, ACCESS_STREAMING);
    }

    /**
     * Open an asset using an explicit access mode, returning an InputStream to
     * read its contents. This provides access to files that have been bundled
     * with an application as assets -- that is, files placed in to the
     * "assets" directory.
     * @param fileName The name of the asset to open. This name can be
     * hierarchical.
     * @param accessMode Desired access mode for retrieving the data.
     * @see #ACCESS_UNKNOWN
     * @see #ACCESS_STREAMING
     * @see #ACCESS_RANDOM
     * @see #ACCESS_BUFFER
     * @see #open(String)
     * @see #list
     */
    public final InputStream open(String fileName, int accessMode) throws IOException {
        return null;
    }

    public final AssetFileDescriptor openFd(String fileName) throws IOException {
        return null;
    }

    /**
     * Return a String array of all the assets at the given path.
     * @param path A relative path within the assets, i.e., "docs/home.html".
     * @return String[] Array of strings, one for each asset. These file
     * names are relative to 'path'. You can open the file by
     * concatenating 'path' and a name in the returned string (via
     * File) and passing that to open().
     * @see #open
     */
    public native final String[] list(String path) throws IOException;

    /**
     * {@hide} Open a non-asset file as an asset using ACCESS_STREAMING mode. This
     * provides direct access to all of the files included in an application
     * package (not only its assets). Applications should not normally use
     * this.
     * @see #open(String)
     */
    public final InputStream openNonAsset(String fileName) throws IOException {
        return openNonAsset(0, fileName, ACCESS_STREAMING);
    }

    /**
     * {@hide} Open a non-asset file as an asset using a specific access mode. This
     * provides direct access to all of the files included in an application
     * package (not only its assets). Applications should not normally use
     * this.
     * @see #open(String, int)
     */
    public final InputStream openNonAsset(String fileName, int accessMode) throws IOException {
        return openNonAsset(0, fileName, accessMode);
    }

    /**
     * {@hide} Open a non-asset in a specified package. Not for use by applications.
     * @param cookie Identifier of the package to be opened.
     * @param fileName Name of the asset to retrieve.
     */
    public final InputStream openNonAsset(int cookie, String fileName) throws IOException {
        return openNonAsset(cookie, fileName, ACCESS_STREAMING);
    }

    /**
     * {@hide} Open a non-asset in a specified package. Not for use by applications.
     * @param cookie Identifier of the package to be opened.
     * @param fileName Name of the asset to retrieve.
     * @param accessMode Desired access mode for retrieving the data.
     */
    public final InputStream openNonAsset(int cookie, String fileName, int accessMode) throws IOException {
        return null;
    }

    public final AssetFileDescriptor openNonAssetFd(String fileName) throws IOException {
        return openNonAssetFd(0, fileName);
    }

    public final AssetFileDescriptor openNonAssetFd(int cookie, String fileName) throws IOException {
        return null;
    }

    /**
     * Retrieve a parser for a compiled XML file.
     * @param fileName The name of the file to retrieve.
     */
    public final XmlResourceParser openXmlResourceParser(String fileName) throws IOException {
        return openXmlResourceParser(0, fileName);
    }

    /**
     * Retrieve a parser for a compiled XML file.
     * @param cookie Identifier of the package to be opened.
     * @param fileName The name of the file to retrieve.
     */
    public final XmlResourceParser openXmlResourceParser(int cookie, String fileName) throws IOException {
        return null;
    }

    public final class AssetInputStream extends InputStream {
        public final int getAssetInt() {
            throw new UnsupportedOperationException();
        }

        public final long getNativeAsset() {
            return mAsset;
        }

        private AssetInputStream(long asset) {
            mAsset = asset;
        }

        public final int read() throws IOException {
            return -1;
        }

        public final boolean markSupported() {
            return true;
        }

        public final int available() throws IOException {
            return -1;
        }

        public final void close() throws IOException {
        }

        public final void mark(int readlimit) {
        }

        public final void reset() throws IOException {
        }

        public final int read(byte[] b) throws IOException {
            return -1;
        }

        public final int read(byte[] b, int off, int len) throws IOException {
            return -1;
        }

        public final long skip(long n) throws IOException {
            return n;
        }

        private long mAsset;
    }

    /**
     * Add an additional set of assets to the asset manager. This can be
     * either a directory or ZIP file. Not for use by applications. Returns
     * the cookie of the added asset, or 0 on failure.
     */
    public final int addAssetPath(String path) {
        return 1;
    }

    /**
     * Add a set of assets to overlay an already added set of assets.
     * This is only intended for application resources. System wide resources
     * are handled before any Java code is executed.
     */
    public native final int addOverlayPath(String idmapPath);

    /**
     * Add multiple sets of assets to the asset manager at once. See {@link #addAssetPath(String)} for more information.
     * Returns array of
     * cookies for each added asset with 0 indicating failure, or null if
     * the input array of paths is null.
     */
    public final int[] addAssetPaths(String[] paths) {
        return null;
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
