///////////////////////////////////////////////////////////////////////////////
// fileutil.inl
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
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

__INLINE__ DirectoryBase::DirectoryBase()
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

__INLINE__ FileHandle DirectoryBase::getFile() const
{
    assert(!isEmpty());
    return FileHandle(::dirfd(mDir));
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


///////////////////////////////////////////////////////////////////////////////
// Implementation of the FileScanner class
//

__INLINE__ FileScanner::FileScanner()
    : size(0), namelist(NULL)
{
}

__INLINE__ FileScanner::~FileScanner()
{
    destroy();
}

__INLINE__ int FileScanner::scan(const char* path, int (*filter)(const struct dirent*)/* = defaultFilter*/, int (*compar)(const struct dirent**, const struct dirent**)/* = alphasort*/)
{
    assert(path);
    assert(isEmpty());

    return ((size = ::scandir(path, &namelist, filter, compar)) < 0 ? __verify(errno, "Couldn't scan directory - '%s'", path) : 0);
}

__INLINE__ void FileScanner::destroy()
{
    if (namelist != NULL)
    {
        while (--size >= 0)
            ::free(namelist[size]);

        LOGI("The FileScanner was released (namelist = %p)\n", namelist);
        ::free(namelist);
        namelist = NULL;
    }
}

#ifndef NDEBUG
__INLINE__ void FileScanner::dump() const
{
    if (isEmpty())
    {
        LOGI("File list is empty\n");
    }
    else
    {
        LOGI("File Count = %d\n{\n", size);
        for (int i = 0; i < size; ++i)
            LOGI("    file info [ name = %s, type = %d, ino = " _PRId64 ", off = " _PRId64 ", reclen = %d ]\n", namelist[i]->d_name, namelist[i]->d_type, namelist[i]->d_ino, namelist[i]->d_off, namelist[i]->d_reclen);
        LOGI("}\n");
    }
}
#endif  // NDEBUG

__INLINE__ bool FileScanner::isEmpty() const
{
    return (namelist == NULL);
}

__INLINE__ const struct dirent* FileScanner::get(int index) const
{
    assert(!isEmpty());
    assert(index >= 0 && index < size);

    return namelist[index];
}


#ifdef ANDROID_ASSET_MANAGER_JNI_H
///////////////////////////////////////////////////////////////////////////////
// Implementation of the AssetDir class
//

__INLINE__ AssetDir::AssetDir()
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
// Implementation of the AssetFileHandle class
//

__INLINE__ AssetFileHandle::AssetFileHandle(void* asset/* = NULL*/)
    : mAAsset(asset), mAsset(asset != NULL ? reinterpret_cast<AAsset*>(&mAAsset) : NULL)
{
}

__INLINE__ jbyteArray AssetFileHandle::read(JNIEnv* env) const
{
    assert(env);
    assert(!isEmpty());

    jbyteArray result = NULL;
    if (const void* buffer = getBuffer())
    {
        const off_t length = getLength();
        if ((result = env->NewByteArray(length)) != NULL)
            env->SetByteArrayRegion(result, 0, length, (const jbyte*)buffer);
    }

    return result;
}

__INLINE__ int AssetFileHandle::read(void* buf, size_t size) const
{
    assert(buf);
    assert(!isEmpty());

    return ::AAsset_read(mAsset, buf, size);
}

__INLINE__ AssetFileHandle::operator AAsset*() const
{
    return mAsset;
}

__INLINE__ const void* AssetFileHandle::getBuffer() const
{
    assert(!isEmpty());

    const void* buffer = ::AAsset_getBuffer(mAsset);
    __check_error(buffer == NULL, "Couldn't get asset file buffer\n");
    return buffer;
}

__INLINE__ off_t AssetFileHandle::seek(off_t offset, int origin/* = SEEK_SET*/) const
{
    assert(!isEmpty());
    return ::AAsset_seek(mAsset, offset, origin);
}

__INLINE__ bool AssetFileHandle::isEmpty() const
{
    return (mAsset == NULL);
}

__INLINE__ int AssetFileHandle::isAllocated() const
{
    assert(!isEmpty());
    return ::AAsset_isAllocated(mAsset);
}

__INLINE__ off_t AssetFileHandle::getLength() const
{
    assert(!isEmpty());
    return ::AAsset_getLength(mAsset);
}

__INLINE__ off_t AssetFileHandle::getRemainingLength() const
{
    assert(!isEmpty());
    return ::AAsset_getRemainingLength(mAsset);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the AssetFile class
//

__INLINE__ AssetFile::AssetFile()
{
}

__INLINE__ AssetFile::~AssetFile()
{
    close();
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
#endif  // ANDROID_ASSET_MANAGER_JNI_H

__END_NAMESPACE

#endif  // __FILEUTIL_INL__