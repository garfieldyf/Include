///////////////////////////////////////////////////////////////////////////////
// fileutil.inl
//
// Author : Garfield
// Creation Date : 2012/7/12

#ifndef __FILEUTIL_INL__
#define __FILEUTIL_INL__

#ifndef __FILEUTIL_H__
    #error fileutil.inl requires fileutil.h to be included first
#endif

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Implementation of the FileHandle class
//

__INLINE__ FileHandle::FileHandle(int _fd/* = INVALID_FD*/)
    : fd(_fd)
{
}

__INLINE__ ssize_t FileHandle::read(void* buf, size_t size) const
{
    assert(buf);
    assert(!isEmpty());

    const ssize_t readBytes = ::read(fd, buf, size);
    __check_error2(readBytes == -1, "Couldn't read (size = " _PRId ")", size);
    return readBytes;
}

__INLINE__ ssize_t FileHandle::read(void* buf, size_t size, off64_t offset) const
{
    assert(buf);
    assert(!isEmpty());

    const ssize_t readBytes = ::pread64(fd, buf, size, offset);
    __check_error2(readBytes == -1, "Couldn't read (offset = " _PRId64 ", size = " _PRId ")", offset, size);
    return readBytes;
}

__INLINE__ ssize_t FileHandle::write(const void* buf, size_t size) const
{
    assert(buf);
    assert(!isEmpty());

    const ssize_t writtenBytes = ::write(fd, buf, size);
    __check_error2(writtenBytes == -1, "Couldn't write (buf = %p, size = " _PRId ")", buf, size);
    return writtenBytes;
}

__INLINE__ ssize_t FileHandle::write(const void* buf, size_t size, off64_t offset) const
{
    assert(buf);
    assert(!isEmpty());

    const ssize_t writtenBytes = ::pwrite64(fd, buf, size, offset);
    __check_error2(writtenBytes == -1, "Couldn't write (buf = %p, offset = " _PRId64 ", size = " _PRId ")", buf, offset, size);
    return writtenBytes;
}

__INLINE__ int FileHandle::truncate(off64_t length) const
{
    assert(!isEmpty());
    return __verify((::ftruncate64(fd, length) == 0 ? 0 : errno), "Couldn't truncate file (length = " _PRId64 ")", length);
}

__INLINE__ off64_t FileHandle::seek(off64_t offset, int origin/* = SEEK_SET*/) const
{
    assert(!isEmpty());

    const off64_t seekPos = ::lseek64(fd, offset, origin);
    __check_error2(seekPos == -1, "Couldn't seek (offset = " _PRId64 ", origin = %d)", offset, origin);
    return seekPos;
}

__INLINE__ int FileHandle::stat(struct stat& buf) const
{
    assert(!isEmpty());
    return __verify((::fstat(fd, &buf) == 0 ? 0 : errno), "Couldn't get file status");
}

__INLINE__ int FileHandle::stat(const char* filename, struct stat& buf, int flags/* = AT_SYMLINK_NOFOLLOW*/) const
{
    assert(filename);
    assert(!isEmpty());

    return __verify((::fstatat(fd, filename, &buf, flags) == 0 ? 0 : errno), "Couldn't get file - '%s' status", filename);
}

__INLINE__ bool FileHandle::isEmpty() const
{
    return (fd == INVALID_FD);
}

