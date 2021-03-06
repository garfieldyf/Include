///////////////////////////////////////////////////////////////////////////////
// jniutil.inl
//
// Author : Garfield
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
// __jni_s2utf(__string, __jstring)
// __jni_s2utf_l(__string, __length, __jstring)
// __jni_ia2a(__array, __length, __jarray)
// __jni_ba2a(__array, __length, __jarray)
// __jni_la2a(__array, __length, __jarray)
// __jni_sa2a(__array, __length, __jarray)
// __jni_fa2a(__array, __length, __jarray)
// __jni_da2a(__array, __length, __jarray)
// __jni_za2a(__array, __length, __jarray)

/*
#define __jni_s2utf(__string, __jstring)            __jni_s2utf_l(__string, __##__string##_length, __jstring)

#ifndef NDEBUG
#define __jni_s2utf_l(__string, __length, __jstring) \
    __basic_s2utf(__string, __length, __jstring); \
    LOGD("Release STACK string : %s { length = %d, capacity = %zu }\n", __string, __length, sizeof(__string))
#else
#define __jni_s2utf_l(__string, __length, __jstring) \
    __basic_s2utf(__string, __length, __jstring)
#endif  // NDEBUG

#define __basic_s2utf(__string, __length, __jstring) \
    const jsize __length = env->GetStringUTFLength(__jstring); \
    char __string[__length + 1]; \
    env->GetStringUTFRegion(__jstring, 0, env->GetStringLength(__jstring), __string); \
    __string[__length] = '\0'

#define __jni_ia2a(__array, __length, __jarray)     __jni_a2a(__array, __length, __jarray, jint, int)
#define __jni_ba2a(__array, __length, __jarray)     __jni_a2a(__array, __length, __jarray, jbyte, byte)
#define __jni_la2a(__array, __length, __jarray)     __jni_a2a(__array, __length, __jarray, jlong, long)
#define __jni_sa2a(__array, __length, __jarray)     __jni_a2a(__array, __length, __jarray, jshort, short)
#define __jni_fa2a(__array, __length, __jarray)     __jni_a2a(__array, __length, __jarray, jfloat, float)
#define __jni_da2a(__array, __length, __jarray)     __jni_a2a(__array, __length, __jarray, jdouble, double)
#define __jni_za2a(__array, __length, __jarray)     __jni_a2a(__array, __length, __jarray, jboolean, boolean)

#ifndef NDEBUG
#define __jni_a2a(__array, __length, __jarray, __jtype, __jname) \
    __basic_a2a(__array, __length, __jarray, __jtype, __jname); \
    LOGD("Release STACK "#__jtype"Array { data = %p, length = %d, capacity = %zu }\n", __array, __length, sizeof(__array))
#else
#define __jni_a2a(__array, __length, __jarray, __jtype, __jname) \
    __basic_a2a(__array, __length, __jarray, __jtype, __jname)
#endif  // NDEBUG

#define __basic_a2a(__array, __length, __jarray, __jtype, __jname) \
    const jsize __length = env->GetArrayLength(__jarray); \
    __jtype __array[__length]; \
    env->Get##__jname##ArrayRegion(__jarray, 0, __length, __array)
*/

