///////////////////////////////////////////////////////////////////////////////
// jniutil.h
//
// Copyright(c) 2019, Garfield. All rights reserved.
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
// jintArray_t
// jbyteArray_t
// jcharArray_t
// jlongArray_t
// jshortArray_t
// jfloatArray_t
// jdoubleArray_t
// jbooleanArray_t
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
    jmutex_t(JNIEnv* env, jobject lock);
    ~jmutex_t();

// Operations
public:
    void unlock();

// Data members
private:
    jint mStatus;
    JNIEnv* mEnv;
    jobject mLock;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the jclass_t class
//

class jclass_t
{
    DECLARE_NONCOPYABLE(jclass_t);

// Constructors
public:
    jclass_t(JNIEnv* env, jobject object);
    jclass_t(JNIEnv* env, const char* className);

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
private:
    JNIEnv* mEnv;
    jclass mClass;

#ifndef NDEBUG
    char mClassName[MAX_PATH];
#endif
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the basic_jstring class
//

template <typename _Ty, jsize t_length>
class ATL_NO_VTABLE basic_jstring
{
    DECLARE_NONCOPYABLE(basic_jstring);

// Constructors/Destructor
protected:
    basic_jstring(jsize len);
    ~basic_jstring();

// Operations
public:
    const _Ty* str() const;
    operator const _Ty*() const;

// Attributes
public:
    const jsize length;

// Data members
protected:
    _Ty* cstr;
    _Ty mstr[t_length];
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the _jstring_t class
//

template <jsize t_length = MAX_PATH>
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

template <jsize t_length = MAX_PATH>
class _jwstring_t : public basic_jstring<jchar, t_length>
{
public:
    typedef basic_jstring<jchar, t_length> _Mybase;

// Constructors
public:
    _jwstring_t(JNIEnv* env, jstring str);
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
// Interface of the jarray_t class
//

template <typename _Ty, jsize t_length = 1024>
class ATL_NO_VTABLE jarray_t
{
    DECLARE_NONCOPYABLE(jarray_t);

// Constructors/Destructor
protected:
    jarray_t(jsize len);
    ~jarray_t();

// Operations
public:
    _Ty* data();
    const _Ty* data() const;

    _Ty& operator[](jsize index);
    _Ty operator[](jsize index) const;

// Attributes
public:
    const jsize length;

// Data members
protected:
    _Ty* cdata;
    _Ty mdata[t_length];
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