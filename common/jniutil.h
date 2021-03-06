///////////////////////////////////////////////////////////////////////////////
// jniutil.h
//
// Author : Garfield
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
// jlocalRef_t
// jstringRef_t
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
// Constructors
public:
    _jstring_t(JNIEnv* env, jstring str);

// Implementation
private:
    using _Mybase = basic_jstring<char, t_length>;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the _jwstring_t class
//

template <jsize t_length = MAX_PATH>
class _jwstring_t : public basic_jstring<jchar, t_length>
{
// Constructors
public:
    _jwstring_t(JNIEnv* env, jstring str);

// Implementation
private:
    using _Mybase = basic_jstring<jchar, t_length>;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the jlocalRef_t class
//

template <typename _Ty>
class jlocalRef_t
{
    DECLARE_NONCOPYABLE(jlocalRef_t);

// Constructors/Destructor
public:
    jlocalRef_t(JNIEnv* env, _Ty localRef);
    ~jlocalRef_t();

// Operations
public:
    _Ty get() const;
    operator _Ty() const;

// Data members
private:
    JNIEnv* mEnv;
    _Ty mLocalRef;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the jstringRef_t class
//

class jstringRef_t : public jlocalRef_t<jstring>
{
// Constructors
public:
    jstringRef_t(JNIEnv* env, const char* str);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the jarray_t class
//

template <typename _Ty>
class ATL_NO_VTABLE jarray_t
{
    DECLARE_NONCOPYABLE(jarray_t);

// Constructors
protected:
    jarray_t(_Ty* data, jsize len);

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
    _Ty* mData;
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