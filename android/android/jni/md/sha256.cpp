/**
 * SHA256 in cpp
 */

#include "md.h"
#include <string.h>
#include "platform.h"

/* Help macros */
#define SHA256_BLOCK_SIZE           64
#define SHA256_SHFR(x, n)           (x >> n)
#define SHA256_ROTR(x, n)           ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA256_CH(x, y, z)          ((x & y) ^ (~x & z))
#define SHA256_MAJ(x, y, z)         ((x & y) ^ (x & z) ^ (y & z))

/* F1, F2, F3, F4 operations used in SHA256 */
#define SHA256_F1(x)                (SHA256_ROTR(x,  2) ^ SHA256_ROTR(x, 13) ^ SHA256_ROTR(x, 22))
#define SHA256_F2(x)                (SHA256_ROTR(x,  6) ^ SHA256_ROTR(x, 11) ^ SHA256_ROTR(x, 25))
#define SHA256_F3(x)                (SHA256_ROTR(x,  7) ^ SHA256_ROTR(x, 18) ^ SHA256_SHFR(x,  3))
#define SHA256_F4(x)                (SHA256_ROTR(x, 17) ^ SHA256_ROTR(x, 19) ^ SHA256_SHFR(x, 10))

static const uint32_t SHA256_PADDING[] =
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

__STATIC_INLINE__ uint32_t SHA256Pack(const u_char* block)
{
    return (((uint32_t)block[3])
           |((uint32_t)block[2] << 8)
           |((uint32_t)block[1] << 16)
           |((uint32_t)block[0] << 24));
}

__STATIC_INLINE__ void SHA256Unpack(u_char* block, uint32_t x)
{
    block[3] = (u_char)x;
    block[2] = (u_char)(x >>  8);
    block[1] = (u_char)(x >> 16);
    block[0] = (u_char)(x >> 24);
}

/**
 * Basic SHA256 step. Transforms buffer based on in.
 */
static void SHA256Transform(SHA256_CTX* context, const u_char* data, uint32_t size)
{
    uint32_t buffer[64], state[8];
    for (uint32_t i = 0, state1 = 0, state2 = 0; i < size; ++i)
    {
        const u_char* block = data + (i << 6);
        for (int j = 0; j < 16; ++j)
            buffer[j] = SHA256Pack(&block[j << 2]);

        for (int j = 16; j < 64; ++j)
            buffer[j] = SHA256_F4(buffer[j -  2]) + buffer[j -  7] + SHA256_F3(buffer[j - 15]) + buffer[j - 16];

        ::memcpy(state, context->state, sizeof(state));
        for (int j = 0; j < 64; ++j)
        {
            state1 = state[7] + SHA256_F2(state[4]) + SHA256_CH(state[4], state[5], state[6]) + SHA256_PADDING[j] + buffer[j];
            state2 = SHA256_F1(state[0]) + SHA256_MAJ(state[0], state[1], state[2]);
            state[7] = state[6];
            state[6] = state[5];
            state[5] = state[4];
            state[4] = state[3] + state1;
            state[3] = state[2];
            state[2] = state[1];
            state[1] = state[0];
            state[0] = state1 + state2;
        }

        for (int j = 0; j < 8; ++j)
            context->state[j] += state[j];
    }
}

__BEGIN_DECLS
/**
 * The routine SHA256Init initializes the SHA256 context.
 */
STDCEXPORT void SHA256Init(SHA256_CTX* context)
{
    context->state[0] = 0x6a09e667;
    context->state[1] = 0xbb67ae85;
    context->state[2] = 0x3c6ef372;
    context->state[3] = 0xa54ff53a;
    context->state[4] = 0x510e527f;
    context->state[5] = 0x9b05688c;
    context->state[6] = 0x1f83d9ab;
    context->state[7] = 0x5be0cd19;
    context->count[0] = context->count[1] = 0;
}

/**
 * The routine SHA256Update updates the SHA256 context to account
 * for the presence of each of the characters data[0..size - 1] in
 * the message whose digest is being computed.
 */
STDCEXPORT void SHA256Update(SHA256_CTX* context, const u_char* data, uint32_t size)
{
    const uint32_t temp = SHA256_BLOCK_SIZE - context->count[1];
    uint32_t remainSize = (size < temp ? size : temp);

    ::memcpy(&context->buffer[context->count[1]], data, remainSize);
    if (context->count[1] + size < SHA256_BLOCK_SIZE)
    {
        context->count[1] += size;
    }
    else
    {
        const uint32_t newSize   = size - remainSize;
        const uint32_t blockSize = newSize / SHA256_BLOCK_SIZE;
        const u_char* remainData = data + remainSize;

        SHA256Transform(context, context->buffer, 1);
        SHA256Transform(context, remainData, blockSize);

        remainSize = newSize % SHA256_BLOCK_SIZE;
        ::memcpy(context->buffer, &remainData[blockSize << 6], remainSize);
        context->count[1]  = remainSize;
        context->count[0] += (blockSize + 1) << 6;
    }
}

/**
 * The routine SHA256 terminates the SHA256 computation and
 * ends with the desired message digest in digest[0...31].
 */
STDCEXPORT void SHA256Final(SHA256_CTX* context, u_char digest[SHA256_DIGEST_LENGTH])
{
    const uint32_t blockSize = 1 + ((SHA256_BLOCK_SIZE - 9) < (context->count[1] % SHA256_BLOCK_SIZE));
    const uint32_t length = (context->count[0] + context->count[1]) << 3;
    const uint32_t count  = blockSize << 6;

    ::memset(context->buffer + context->count[1], 0, count - context->count[1]);
    context->buffer[context->count[1]] = 0x80;

    SHA256Unpack(context->buffer + count - 4, length);
    SHA256Transform(context, context->buffer, blockSize);

    for (int i = 0 ; i < 8; ++i)
        SHA256Unpack(&digest[i << 2], context->state[i]);
}
__END_DECLS