///////////////////////////////////////////////////////////////////////////////
// bencode.h
//
// Author : Garfield
// Creation Date : 2011/6/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __BENCODE_H__
#define __BENCODE_H__

#include <deque>
#include <map>
#include <time.h>
#include "memutil.h"

///////////////////////////////////////////////////////////////////////////////
// Bencode key:
//

#define KEY_BENCODE_ANNOUNCE            "announce"
#define KEY_BENCODE_COMMENT             "comment"
#define KEY_BENCODE_ENCODING            "encoding"
#define KEY_BENCODE_CREATED_BY          "created by"
#define KEY_BENCODE_CREATION_DATE       "creation date"
#define KEY_BENCODE_PUBLISHER           "publisher"
#define KEY_BENCODE_PUBLISHER_URL       "publisher-url"
#define KEY_BENCODE_ANNOUNCE_LIST       "announce-list"
#define KEY_BENCODE_INFO                "info"
#define KEY_BENCODE_NAME                "name"
#define KEY_BENCODE_NAME_UTF8           "name.utf-8"
#define KEY_BENCODE_PIECES              "pieces"
#define KEY_BENCODE_FILES               "files"
#define KEY_BENCODE_PATH                "path"
#define KEY_BENCODE_LENGTH              "length"
#define KEY_BENCODE_PIECE_LENGTH        "piece length"


///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// bencode_object
// bencode_integer
// bencode_string
// bencode_list
// bencode_dict
// bencode_file
// bencode_torrent

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Forward declarations
//

class bencode_object;
class bencode_integer;
class bencode_string;
class bencode_list;
class bencode_dict;
class bencode_file;
class bencode_torrent;


///////////////////////////////////////////////////////////////////////////////
// Interface of the bencode_object class
//

class ATL_NO_VTABLE bencode_object
{
    DECLARE_NONCOPYABLE(bencode_object);

// Constructors/Destructor
public:
    bencode_object(int _type);
    virtual ~bencode_object();

public:
    enum { dict = 'd', list = 'l', integer = 'i', string = ':', endpos = 'e' };

// Overridables
public:
    virtual bool decode(const char*& contents) = 0;

// Operations
protected:
    static bool _Data(const bencode_object* object, int64_t& value);
    static bool _Data(const bencode_object* object, struct tm& tv);
    static bool _Data(const bencode_object* object, bencode_file& file);
    static bencode_object* _Decode(const char*& contents, bool& successful);

// Data members
public:
    const int type;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the bencode_integer class
//

class bencode_integer : public bencode_object
{
// Constructors/Destructor
public:
    bencode_integer();
    virtual ~bencode_integer();

// Overrides
public:
    virtual bool decode(const char*& contents);

// Operations
public:
    bool to_date(struct tm& tv) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void dump(const _Pr& printer, bool dumpDate) const;
#endif

// Data members
public:
    int64_t value;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the bencode_key class
//

class bencode_key : public char_sequence
{
// Constructors
public:
    bencode_key();
    explicit bencode_key(const char* key);

// Operations
public:
    bool decode(const char*& contents);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void dump(const _Pr& printer) const;
#endif
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the bencode_string class
//

class bencode_string : public bencode_object, public bencode_key
{
// Constructors/Destructor
public:
    bencode_string();
    virtual ~bencode_string();

public:
    enum { hash_length = 20 };

// Overrides
public:
    virtual bool decode(const char*& contents);

// Operations
public:
    // Returns a hash code at a specified position.
    const unsigned char* hash(uint32_t index) const;

    // Compares hash code.
    bool compare_hash(uint32_t index, const unsigned char* hashCode) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void dump(const _Pr& printer, bool dumpHash) const;
#endif

// Attributes
public:
    uint32_t hash_size() const;    // The number of hash count.
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the bencode_list class
//

class bencode_list : public bencode_object
{
private:
    typedef std::deque<bencode_object*> object_list;
    typedef object_list::const_iterator const_iterator;

// Constructors/Destructor
public:
    bencode_list();
    virtual ~bencode_list();

// Overrides
public:
    virtual bool decode(const char*& contents);

// Operations
public:
    const bencode_string* get_string(uint32_t index) const;
    const bencode_object* get(uint32_t index) const;
    template <typename _Ty> bool get(uint32_t index, _Ty& value) const;

    // enumProc prototype
    // 1. bool enumProc(const bencode_string* str, void* userData);
    // 2. [](const bencode_string* str, void* userData) -> bool { ... }
    template <typename _Pred>
    uint32_t enum_strings(_Pred enumProc, void* userData = NULL) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void dump(const _Pr& printer, const bencode_string* parent) const;
#endif

// Attributes
public:
    bool empty() const;
    uint32_t size() const;

// Data members
private:
    object_list _List;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the bencode_dict class
//

class bencode_dict : public bencode_object
{
private:
    typedef std::map<bencode_key, bencode_object*> object_dict;
    typedef object_dict::const_iterator const_iterator;

// Constructors/Destructor
public:
    bencode_dict();
    virtual ~bencode_dict();

// Overrides
public:
    virtual bool decode(const char*& contents);

// Operations
public:
    void clear();

    const bencode_object* find(const char* key) const;
    template <typename _Ty> bool find(const char* key, _Ty& value) const;

    const bencode_string* find_string(const char* key) const;
    const bencode_list* find_list(const char* key) const;
    const bencode_dict* find_dict(const char* key) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void dump(const _Pr& printer, const char* tag) const;
#endif

// Attributes
public:
    bool empty() const;
    uint32_t size() const;

// Data members
private:
    object_dict _Dict;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the bencode_file class
//

class bencode_file
{
// Constructors
public:
    bencode_file();

// Operations
public:
    // Returns the file path (such as 'aaa\bbb\ccc.dat').
    void path(char* outPath, uint32_t length, char separator = '\\') const;

#if defined(_STRING_) || defined(_STLP_STRING) || defined(_GLIBCXX_STRING) || defined(_LIBCPP_STRING)
    std::string path(char separator = '\\') const;
#endif

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void dump(const _Pr& printer, const bencode_string* parent) const;
#endif

// Attributes
public:
    bool empty() const;

// Implementation
private:
    void _FilePath(char* outPath, uint32_t length, uint32_t size, char separator) const;

// Data members
public:
    const bencode_list* filePath;   // The file path list.
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the bencode_torrent class
//

class bencode_torrent
{
    DECLARE_NONCOPYABLE(bencode_torrent);

// Constructors
public:
    bencode_torrent();

// Operations
public:
    bool decode(const char* filename);
    bool decode(char* data, uint32_t size, bool attach = true);

    void clear(bool clearBuffer = true);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& printer) const;
#endif

// Attributes
public:
    bool empty() const;
    const bencode_dict& root() const;

// Data members
private:
    bencode_dict _Root;
    heap_buffer<char> _Buffer;
};

}  // namespace stdutil

#include "bencode.inl"

#endif  // __BENCODE_H__