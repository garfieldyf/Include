///////////////////////////////////////////////////////////////////////////////
// jniutil.inl
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Creation Date : 2012/4/23

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __JNIUTIL_INL__
#define __JNIUTIL_INL__

#ifndef __JNIUTIL_H__
    #error jniutil.inl requires jniutil.h to be included first
#endif

///////////////////////////////////////////////////////////////////////////////
// Macros in this file:
//

#define DECLARE_ARRAY(_jtype, _jname) \
class _jtype##Array_t : public jarray_t<_jtype> { \
public: \
    _jtype##Array_t(JNIEnv* env, _jtype##Array array) \
        : jarray_t(env->Get##_jname##ArrayElements(array, NULL), env->GetArrayLength(array)), mEnv(env), mArray(array) \
        { assert(array); } \
    ~_jtype##Array_t() \
        { LOGD("Release " #_jtype "Array elements - { data = %p, length = %d }\n", this->data(), this->length); \
          mEnv->Release##_jname##ArrayElements(mArray, this->data(), 0); } \
private: \
    JNIEnv* mEnv; \
    _jtype##Array mArray; \
}

namespace JNI {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the jxxxArray_t class
//

DECLARE_ARRAY(jint, Int);           // jintArray_t
DECLARE_ARRAY(jbyte, Byte);         // jbyteArray_t
DECLARE_ARRAY(jchar, Char);         // jcharArray_t
DECLARE_ARRAY(jlong, Long);         // jlongArray_t
DECLARE_ARRAY(jshort, Short);       // jshortArray_t
DECLARE_ARRAY(jfloat, Float);       // jfloatArray_t
DECLARE_ARRAY(jdouble, Double);     // jdoubleArray_t
DECLARE_ARRAY(jboolean, Boolean);   // jbooleanArray_t


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jmutex_t class
//

__INLINE__ jmutex_t::jmutex_t(JNIEnv* env, jobject lock)
    : mStatus(env->MonitorEnter(lock)), mEnv(env), mLock(lock)
{
    assert(mEnv);
    assert(mLock);

#ifndef NDEBUG
    if (mStatus != JNI_OK)
        LOGE("Couldn't lock object (object = %p, error = %d)\n", mLock, mStatus);
#endif  // NDEBUG
}

__INLINE__ jmutex_t::~jmutex_t()
{
    unlock();
}

__INLINE__ void jmutex_t::unlock()
{
    assert(mEnv);
    assert(mLock);

    if (mStatus == JNI_OK)
    {
        mStatus = JNI_ERR;
    #ifndef NDEBUG
        jint result = mEnv->MonitorExit(mLock);
        if (result != JNI_OK)
            LOGE("Couldn't unlock object (object = %p, error = %d)\n", mLock, result);
    #else
        mEnv->MonitorExit(mLock);
    #endif  // NDEBUG
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jclass_t class
//

__INLINE__ jclass_t::jclass_t(JNIEnv* env, jobject object)
    : mEnv(env), mClass(env->GetObjectClass(object))
{
    assert(env);
    assert(object);

#ifndef NDEBUG
    if (mClass == NULL)
    {
        LOGE("Unable to get object class - %p\n", object);
        assert(mClass);
    }
    else
    {
        LOGI("The object %p className - '%s'\n", object, ::__android_class_name(env, object, mClassName));
    }
#endif  // NDEBUG
}

__INLINE__ jclass_t::jclass_t(JNIEnv* env, const char* className)
    : mEnv(env), mClass(env->FindClass(className))
{
    assert(env);
    assert(className);

#ifndef NDEBUG
    ::__strncpy(mClassName, className, _countof(mClassName));
    if (mClass == NULL)
    {
        LOGE("Unable to find class - '%s'\n", className);
        assert(mClass);
    }
#endif  // NDEBUG
}

inline jobject CDECL jclass_t::newObject(jmethodID methodID, ...) const
{
    assert(mEnv);
    assert(mClass);
    assert(methodID);

    va_list args;
    va_start(args, methodID);

    jobject result = mEnv->NewObjectV(mClass, methodID, args);
    va_end(args);

    return result;
}

__INLINE__ jobjectArray jclass_t::newArray(jsize length, jobject initialElement/* = NULL*/) const
{
    assert(mEnv);
    assert(mClass);
    assert(length >= 0);

    return mEnv->NewObjectArray(length, mClass, initialElement);
}

__INLINE__ jint jclass_t::registerNatives(const JNINativeMethod* methods, jsize numMethods) const
{
    assert(mEnv);
    assert(mClass);
    assert(methods);

    const jint result = mEnv->RegisterNatives(mClass, methods, numMethods);
    __check_error(result != JNI_OK, "Unable to register '%s' native methods, error = %d\n", mClassName, result);
    return result;
}

__INLINE__ jint jclass_t::unregisterNatives() const
{
    assert(mEnv);
    assert(mClass);

    const jint result = mEnv->UnregisterNatives(mClass);
    __check_error(result != JNI_OK, "Unable to unregister '%s' native methods, error = %d\n", mClassName, result);
    return result;
}

__INLINE__ jfieldID jclass_t::getFieldID(const char* fieldName, const char* signature) const
{
    assert(mEnv);
    assert(mClass);
    assert(fieldName);
    assert(signature);

    jfieldID fieldID = mEnv->GetFieldID(mClass, fieldName, signature);
    __check_error(fieldID == NULL, "Couldn't get field ID [class - '%s' field - '%s' signature - '%s']\n", mClassName, fieldName, signature);
    return fieldID;
}

__INLINE__ jfieldID jclass_t::getStaticFieldID(const char* fieldName, const char* signature) const
{
    assert(mEnv);
    assert(mClass);
    assert(fieldName);
    assert(signature);

    jfieldID fieldID = mEnv->GetStaticFieldID(mClass, fieldName, signature);
    __check_error(fieldID == NULL, "Couldn't get static field ID [class - '%s' field - '%s' signature - '%s']\n", mClassName, fieldName, signature);
    return fieldID;
}

__INLINE__ jmethodID jclass_t::getMethodID(const char* methodName, const char* signature) const
{
    assert(mEnv);
    assert(mClass);
    assert(signature);
    assert(methodName);

    jmethodID methodID = mEnv->GetMethodID(mClass, methodName, signature);
    __check_error(methodID == NULL, "Couldn't get method ID [class - '%s' method - '%s' signature - '%s']\n", mClassName, methodName, signature);
    return methodID;
}

__INLINE__ jmethodID jclass_t::getStaticMethodID(const char* methodName, const char* signature) const
{
    assert(mEnv);
    assert(mClass);
    assert(signature);
    assert(methodName);

    jmethodID methodID = mEnv->GetStaticMethodID(mClass, methodName, signature);
    __check_error(methodID == NULL, "Couldn't get static method ID [class - '%s' method - '%s' signature - '%s']\n", mClassName, methodName, signature);
    return methodID;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the basic_jstring class
//

template <typename _Ty, uint32_t t_length>
__INLINE__ basic_jstring<_Ty, t_length>::basic_jstring(uint32_t len)
    : length(len)
{
    cstr = (len >= t_length ? (_Ty*)::malloc((len + 1) * sizeof(_Ty)) : mstr);
    assert(cstr);
}

template <typename _Ty, uint32_t t_length>
__INLINE__ basic_jstring<_Ty, t_length>::~basic_jstring()
{
    if (cstr != mstr)
    {
        LOGW("Release HEAP string characters : %s(%u)\n", cstr, length);
        ::free(cstr);
    }
#ifndef NDEBUG
    else
    {
        LOGD("Release STACK string characters : %s(%u)\n", cstr, length);
        ::memset(mstr, 0xCCCCCCCC, sizeof(mstr));
    }
#endif  // NDEBUG
}

template <typename _Ty, uint32_t t_length>
__INLINE__ const _Ty* basic_jstring<_Ty, t_length>::str() const
{
    return cstr;
}

template <typename _Ty, uint32_t t_length>
__INLINE__ basic_jstring<_Ty, t_length>::operator const _Ty*() const
{
    return cstr;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the _jstring_t class
//

template <uint32_t t_length>
__INLINE__ _jstring_t<t_length>::_jstring_t(JNIEnv* env, jstring str)
    : _Mybase(env->GetStringUTFLength(str))
{
    assert(env);
    assert(str);

    env->GetStringUTFRegion(str, 0, env->GetStringLength(str), this->cstr);
    this->cstr[this->length] = '\0';
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the _jwstring_t class
//

template <uint32_t t_length>
__INLINE__ _jwstring_t<t_length>::_jwstring_t(JNIEnv* env, jstring str)
    : _Mybase(env->GetStringLength(str))
{
    assert(env);
    assert(str);

    env->GetStringRegion(str, 0, this->length, this->cstr);
    this->cstr[this->length] = '\0';
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jstringRef class
//

__INLINE__ jstringRef::jstringRef(JNIEnv* _env, const char* _str)
    : env(_env), str(newString(_env, _str))
{
    assert(env);
    assert(str);
}

__INLINE__ jstringRef::~jstringRef()
{
    env->DeleteLocalRef(str);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jarray_t class
//

template <typename _Ty>
__INLINE__ jarray_t<_Ty>::jarray_t(_Ty* _data, jsize len)
    : length(len), mData(_data)
{
    assert(_data);
    assert(len >= 0);
}

template <typename _Ty>
__INLINE__ _Ty* jarray_t<_Ty>::data()
{
    return mData;
}

template <typename _Ty>
__INLINE__ const _Ty* jarray_t<_Ty>::data() const
{
    return mData;
}

template <typename _Ty>
__INLINE__ _Ty& jarray_t<_Ty>::operator[](jsize index)
{
    assert(mData);
    assert(index >= 0 && index < length);

    return mData[index];
}

template <typename _Ty>
__INLINE__ _Ty jarray_t<_Ty>::operator[](jsize index) const
{
    assert(mData);
    assert(index >= 0 && index < length);

    return mData[index];
}

#ifndef NDEBUG
template <typename _Ty>
__INLINE__ void jarray_t<_Ty>::dump() const
{
    LOGI("jarray_t { data = %p, length = %d }\n", mData, length);
}
#endif  // NDEBUG

}  // namespace JNI

#endif  // __JNIUTIL_INL__