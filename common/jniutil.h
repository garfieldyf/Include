///////////////////////////////////////////////////////////////////////////////
// jniutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Creation Date : 2012/4/23

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __JNIUTIL_H__
#define __JNIUTIL_H__

#include <jni.h>
#include "miscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// jmutex_t
// jclass_t
// jstring_t
// jwstring_t
// jstringRef
// jintArray_t<t_length>
// jbyteArray_t<t_length>
// jcharArray_t<t_length>
// jlongArray_t<t_length>
// jshortArray_t<t_length>
// jfloatArray_t<t_length>
// jdoubleArray_t<t_length>
// jbooleanArray_t<t_length>
//
// Global functions in this file:
//
// getLength()
// newString()

namespace JNI {

///////////////////////////////////////////////////////////////////////////////
// Interface of the jmutex_t class
//

class jmutex_t
{
    DECLARE_NONCOPYABLE(jmutex_t);

// Constructors/Destructor
public:
    jmutex_t(JNIEnv* _env, jobject _lock);
    ~jmutex_t();

// Operations
public:
    void unlock();

// Data members
private:
    jint status;
    JNIEnv* env;
    jobject lock;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the jclass_t class
//

class jclass_t
{
    DECLARE_NONCOPYABLE(jclass_t);

// Constructors
public:
    jclass_t(JNIEnv* _env, jobject object);
    jclass_t(JNIEnv* _env, const char* _className);

// Operations
public:
    jobject CDECL newObject(jmethodID methodID, ...) const;
    jobjectArray newArray(jsize length, jobject initialElement = NULL) const;

    jint registerNatives(const JNINativeMethod* methods, jsize numMethods) const;
    jint unregisterNatives() const;

    jfieldID getFieldID(const char* fieldName, const char* signature) const;
    jfieldID getStaticFieldID(const char* fieldName, const char* signature) const;

    jmethodID getMethodID(const char* methodName, const char* signature) const;
    jmethodID getStaticMethodID(const char* methodName, const char* signature) const;

// Data members
public:
    JNIEnv* env;
    jclass clazz;

#ifndef NDEBUG
    char className[MAX_PATH];
#endif
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the basic_jstring class
//

template <typename _Ty, uint32_t t_length>
class ATL_NO_VTABLE basic_jstring
{
    DECLARE_NONCOPYABLE(basic_jstring);

// Constructors/Destructor
protected:
    basic_jstring(uint32_t len);
    ~basic_jstring();

// Operations
public:
    const _Ty* str() const;
    operator const _Ty*() const;

// Attributes
public:
    const uint32_t length;

// Data members
protected:
    _Ty* cstr;
    _Ty mstr[t_length];
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the _jstring_t class
//

template <uint32_t t_length = MAX_PATH>
class _jstring_t : public basic_jstring<char, t_length>
{
public:
    typedef basic_jstring<char, t_length> _Mybase;

// Constructors
public:
    _jstring_t(JNIEnv* env, jstring str);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the _jwstring_t class
//

template <uint32_t t_length = MAX_PATH>
class _jwstring_t : public basic_jstring<jchar, t_length>
{
public:
    typedef basic_jstring<jchar, t_length> _Mybase;

// Constructors
public:
    _jwstring_t(JNIEnv* _env, jstring _str);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the jstring_t / jwstring_t class
//

typedef _jstring_t<>    jstring_t;
typedef _jwstring_t<>   jwstring_t;


///////////////////////////////////////////////////////////////////////////////
// Interface of the jstringRef class
//

class jstringRef
{
    DECLARE_NONCOPYABLE(jstringRef);

// Constructors/Destructor
public:
    jstringRef(JNIEnv* _env, const char* _str);
    ~jstringRef();

// Data members
public:
    JNIEnv* env;
    jstring str;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the _jarray_t class
//

template <typename _Traits, uint32_t t_length>
class _jarray_t
{
    DECLARE_NONCOPYABLE(_jarray_t);

public:
    typedef typename _Traits::value_type value_type;

// Constructors/Destructor
public:
    _jarray_t(JNIEnv* env, jarray array);
    ~_jarray_t();

// Operations
public:
    value_type* data();
    const value_type* data() const;

    value_type& operator[](uint32_t index);
    value_type operator[](uint32_t index) const;

// Attributes
public:
    const uint32_t length;

// Data members
private:
    value_type* cdata;
    value_type mdata[t_length];
};


///////////////////////////////////////////////////////////////////////////////
// Global functions
//

__STATIC_INLINE__ jint getLength(JNIEnv* env, jstring str)
{
    assert(env);
    return (str != NULL ? env->GetStringLength(str) : 0);
}

__STATIC_INLINE__ jint getLength(JNIEnv* env, jarray array)
{
    assert(env);
    return (array != NULL ? env->GetArrayLength(array) : 0);
}

__STATIC_INLINE__ jstring newString(JNIEnv* env, const char* str)
{
    assert(env);
    return env->NewStringUTF(str != NULL ? str : "");
}

}  // namespace JNI

#include "jniutil.inl"

#endif  // __JNIUTIL_H__