///////////////////////////////////////////////////////////////////////////////
// strmutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2015/4/13

#ifndef __STRMUTIL_H__
#define __STRMUTIL_H__

#include "jniutil.h"
#include "miscutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// BufferedInputStream
// ByteArrayInputStream

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Interface of the ByteArrayInputStream class
//

class ByteArrayInputStream
{
// Constructors
public:
    ByteArrayInputStream(JNIEnv* env, jbyteArray data, int32_t length, int32_t offset = 0);

// Operations
public:
    int32_t read(void* buf, int32_t size);

// Data members
protected:
    JNIEnv* mEnv;
    int32_t mOffset;
    int32_t mLength;
    jbyteArray mData;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the BufferedInputStream class
//

class BufferedInputStream : private ByteArrayInputStream
{
// Constructors
public:
    BufferedInputStream(JNIEnv* env, jobject stream, jbyteArray buf);

// Operations
public:
    int32_t read(void* buf, int32_t size);

// Data members
private:
    jobject mStream;
};

__END_NAMESPACE

#include "strmutil.inl"

#endif  // __STRMUTIL_H__