#define DECLARE_ARRAY(__jtype, __jname) \
template <jint t_mode = 0> \
class __jtype##Array_t : public jarray_t<__jtype> { \
public: \
    __jtype##Array_t(JNIEnv* env, __jtype##Array array) \
        : jarray_t<__jtype>(env->Get##__jname##ArrayElements(array, NULL), env->GetArrayLength(array)), mEnv(env), mArray(array) \
        { static_assert(t_mode != JNI_COMMIT, "Invalid t_mode - JNI_COMMIT"); } \
    ~__jtype##Array_t() \
        { LOGD("Release "#__jtype"Array { data = %p, length = %d, mode = %s }\n", mData, length, (t_mode == JNI_ABORT ? "JNI_ABORT" : "0")); \
          mEnv->Release##__jname##ArrayElements(mArray, mData, t_mode); } \
private: \
    JNIEnv* mEnv; \
    __jtype##Array mArray; \
}

namespace JNI {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the jstring_t / jwstring_t class
//

typedef _jstring_t<>    jstring_t;
typedef _jwstring_t<>   jwstring_t;


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jxxxArray_t class
//

DECLARE_ARRAY(jint, Int);           // jintArray_t<t_mode>
DECLARE_ARRAY(jbyte, Byte);         // jbyteArray_t<t_mode>
DECLARE_ARRAY(jlong, Long);         // jlongArray_t<t_mode>
DECLARE_ARRAY(jshort, Short);       // jshortArray_t<t_mode>
DECLARE_ARRAY(jfloat, Float);       // jfloatArray_t<t_mode>
DECLARE_ARRAY(jdouble, Double);     // jdoubleArray_t<t_mode>
DECLARE_ARRAY(jboolean, Boolean);   // jbooleanArray_t<t_mode>


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jmutex_t class
//

__INLINE__ jmutex_t::jmutex_t(JNIEnv* env, jobject lock)
    : mStatus(env->MonitorEnter(lock)), mEnv(env), mLock(lock)
{
    assert(mEnv);
    assert(mLock);

#ifndef NDEBUG
    if (mStatus != JNI_OK) {
        LOGE("Couldn't lock object (object = %p, error = %d)\n", mLock, mStatus);
    }
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
        if (result != JNI_OK) {
            LOGE("Couldn't unlock object (object = %p, error = %d)\n", mLock, result);
        }
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
    if (mClass == NULL) {
        LOGE("Unable to get object class - %p\n", object);
        assert(mClass);
    } else {
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
    if (mClass == NULL) {
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

template <typename _Ty, jsize t_length>
__INLINE__ basic_jstring<_Ty, t_length>::basic_jstring(jsize len)
    : length(len)
{
    cstr = (len >= t_length ? (_Ty*)::malloc(sizeof(_Ty) * (len + 1)) : mstr);
    assert(cstr);
}

template <typename _Ty, jsize t_length>
__INLINE__ basic_jstring<_Ty, t_length>::~basic_jstring()
{
    if (cstr != mstr)
    {
        LOGW("Release HEAP string : %s { length = %d, capacity = %zu }\n", cstr, length, sizeof(mstr));
        ::free(cstr);
    }
#ifndef NDEBUG
    else
    {
        LOGD("Release STACK string : %s { length = %d, capacity = %zu }\n", cstr, length, sizeof(mstr));
        ::memset(mstr, 0xCCCCCCCC, sizeof(mstr));
    }
#endif  // NDEBUG
}

template <typename _Ty, jsize t_length>
__INLINE__ const _Ty* basic_jstring<_Ty, t_length>::str() const
{
    return cstr;
}

template <typename _Ty, jsize t_length>
__INLINE__ basic_jstring<_Ty, t_length>::operator const _Ty*() const
{
    return cstr;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the _jstring_t class
//

template <jsize t_length>
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

template <jsize t_length>
__INLINE__ _jwstring_t<t_length>::_jwstring_t(JNIEnv* env, jstring str)
    : _Mybase(env->GetStringLength(str))
{
    assert(env);
    assert(str);

    env->GetStringRegion(str, 0, this->length, this->cstr);
    this->cstr[this->length] = '\0';
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jlocalRef_t class
//

template <typename _Ty>
__INLINE__ jlocalRef_t<_Ty>::jlocalRef_t(JNIEnv* env, _Ty localRef)
    : mEnv(env), mLocalRef(localRef)
{
    assert(env);
    assert(localRef);
}

template <typename _Ty>
__INLINE__ _Ty jlocalRef_t<_Ty>::get() const
{
    return mLocalRef;
}

template <typename _Ty>
__INLINE__ jlocalRef_t<_Ty>::operator _Ty() const
{
    return mLocalRef;
}

template <typename _Ty>
__INLINE__ jlocalRef_t<_Ty>::~jlocalRef_t()
{
    mEnv->DeleteLocalRef(mLocalRef);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jstringRef_t class
//

__INLINE__ jstringRef_t::jstringRef_t(JNIEnv* env, const char* str)
    : jlocalRef_t<jstring>(env, newString(env, str))
{
    assert(env);
    assert(str);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the jarray_t class
//

template <typename _Ty>
__INLINE__ jarray_t<_Ty>::jarray_t(_Ty* data, jsize len)
    : length(len), mData(data)
{
    assert(data);
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

}  // namespace JNI

#endif  // __JNIUTIL_INL__