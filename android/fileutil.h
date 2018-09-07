///////////////////////////////////////////////////////////////////////////////
// fileutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2012/7/12

#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__

#include "jniutil.h"
#include "memutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// File
// FileHandle
// Directory
// FileScanner
// AssetDir
// AssetFile
//
// Global functions in this file:
//
// isRootDir()
// isAbsolutePath()
// splitPath()
// fileAccess()
// fileStatus()
// deleteFile()
// deleteFiles()
// emptyFilter()
// defaultFilter()
// createDirectory()

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Interface of the FileHandle class
//

class FileHandle
{
// Constructors
public:
    explicit FileHandle(int _fd = INVALID_FD);

// Operations
public:
    ssize_t read(void* buf, size_t size) const;
    ssize_t read(void* buf, size_t size, off64_t offset) const;

    ssize_t write(const void* buf, size_t size) const;
    ssize_t write(const void* buf, size_t size, off64_t offset) const;

    int truncate(off64_t length) const;
    off64_t seek(off64_t offset, int origin = SEEK_SET) const;

    int stat(struct stat& buf) const;
    int stat(const char* filename, struct stat& buf, int flags = AT_SYMLINK_NOFOLLOW) const;

// Attributes
public:
    bool isEmpty() const;
    int64_t getLength() const;

// Data members
public:
    int fd;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the File class
//

class File : public FileHandle
{
    DECLARE_NONCOPYABLE(File);

// Constructors/Destructor
public:
    File();
    ~File();

// Operations
public:
    int open(const char* filename, int flags = O_CREAT | O_WRONLY | O_TRUNC, int mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    void close();
};


///////////////////////////////////////////////////////////////////////////////
// Scan directory filter functions
//

__STATIC_INLINE__ int emptyFilter(const struct dirent* /*entry*/)
{
    return 1;
}

__STATIC_INLINE__ int defaultFilter(const struct dirent* entry)
{
    // Ignored the entry '.' and '..' representing the current and parent directory.
    return !(entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0')));
}


///////////////////////////////////////////////////////////////////////////////
// Interface of the DirectoryBase class
//

class ATL_NO_VTABLE DirectoryBase
{
    DECLARE_NONCOPYABLE(DirectoryBase);

// Constructors/Destructor
public:
    DirectoryBase();
    ~DirectoryBase();

// Operations
public:
    void close();
    void rewind() const;

// Attributes
public:
    bool isEmpty() const;
    FileHandle getFile() const;

// Data members
protected:
    DIR* mDir;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the Directory class
//

template <typename _Filter = int (*)(const struct dirent*) >
class Directory : public DirectoryBase
{
// Constructors
public:
    Directory();
    explicit Directory(_Filter _filter);

// Operations
public:
    int open(const char* path);
    int open(int fd);
    int read(struct dirent*& entry) const;

// Data members
public:
    _Filter filter;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the FileScanner class
//

class FileScanner
{
    DECLARE_NONCOPYABLE(FileScanner);

// Constructors/Destructor
public:
    FileScanner();
    ~FileScanner();

// Operations
public:
    int scan(const char* path, int (*filter)(const struct dirent*) = defaultFilter, int (*compar)(const struct dirent**, const struct dirent**) = alphasort);
    void destroy();

#ifndef NDEBUG
    void dump() const;
#endif

// Attributes
public:
    bool isEmpty() const;
    const struct dirent* get(int index) const;

// Data members
public:
    int size;
    struct dirent** namelist;
};


#ifdef ANDROID_ASSET_MANAGER_JNI_H
///////////////////////////////////////////////////////////////////////////////
// Interface of the AssetDir class
//

class AssetDir
{
    DECLARE_NONCOPYABLE(AssetDir);

// Constructors/Destructor
public:
    AssetDir();
    ~AssetDir();

// Operations
public:
    AAssetDir* open(JNIEnv* env, jobject assetManager, const char* dirName = "");
    AAssetDir* open(AAssetManager* am, const char* dirName = "");

    void close();
    void rewind() const;
    const char* next() const;

// Attributes
public:
    bool isEmpty() const;

// Data members
private:
    AAssetDir* mAssetDir;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the AssetFile class
//

class AssetFile
{
    DECLARE_NONCOPYABLE(AssetFile);

// Constructors/Destructor
public:
    AssetFile();
    ~AssetFile();

// Operations
public:
    AAsset* open(JNIEnv* env, jobject assetManager, const char* filename, int mode = AASSET_MODE_STREAMING);
    AAsset* open(AAssetManager* am, const char* filename, int mode = AASSET_MODE_STREAMING);

    void close();
    operator AAsset*() const;

    jbyteArray read(JNIEnv* env) const;
    int read(void* buf, size_t size) const;

    const void* getBuffer() const;
    off_t seek(off_t offset, int origin = SEEK_SET) const;

// Attributes
public:
    bool isEmpty() const;
    int isAllocated() const;

    off_t getLength() const;
    off_t getRemainingLength() const;

// Data members
private:
    AAsset* mAsset;
};
#endif  // ANDROID_ASSET_MANAGER_JNI_H


///////////////////////////////////////////////////////////////////////////////
// Global functions
//

__STATIC_INLINE__ bool isRootDir(const char* path)
{
    return (path != NULL && path[0] == '/' && path[1] == '\0');
}

__STATIC_INLINE__ bool isAbsolutePath(const char* path)
{
    return (path != NULL && path[0] == '/');
}

__STATIC_INLINE__ const char* splitPath(const char* path, stdutil::char_sequence& dirPath)
{
    assert(path);

    const char* result = path;
    if (const char* pathsep = ::strrchr(path, '/'))
    {
        dirPath.set(path, pathsep == path ? 1 : pathsep - path);
        result = pathsep + 1;
    }

    return result;
}

__STATIC_INLINE__ int fileAccess(const char* path, int mode)
{
    assert(path);

#ifndef NDEBUG
    const int errnum = (::access(path, mode) == 0 ? 0 : errno);
    if (errnum != 0 && mode != F_OK)
        __android_check_error(errnum, "Couldn't access file '%s' mode = %d", path, mode);

    return errnum;
#else
    return (::access(path, mode) == 0 ? 0 : errno);
#endif  // NDEBUG
}

__STATIC_INLINE__ int fileStatus(const char* path, struct stat& buf)
{
    assert(path);
    return __android_check_error((::lstat(path, &buf) == 0 ? 0 : errno), "Couldn't get file '%s' status", path);
}

__STATIC_INLINE__ int deleteFile(const char* path)
{
    assert(path);
    return __android_check_error((::remove(path) == 0 ? 0 : errno), "Couldn't delete '%s'", path);
}

static inline int deleteFiles(const char* path)
{
    assert(path);

    Directory<> dir(defaultFilter);
    int errnum = dir.open(path);
    if (errnum == 0)
    {
        char tempPath[MAX_PATH];
        const int length = ::snprintf(tempPath, _countof(tempPath), "%s/", path);

        for (struct dirent* entry; (errnum = dir.read(entry)) == 0 && entry != NULL; )
        {
            // Delete sub directory.
            ::strlcpy(tempPath + length, entry->d_name, _countof(tempPath) - length);
            if (entry->d_type == DT_DIR && (errnum = deleteFiles(tempPath)) != 0)
                break;

            // Delete sub file.
            if ((errnum = deleteFile(tempPath)) != 0)
                break;
        }
    }

    return errnum;
}

__STATIC_INLINE__ int deleteFiles(const char* path, bool deleteSelf)
{
    assert(path);

    const int errnum = deleteFiles(path);
    return (errnum == 0 && deleteSelf ? deleteFile(path) : errnum);
}

__STATIC_INLINE__ int createDirectory(const char* path, uint32_t length, mode_t mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
{
    assert(path);

    // Makes the temp path.
    char tempPath[MAX_PATH];
    ::snprintf(tempPath, _countof(tempPath), "%.*s/", length, path);

    // Checks the directory exists.
    int errnum = 0;
    if (::access(tempPath, F_OK) != 0)
    {
        // Creates the directory (including all sub directories).
        for (char* pathsep = tempPath + 1; *pathsep != '\0'; ++pathsep)
        {
            if (*pathsep == '/')
            {
                // Make the sub directory is a null-terminated string.
                *pathsep = '\0';

                // Creates the sub directory, if not exists.
                if (::access(tempPath, F_OK) != 0 && ::mkdir(tempPath, mode) != 0)
                {
                    errnum = __android_check_error(errno, "Couldn't create '%.*s' directory", length, path);
                    break;
                }

                // Restores the sub directory path separator.
                *pathsep = '/';
            }
        }
    }

    return errnum;
}

__END_NAMESPACE

#include "fileutil.inl"

#endif  // __FILEUTIL_H__