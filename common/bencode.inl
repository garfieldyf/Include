///////////////////////////////////////////////////////////////////////////////
// bencode.inl
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2011/6/22

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __BENCODE_INL__
#define __BENCODE_INL__

#ifndef __BENCODE_H__
    #error bencode.inl requires bencode.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Global functions
//

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__STATIC_INLINE__ bool dumpString(const bencode_string* str, void* userData)
{
    assert(str);
    assert(userData);

    const _Pr& printer = *(const _Pr*)userData;
    printer("  ");
    str->dump(printer, false);

    return true;
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the bencode_object class
//

__INLINE__ bencode_object::bencode_object(int _type)
    : type(_type)
{
}

__INLINE__ bencode_object::~bencode_object()
{
}

__INLINE__ bool bencode_object::_Data(const bencode_object* object, int64_t& value)
{
    const bool successful = (object != NULL && object->type == integer);
    if (successful)
        value = static_cast<const bencode_integer*>(object)->value;

    return successful;
}

__INLINE__ bool bencode_object::_Data(const bencode_object* object, struct tm& tv)
{
    return (object != NULL && object->type == integer && static_cast<const bencode_integer*>(object)->to_date(tv));
}

__INLINE__ bool bencode_object::_Data(const bencode_object* object, bencode_file& file)
{
    bool successful = (object != NULL && object->type == dict);
    if (successful)
        successful = ((file.filePath = static_cast<const bencode_dict*>(object)->find_list(KEY_BENCODE_PATH)) != NULL);

    return successful;
}

__INLINE__ bencode_object* bencode_object::_Decode(const char*& contents, bool& successful)
{
    assert(contents);

    bencode_object* object = NULL;
    switch (*contents)
    {
    case integer:
        object = new bencode_integer;
        break;

    case list:
        object = new bencode_list;
        break;

    case dict:
        object = new bencode_dict;
        break;

    default:
        object = new bencode_string;
    }

    successful = object->decode(contents);
    return object;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the bencode_integer class
//

__INLINE__ bencode_integer::bencode_integer()
    : bencode_object(integer), value(0)
{
}

__INLINE__ bencode_integer::~bencode_integer()
{
}

__INLINE__ bool bencode_integer::decode(const char*& contents)
{
    assert(contents);

    value = ::__strtoll(++contents, (char**)&contents, 10);
    const bool successful = (*contents == endpos);
    if (successful)
        ++contents;

    return successful;
}

__INLINE__ bool bencode_integer::to_date(struct tm& tv) const
{
    return __localtime(tv, value);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__INLINE__ void bencode_integer::dump(const _Pr& printer, bool dumpDate) const
{
    if (!dumpDate)
    {
        printer(_PRId64 "\n", value);
    }
    else
    {
        struct tm tv;
        _verify(to_date(tv));
        printer("%04d/%02d/%02d %02d:%02d:%02d\n", tv.tm_year + 1900, tv.tm_mon + 1, tv.tm_mday, tv.tm_hour, tv.tm_min, tv.tm_sec);
    }
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the bencode_key class
//

__INLINE__ bencode_key::bencode_key()
{
}

__INLINE__ bencode_key::bencode_key(const char* key)
    : char_sequence(key)
{
}

__INLINE__ bool bencode_key::decode(const char*& contents)
{
    assert(empty());
    assert(contents);

    size = ::strtol(contents, (char**)&contents, 10);
    const bool successful = (*contents == bencode_object::string);
    if (successful)
        data = ++contents, contents += size;

    return successful;
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__INLINE__ void bencode_key::dump(const _Pr& printer) const
{
    assert(!empty());
    printer("  KEY = %-14.*s VALUE = ", size, data);
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the bencode_string class
//

__INLINE__ bencode_string::bencode_string()
    : bencode_object(string)
{
}

__INLINE__ bencode_string::~bencode_string()
{
}

__INLINE__ bool bencode_string::decode(const char*& contents)
{
    assert(contents);
    return bencode_key::decode(contents);
}

__INLINE__ const unsigned char* bencode_string::hash(uint32_t index) const
{
    assert(!empty());
    assert(index < hash_size());

    return (const unsigned char*)(data + (index * hash_length));
}

__INLINE__ bool bencode_string::compare_hash(uint32_t index, const unsigned char* hashCode) const
{
    assert(hashCode);
    assert(!empty());
    assert(index < hash_size());

    return (::memcmp(hash(index), hashCode, hash_length) == 0);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__INLINE__ void bencode_string::dump(const _Pr& printer, bool dumpHash) const
{
    assert(!empty());

    if (dumpHash)
        printer("%u\n", hash_size());
    else
        printer("%.*s\n", size, data);
}
#endif  // _CRT_DEBUG_DUMP

__INLINE__ uint32_t bencode_string::hash_size() const
{
    assert(!empty());
    return (size / hash_length);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the bencode_list class
//

__INLINE__ bencode_list::bencode_list()
    : bencode_object(list)
{
}

__INLINE__ bencode_list::~bencode_list()
{
    for (const_iterator itor = _List.begin(); itor != _List.end(); ++itor)
        delete (*itor);
}

__INLINE__ bool bencode_list::decode(const char*& contents)
{
    assert(contents);

    bool successful = true;
    for (++contents; successful && *contents != endpos; )
        _List.push_back(_Decode(contents, successful));

    ++contents;
    return successful;
}

__INLINE__ const bencode_string* bencode_list::get_string(uint32_t index) const
{
    assert(index < size());

    const bencode_object* result = _List.at(index);
    return (result->type == string ? static_cast<const bencode_string*>(result) : NULL);
}

__INLINE__ const bencode_object* bencode_list::get(uint32_t index) const
{
    assert(index < size());
    return _List.at(index);
}

template <typename _Ty>
__INLINE__ bool bencode_list::get(uint32_t index, _Ty& value) const
{
    assert(index < size());
    return _Data(_List.at(index), value);
}

template <typename _Pred>
__INLINE__ uint32_t bencode_list::enum_strings(_Pred enumProc, void* userData/* = NULL*/) const
{
    uint32_t enumCount = 0;
    bool _Continue = true;
    for (const_iterator itor = _List.begin(); _Continue && itor != _List.end(); ++itor)
    {
        const bencode_object* object = *itor;
        assert(object);

        switch (object->type)
        {
        // bencode_list
        case list:
            enumCount += static_cast<const bencode_list*>(object)->enum_strings(enumProc, userData);
            break;

        // bencode_string
        case string:
            ++enumCount;
            _Continue = enumProc(static_cast<const bencode_string*>(object), userData);
            break;
        }
    }

    return enumCount;
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__INLINE__ void bencode_list::dump(const _Pr& printer, const bencode_string* parent) const
{
    uint32_t count = size();
    printer("[ FILES = %u ]\n[\n", count);

    for (uint32_t i = 0; i < count; ++i)
    {
        bencode_file file;
        if (get(i, file)) file.dump(printer, parent);
    }

    printer("]\n");
}
#endif  // _CRT_DEBUG_DUMP

__INLINE__ bool bencode_list::empty() const
{
    return _List.empty();
}

__INLINE__ uint32_t bencode_list::size() const
{
    return _List.size();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the bencode_dict class
//

__INLINE__ bencode_dict::bencode_dict()
    : bencode_object(dict)
{
}

__INLINE__ bencode_dict::~bencode_dict()
{
    clear();
}

__INLINE__ bool bencode_dict::decode(const char*& contents)
{
    assert(contents);

    bool successful = true;
    bencode_key key;

    for (++contents; successful && *contents != endpos; )
    {
        if (key.empty())
        {
            successful = key.decode(contents);
        }
        else
        {
            _Dict.insert(std::make_pair(key, _Decode(contents, successful)));
            key.clear();
        }
    }

    ++contents;
    return successful;
}

__INLINE__ void bencode_dict::clear()
{
    for (const_iterator itor = _Dict.begin(); itor != _Dict.end(); ++itor)
        delete (*itor).second;

    _Dict.clear();
}

__INLINE__ const bencode_object* bencode_dict::find(const char* key) const
{
    assert(key);

    const_iterator itor = _Dict.find(bencode_key(key));
    return (itor != _Dict.end() ? (*itor).second : NULL);
}

template <typename _Ty>
__INLINE__ bool bencode_dict::find(const char* key, _Ty& value) const
{
    assert(key);
    return _Data(find(key), value);
}

__INLINE__ const bencode_string* bencode_dict::find_string(const char* key) const
{
    assert(key);

    const bencode_object* result = find(key);
    return (result != NULL && result->type == string ? static_cast<const bencode_string*>(result) : NULL);
}

__INLINE__ const bencode_list* bencode_dict::find_list(const char* key) const
{
    assert(key);

    const bencode_object* result = find(key);
    return (result != NULL && result->type == list ? static_cast<const bencode_list*>(result) : NULL);
}

__INLINE__ const bencode_dict* bencode_dict::find_dict(const char* key) const
{
    assert(key);

    const bencode_object* result = find(key);
    return (result != NULL && result->type == dict ? static_cast<const bencode_dict*>(result) : NULL);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__INLINE__ void bencode_dict::dump(const _Pr& printer, const char* tag) const
{
    printer("[ %s ITEMS = %u ]\n[\n", tag, size());
    for (const_iterator itor = _Dict.begin(); itor != _Dict.end(); ++itor)
    {
        const bencode_key& key = (*itor).first;
        key.dump(printer);

        const bencode_object* object = (*itor).second;
        assert(object);

        switch (object->type)
        {
        // bencode_string
        case string:
            static_cast<const bencode_string*>(object)->dump(printer, key == KEY_BENCODE_PIECES);
            break;

        // bencode_integer
        case integer:
            static_cast<const bencode_integer*>(object)->dump(printer, key == KEY_BENCODE_CREATION_DATE);
            break;

        // others
        default:
            printer("\n");
        }
    }

    printer("]\n");
}
#endif  // _CRT_DEBUG_DUMP

__INLINE__ bool bencode_dict::empty() const
{
    return _Dict.empty();
}

__INLINE__ uint32_t bencode_dict::size() const
{
    return _Dict.size();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the bencode_file class
//

__INLINE__ bencode_file::bencode_file()
    : filePath(NULL)
{
}

__INLINE__ void bencode_file::path(char* outPath, uint32_t length, char separator/* = '\\'*/) const
{
    assert(outPath);
    assert(!empty());

    _FilePath(outPath, length, filePath->size(), separator);
}

#if defined(_STRING_) || defined(_STLP_STRING) || defined(_GLIBCXX_STRING)
__INLINE__ std::string bencode_file::path(char separator/* = '\\'*/) const
{
    assert(!empty());

    char tmpPath[MAX_PATH];
    _FilePath(tmpPath, _countof(tmpPath), filePath->size(), separator);

    return std::string(tmpPath);
}
#endif  // defined(_STRING_) || defined(_STLP_STRING) || defined(_GLIBCXX_STRING)

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__INLINE__ void bencode_file::dump(const _Pr& printer, const bencode_string* parent) const
{
    assert(!empty());

    char subPath[MAX_PATH];
    path(subPath, _countof(subPath));
    if (parent == NULL)
        printer("  PATH = %s\n", subPath);
    else
        printer("  PATH = %.*s\\%s\n", parent->size, parent->data, subPath);
}
#endif  // _CRT_DEBUG_DUMP

__INLINE__ bool bencode_file::empty() const
{
    return (filePath == NULL);
}

__INLINE__ void bencode_file::_FilePath(char* outPath, uint32_t length, uint32_t size, char separator) const
{
    assert(outPath);
    assert(!empty());
    assert(length > 0);

    *outPath = '\0';
    for (uint32_t i = 0, result = 0; i < size; ++i)
    {
        const bencode_string* tmpPath = filePath->get_string(i);
        assert(tmpPath);

        result = (i != size - 1 ? ::__snprintf(outPath, length, "%.*s%c", tmpPath->size, tmpPath->data, separator) : ::__snprintf(outPath, length, "%.*s", tmpPath->size, tmpPath->data));
        outPath += result, length -= result;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the bencode_torrent class
//

__INLINE__ bencode_torrent::bencode_torrent()
{
}

__INLINE__ bool bencode_torrent::decode(const char* filename)
{
    assert(empty());
    assert(filename);

    bool successful = false;
    if (FILE* fp = __fopen(filename, "rb"))
    {
        ::fseek(fp, 0, SEEK_END);
        size_t size = ::ftell(fp);
        ::fseek(fp, 0, SEEK_SET);

        if (char* data = _Buffer.allocateBytes(size))
        {
            for (size_t readBytes, offset = 0; size > 0; offset += readBytes, size -= readBytes)
                readBytes = ::fread(data + offset, sizeof(char), size, fp);

            successful = (*data == bencode_object::dict && _Root.decode((const char*&)data));
        }

        verify(::fclose(fp), 0);
    }
#ifndef NDEBUG
    else
    {
        LOGE("Couldn't open '%s' file\n", filename);
    }
#endif  // NDEBUG

    if (!successful)
        clear();

    return successful;
}

__INLINE__ bool bencode_torrent::decode(char* data, uint32_t size, bool attach/* = true*/)
{
    assert(data);
    assert(empty());
    assert(size > 0);

    if (attach)
        _Buffer.attach(data, size);
    else if (_Buffer.allocateBytes(size) != NULL)
        ::__memcpy(_Buffer.data, data, size);

    const char* buffer = _Buffer.data;
    const bool successful = (buffer != NULL && *buffer == bencode_object::dict && _Root.decode(buffer));
    if (!successful)
        clear();

    return successful;
}

__INLINE__ void bencode_torrent::clear(bool clearBuffer/* = true*/)
{
    _Root.clear();
    if (clearBuffer) _Buffer.clear();
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__INLINE__ void bencode_torrent::Dump(const _Pr& printer) const
{
    if (!empty())
    {
        _Root.dump(printer, "ROOT");
        if (const bencode_list* announceList = _Root.find_list(KEY_BENCODE_ANNOUNCE_LIST))
        {
            printer("[ ANNOUNCE LIST = %u ]\n[\n", announceList->size());
            announceList->enum_strings(dumpString<_Pr>, (void*)&printer);
            printer("]\n");
        }

        if (const bencode_dict* info = _Root.find_dict(KEY_BENCODE_INFO))
        {
            info->dump(printer, "INFO");
            if (const bencode_list* files = info->find_list(KEY_BENCODE_FILES))
            {
                const bencode_string* name = info->find_string(KEY_BENCODE_NAME);
                files->dump(printer, (name != NULL ? name : info->find_string(KEY_BENCODE_NAME_UTF8)));
            }
        }
    }
    else
    {
        printer("[ The bencode_torrent is empty! ]\n");
    }
}
#endif  // _CRT_DEBUG_DUMP

__INLINE__ bool bencode_torrent::empty() const
{
    return _Root.empty();
}

__INLINE__ const bencode_dict& bencode_torrent::root() const
{
    return _Root;
}

}  // namespace stdutil

#endif  // __BENCODE_INL__