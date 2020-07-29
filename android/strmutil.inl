///////////////////////////////////////////////////////////////////////////////
// strmutil.inl
//
// Author : Garfield
// Creation Date : 2015/4/13

#ifndef __STRMUTIL_INL__
#define __STRMUTIL_INL__

#ifndef __STRMUTIL_H__
    #error strmutil.inl requires strmutil.h to be included first
#endif

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Implementation of the ByteArrayInputStream class
//

__INLINE__ ByteArrayInputStream::ByteArrayInputStream(JNIEnv* env, jbyteArray data, int32_t length, int32_t offset/* = 0*/)
    : mEnv(env), mOffset(offset), mLength(length), mData(data)
{
    assert(env);
    assert(data);
    assert(offset >= 0 && length >= 0);
    assert(env->GetArrayLength(data) - offset >= length);
}

__INLINE__ int32_t ByteArrayInputStream::read(void* buf, int32_t size)
{
    assert(buf);
    assert(mEnv);
    assert(mData);

    const int32_t readBytes = ::__Min(mLength, size);
    if (readBytes > 0)
    {
        mEnv->GetByteArrayRegion(mData, mOffset, readBytes, (jbyte*)buf);
        mOffset += readBytes;
        mLength -= readBytes;
    }

    return readBytes;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the BufferedInputStream class
//

__INLINE__ BufferedInputStream::BufferedInputStream(JNIEnv* env, jobject stream, jbyteArray buf)
    : ByteArrayInputStream(env, buf, 0), mStream(stream)
{
}

__INLINE__ int32_t BufferedInputStream::read(void* buf, int32_t size)
{
    assert(buf);
    assert(mEnv);
    assert(mData);

    static const jmethodID readID = getMethodID();
    assert(readID);

    int32_t count = 0;
    for (int32_t readBytes = 0; size > 0; size -= readBytes, count += readBytes)
    {
        if (mLength <= 0)
        {
            // Reads the data from java InputStream to the mData, if the mData is empty.
            mLength = mEnv->CallIntMethod(mStream, readID, mData);
            if (mEnv->ExceptionCheck())
            {
                mEnv->ExceptionDescribe();
                mEnv->ExceptionClear();
                count = 0;
                break;      // read threw an IOException.
            }

            LOGI("Read the data from java InputStream - read bytes = %d\n", mLength);
            if (mLength <= 0)
                break;      // read the end of the stream (EOF).

            mOffset = 0;    // reset the mOffset.
        }

        // Reads the data from the mData to buf.
        readBytes = ByteArrayInputStream::read(reinterpret_cast<jbyte*>(buf) + count, size);
    }

    return count;
}

__INLINE__ jmethodID BufferedInputStream::getMethodID()
{
    const jmethodID readID = JNI::jclass_t(mEnv, "java/io/InputStream").getMethodID("read", "([B)I");
    LOGD("obtain InputStream read methodID = %p\n", readID);
    return readID;
}

__END_NAMESPACE

#endif  // __STRMUTIL_INL__