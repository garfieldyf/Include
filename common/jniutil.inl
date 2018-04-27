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
struct _jtype##ArrayTraits \
{ \
    typedef _jtype value_type; \
    static _jtype* copy(JNIEnv* env, jarray array, uint32_t length, _jtype* buf, uint32_t t_length) \
    { \
        assert(env); assert(buf); assert(array); \
        _jtype* result = (length > t_length ? (_jtype*)::malloc(length * sizeof(_jtype)) : buf); \
        env->Get##_jname##ArrayRegion(static_cast<_jtype##Array>(array), 0, length, result); \
        return result; \
    } \
}; \
template <uint32_t t_length = 128> \
struct _jtype##Array_t : public _jarray_t<_jtype##ArrayTraits, t_length> \
{ \
    _jtype##Array_t(JNIEnv* env, _jtype##Array array) : _jarray_t<_jtype##ArrayTraits, t_length>(env, array) { } \
}

namespace JNI {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the jxxxArray_t class
//

DECLARE_ARRAY(jint, Int);           // jintArray_t<t_length>
DECLARE_ARRAY(jbyte, Byte);         // jbyteArray_t<t_length>
DECLARE_ARRAY(jchar, Char);         // jcharArray_t<t_length>
DECLARE_ARRAY(jlong, Long);         // jlongArray_t<t_length>
DECLARE_ARRAY(jshort, Short);       // jshortArray_t<t_length>
DECLARE_ARRAY(jfloat, Float);       // jfloatArray_t<t_length>
DECLARE_ARRAY(jdouble, Double);     // jdoubleArray_t<t_length>
DECLARE_ARRAY(jboolean, Boolean);   // jbooleanArray_t<t_length>


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jlock_t class
//

__INLINE__ jlock_t::jlock_t(JNIEnv* _env, jobject _lock)
    : env(_env), lock(_lock)
{
    assert(env);
    assert(lock);

#ifndef NDEBUG
    jint status = env->MonitorEnter(lock);
    if (status != JNI_OK)
        LOGE("Couldn't lock object (object = %p, error = %d)\n", lock, status);
#else
    env->MonitorEnter(lock);
#endif  // NDEBUG
}

__INLINE__ jlock_t::~jlock_t()
{
    assert(env);
    assert(lock);

#ifndef NDEBUG
    jint status = env->MonitorExit(lock);
    if (status != JNI_OK)
        LOGE("Couldn't unlock object (object = %p, error = %d)\n", lock, status);
#else
    env->MonitorExit(lock);
#endif  // NDEBUG
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jweak_t class
//

__INLINE__ jweak_t::jweak_t(JNIEnv* _env, jobject object)
    : env(_env), objRef(_env->NewWeakGlobalRef(object))
{
    assert(env);
    assert(object);
    assert(objRef);
}

__INLINE__ jweak_t::~jweak_t()
{
    assert(objRef);
    env->DeleteWeakGlobalRef(objRef);
}

__INLINE__ jobject jweak_t::get() const
{
    assert(objRef);
    return env->NewLocalRef(objRef);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jclass_t class
//

__INLINE__ jclass_t::jclass_t(JNIEnv* _env, jobject object)
    : env(_env), clazz(_env->GetObjectClass(object))
{
    assert(_env);
    assert(object);

#ifndef NDEBUG
    ::__strncpy(className, "unknown class", _countof(className));
    if (clazz == NULL)
    {
        LOGE("Unable to get object class - %p\n", object);
        assert(clazz);
    }
#endif  // NDEBUG
}

__INLINE__ jclass_t::jclass_t(JNIEnv* _env, const char* _className)
    : env(_env), clazz(_env->FindClass(_className))
{
    assert(_env);
    assert(_className);

#ifndef NDEBUG
    ::__strncpy(className, _className, _countof(className));
    if (clazz == NULL)
    {
        LOGE("Unable to find class - '%s'\n", _className);
        assert(clazz);
    }
#endif  // NDEBUG
}

inline jobject CDECL jclass_t::newObject(jmethodID methodID, ...) const
{
    assert(env);
    assert(clazz);
    assert(methodID);

    va_list args;
    va_start(args, methodID);

    jobject result = env->NewObjectV(clazz, methodID, args);
    va_end(args);

    return result;
}

__INLINE__ jobjectArray jclass_t::newArray(jsize length, jobject initialElement/* = NULL*/) const
{
    assert(env);
    assert(clazz);
    assert(length >= 0);

    return env->NewObjectArray(length, clazz, initialElement);
}

__INLINE__ jint jclass_t::registerNatives(const JNINativeMethod* methods, jsize numMethods) const
{
    assert(env);
    assert(clazz);
    assert(methods);

    const jint result = env->RegisterNatives(clazz, methods, numMethods);
    __check_error(result != JNI_OK, "Unable to register '%s' native methods, error = %d\n", className, result);
    return result;
}

__INLINE__ jint jclass_t::unregisterNatives() const
{
    assert(env);
    assert(clazz);

    const jint result = env->UnregisterNatives(clazz);
    __check_error(result != JNI_OK, "Unable to unregister '%s' native methods, error = %d\n", className, result);
    return result;
}

__INLINE__ jfieldID jclass_t::getFieldID(const char* fieldName, const char* signature) const
{
    assert(env);
    assert(clazz);
    assert(fieldName);
    assert(signature);

    jfieldID fieldID = env->GetFieldID(clazz, fieldName, signature);
    __check_error(fieldID == NULL, "Couldn't get field ID [class - '%s' field - '%s' signature - '%s']\n", className, fieldName, signature);
    return fieldID;
}

__INLINE__ jfieldID jclass_t::getStaticFieldID(const char* fieldName, const char* signature) const
{
    assert(env);
    assert(clazz);
    assert(fieldName);
    assert(signature);

    jfieldID fieldID = env->GetStaticFieldID(clazz, fieldName, signature);
    __check_error(fieldID == NULL, "Couldn't get static field ID [class - '%s' field - '%s' signature - '%s']\n", className, fieldName, signature);
    return fieldID;
}

__INLINE__ jmethodID jclass_t::getMethodID(const char* methodName, const char* signature) const
{
    assert(env);
    assert(clazz);
    assert(signature);
    assert(methodName);

    jmethodID methodID = env->GetMethodID(clazz, methodName, signature);
    __check_error(methodID == NULL, "Couldn't get method ID [class - '%s' method - '%s' signature - '%s']\n", className, methodName, signature);
    return methodID;
}

__INLINE__ jmethodID jclass_t::getStaticMethodID(const char* methodName, const char* signature) const
{
    assert(env);
    assert(clazz);
    assert(signature);
    assert(methodName);

    jmethodID methodID = env->GetStaticMethodID(clazz, methodName, signature);
    __check_error(methodID == NULL, "Couldn't get static method ID [class - '%s' method - '%s' signature - '%s']\n", className, methodName, signature);
    return methodID;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the basic_jstring class
//

template <typename _Ty, uint32_t t_length>
__INLINE__ basic_jstring<_Ty, t_length>::basic_jstring(uint32_t size)
    : length(size)
{
    cstr = (size >= t_length ? (_Ty*)::malloc((size + 1) * sizeof(_Ty)) : mstr);
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
// Implementation of the _jarray_t class
//

template <typename _Traits, uint32_t t_length>
__INLINE__ _jarray_t<_Traits, t_length>::_jarray_t(JNIEnv* env, jarray array)
    : length(env->GetArrayLength(array))
{
    carray = _Traits::copy(env, array, length, marray, t_length);
    assert(carray);
}

template <typename _Traits, uint32_t t_length>
__INLINE__ _jarray_t<_Traits, t_length>::~_jarray_t()
{
    if (carray != marray)
    {
        LOGW("Release HEAP array elements [ length = %u ]\n", length);
        ::free(carray);
    }
#ifndef NDEBUG
    else
    {
        LOGD("Release STACK array elements [ length = %u ]\n", length);
        ::memset(marray, 0xCCCCCCCC, sizeof(marray));
    }
#endif  // NDEBUG
}

template <typename _Traits, uint32_t t_length>
__INLINE__ typename _jarray_t<_Traits, t_length>::value_type _jarray_t<_Traits, t_length>::at(uint32_t index) const
{
    assert(carray);
    assert(index < length);

    return carray[index];
}

template <typename _Traits, uint32_t t_length>
__INLINE__ typename _jarray_t<_Traits, t_length>::value_type _jarray_t<_Traits, t_length>::operator[](uint32_t index) const
{
    assert(carray);
    assert(index < length);

    return carray[index];
}

template <typename _Traits, uint32_t t_length>
__INLINE__ const typename _jarray_t<_Traits, t_length>::value_type* _jarray_t<_Traits, t_length>::array() const
{
    return carray;
}

}  // namespace JNI

#endif  // __JNIUTIL_INL__