__INLINE__ int64_t FileHandle::getLength() const
{
    assert(!isEmpty());

    struct stat buf;
    return (stat(buf) == 0 ? buf.st_size : -1);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the File class
//

__INLINE__ File::File()
{
}

__INLINE__ File::~File()
{
    close();
}

__INLINE__ int File::open(const char* filename, int flags/* = O_CREAT | O_WRONLY | O_TRUNC*/, int mode/* = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH*/)
{
    assert(filename);
    assert(isEmpty());

    return __verify(((fd = ::open(filename, flags, mode)) != INVALID_FD ? 0 : errno), "Couldn't open file - '%s'", filename);
}

__INLINE__ void File::close()
{
    if (fd != INVALID_FD && ::close(fd) == 0)
    {
        LOGI("The file was closed (fd = %d)\n", fd);
        fd = INVALID_FD;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the DirectoryBase class
//

__INLINE__ constexpr DirectoryBase::DirectoryBase()
    : mDir(NULL)
{
}

__INLINE__ DirectoryBase::~DirectoryBase()
{
    close();
}

__INLINE__ int DirectoryBase::open(const char* path)
{
    assert(path);
    assert(isEmpty());

    mDir = ::opendir(path);
    return (mDir != NULL ? 0 : __verify(errno, "Couldn't open directory - '%s'", path));
}

__INLINE__ int DirectoryBase::open(int fd)
{
    assert(isEmpty());
    assert(fd != INVALID_FD);

    mDir = ::fdopendir(fd);
    return (mDir != NULL ? 0 : __verify(errno, "Couldn't open directory (file descriptor = %d)", fd));
}

__INLINE__ void DirectoryBase::close()
{
    if (mDir != NULL && ::closedir(mDir) == 0)
    {
        LOGI("The directory was closed (mDir = %p)\n", mDir);
        mDir = NULL;
    }
}

__INLINE__ void DirectoryBase::rewind() const
{
    assert(!isEmpty());
    ::rewinddir(mDir);
}

__INLINE__ bool DirectoryBase::isEmpty() const
{
    return (mDir == NULL);
}

__INLINE__ DIR* DirectoryBase::getDir() const
{
    return mDir;
}

__INLINE__ FileHandle DirectoryBase::getDirFile() const
{
    assert(!isEmpty());
    return FileHandle(::dirfd(mDir));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the DefaultFilter class
//

__INLINE__ int DefaultFilter::operator()(const struct dirent* entry) const
{
    return defaultFilter(entry);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the IgnoreHiddenFilter class
//

__INLINE__ int IgnoreHiddenFilter::operator()(const struct dirent* entry) const
{
    return ignoreHiddenFilter(entry);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Directory class
//

template <typename _Filter>
__INLINE__ Directory<_Filter>::Directory()
{
}

template <typename _Filter>
__INLINE__ Directory<_Filter>::Directory(_Filter _filter)
    : filter(_filter)
{
}

template <typename _Filter>
__INLINE__ int Directory<_Filter>::read(struct dirent*& entry) const
{
    assert(!isEmpty());

    errno = 0;
    do
    {
        entry = ::readdir(mDir);
    } while (entry != NULL && !filter(entry));

    return __verify(errno, "Couldn't read directory");
}


#ifdef ANDROID_ASSET_MANAGER_JNI_H
///////////////////////////////////////////////////////////////////////////////
// Implementation of the AssetDir class
//

__INLINE__ constexpr AssetDir::AssetDir()
    : mAssetDir(NULL)
{
}

__INLINE__ AssetDir::~AssetDir()
{
    close();
}

__INLINE__ AAssetDir* AssetDir::open(JNIEnv* env, jobject assetManager, const char* dirName/* = ""*/)
{
    assert(env);
    assert(dirName);
    assert(isEmpty());
    assert(assetManager);

    AAssetManager* am = ::AAssetManager_fromJava(env, assetManager);

#ifndef NDEBUG
    AAssetDir* dir = NULL;
    if (am != NULL)
        dir = open(am, dirName);
    else
        LOGE("Couldn't get native AssetManager (directory = '%s')\n", dirName);

    return dir;
#else
    return (am != NULL ? open(am, dirName) : NULL);
#endif  // NDEBUG
}

__INLINE__ AAssetDir* AssetDir::open(AAssetManager* am, const char* dirName/* = ""*/)
{
    assert(am);
    assert(dirName);
    assert(isEmpty());

    mAssetDir = ::AAssetManager_openDir(am, dirName);
    __check_error(mAssetDir == NULL, "Couldn't open directory - '%s'\n", dirName);
    return mAssetDir;
}

__INLINE__ void AssetDir::close()
{
    if (mAssetDir != NULL)
    {
        ::AAssetDir_close(mAssetDir);
        LOGI("The directory was closed (mAssetDir = %p)\n", mAssetDir);
        mAssetDir = NULL;
    }
}

__INLINE__ void AssetDir::rewind() const
{
    assert(!isEmpty());
    return ::AAssetDir_rewind(mAssetDir);
}

__INLINE__ const char* AssetDir::next() const
{
    assert(!isEmpty());
    return ::AAssetDir_getNextFileName(mAssetDir);
}

__INLINE__ bool AssetDir::isEmpty() const
{
    return (mAssetDir == NULL);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the AssetFile class
//

__INLINE__ constexpr AssetFile::AssetFile()
    : mAsset(NULL)
{
}

__INLINE__ AssetFile::~AssetFile()
{
    close();
}

__INLINE__ AAsset* AssetFile::open(JNIEnv* env, jobject assetManager, const char* filename, int mode/* = AASSET_MODE_STREAMING*/)
{
    assert(env);
    assert(filename);
    assert(isEmpty());
    assert(assetManager);

    AAssetManager* am = ::AAssetManager_fromJava(env, assetManager);

#ifndef NDEBUG
    AAsset* asset = NULL;
    if (am != NULL)
        asset = open(am, filename, mode);
    else
        LOGE("Couldn't get native AssetManager (filename = '%s')\n", filename);

    return asset;
#else
    return (am != NULL ? open(am, filename, mode) : NULL);
#endif  // NDEBUG
}

__INLINE__ AAsset* AssetFile::open(AAssetManager* am, const char* filename, int mode/* = AASSET_MODE_STREAMING*/)
{
    assert(am);
    assert(filename);
    assert(isEmpty());

    mAsset = ::AAssetManager_open(am, filename, mode);
    __check_error(mAsset == NULL, "Couldn't open asset file - '%s'\n", filename);
    return mAsset;
}

__INLINE__ void AssetFile::close()
{
    if (mAsset != NULL)
    {
        ::AAsset_close(mAsset);
        LOGI("The asset file was closed (mAsset = %p)\n", mAsset);
        mAsset = NULL;
    }
}

__INLINE__ AssetFile::operator AAsset*() const
{
    return mAsset;
}

__INLINE__ const void* AssetFile::getBuffer() const
{
    assert(!isEmpty());
    return ::AAsset_getBuffer(mAsset);
}

__INLINE__ int AssetFile::read(void* buf, size_t size) const
{
    assert(buf);
    assert(!isEmpty());

    return ::AAsset_read(mAsset, buf, size);
}

__INLINE__ off_t AssetFile::seek(off_t offset, int origin/* = SEEK_SET*/) const
{
    assert(!isEmpty());
    return ::AAsset_seek(mAsset, offset, origin);
}

__INLINE__ bool AssetFile::isEmpty() const
{
    return (mAsset == NULL);
}

__INLINE__ int AssetFile::isAllocated() const
{
    assert(!isEmpty());
    return ::AAsset_isAllocated(mAsset);
}

__INLINE__ off_t AssetFile::getLength() const
{
    assert(!isEmpty());
    return ::AAsset_getLength(mAsset);
}

__INLINE__ off_t AssetFile::getRemainingLength() const
{
    assert(!isEmpty());
    return ::AAsset_getRemainingLength(mAsset);
}
#endif  // ANDROID_ASSET_MANAGER_JNI_H

__END_NAMESPACE

#endif  // __FILEUTIL_INL__