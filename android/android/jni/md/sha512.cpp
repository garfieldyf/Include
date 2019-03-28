/**
 * SHA512 in cpp
 */

#include "md.h"
#include <string.h>
#include "platform.h"

/* Help macros */
#define SHA512_BLOCK_SIZE           128
#define SHA512_SHFR(x, n)           (x >> n)
#define SHA512_ROTR(x, n)           ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA512_CH(x, y, z)          ((x & y) ^ (~x & z))
#define SHA512_MAJ(x, y, z)         ((x & y) ^ (x & z) ^ (y & z))

/* F1, F2, F3, F4 operations used in SHA512 */
#define SHA512_F1(x)                (SHA512_ROTR(x, 28) ^ SHA512_ROTR(x, 34) ^ SHA512_ROTR(x, 39))
#define SHA512_F2(x)                (SHA512_ROTR(x, 14) ^ SHA512_ROTR(x, 18) ^ SHA512_ROTR(x, 41))
#define SHA512_F3(x)                (SHA512_ROTR(x,  1) ^ SHA512_ROTR(x,  8) ^ SHA512_SHFR(x,  7))
#define SHA512_F4(x)                (SHA512_ROTR(x, 19) ^ SHA512_ROTR(x, 61) ^ SHA512_SHFR(x,  6))

static const uint64_t SHA512_PADDING[] =
{
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
    0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
    0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
    0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
    0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
    0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
    0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
    0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
    0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
    0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
    0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
    0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL,
};

__STATIC_INLINE__ uint64_t SHA512Pack(const u_char* block)
{
    return (((uint64_t)block[7])
           |((uint64_t)block[6] << 8)
           |((uint64_t)block[5] << 16)
           |((uint64_t)block[4] << 24)
           |((uint64_t)block[3] << 32)
           |((uint64_t)block[2] << 40)
           |((uint64_t)block[1] << 48)
           |((uint64_t)block[0] << 56));
}

__STATIC_INLINE__ void SHA256Unpack(u_char* block, uint32_t x)
{
    block[3] = (u_char)x;
    block[2] = (u_char)(x >>  8);
    block[1] = (u_char)(x >> 16);
    block[0] = (u_char)(x >> 24);
}

__STATIC_INLINE__ void SHA512Unpack(u_char* block, uint64_t x)
{
    block[7] = (u_char)x;
    block[6] = (u_char)(x >>  8);
    block[5] = (u_char)(x >> 16);
    block[4] = (u_char)(x >> 24);
    block[3] = (u_char)(x >> 32);
    block[2] = (u_char)(x >> 40);
    block[1] = (u_char)(x >> 48);
    block[0] = (u_char)(x >> 56);
}

/**
 * Basic SHA512 step. Transforms buffer based on in.
 */
static void SHA512Transform(SHA512_CTX* context, const u_char* data, uint32_t size)
{
    uint64_t buffer[80], state[8], state1 = 0, state2 = 0;
    for (uint32_t i = 0; i < size; ++i)
    {
        const u_char* block = data + (i << 7);
        for (uint32_t j = 0; j < 16; ++j)
            buffer[j] = SHA512Pack(&block[j << 3]);

        for (uint32_t j = 16; j < 80; ++j)
            buffer[j] =  SHA512_F4(buffer[j -  2]) + buffer[j -  7] + SHA512_F3(buffer[j - 15]) + buffer[j - 16];

        ::memcpy(state, context->state, sizeof(state));
        for (uint32_t j = 0; j < 80; ++j)
        {
            state1 = state[7] + SHA512_F2(state[4]) + SHA512_CH(state[4], state[5], state[6]) + SHA512_PADDING[j] + buffer[j];
            state2 = SHA512_F1(state[0]) + SHA512_MAJ(state[0], state[1], state[2]);
            state[7] = state[6];
            state[6] = state[5];
            state[5] = state[4];
            state[4] = state[3] + state1;
            state[3] = state[2];
            state[2] = state[1];
            state[1] = state[0];
            state[0] = state1 + state2;
        }

        for (uint32_t j = 0; j < 8; ++j)
            context->state[j] += state[j];
    }
}

/**
 * The routine SHA384/SHA512 terminates the SHA384/SHA512 computation
 * and ends with the desired message digest in digest[0..size - 1].
 */
static void SHAFinalImpl(SHA512_CTX* context, u_char* digest, uint32_t size)
{
    const uint32_t blockSize = 1 + ((SHA512_BLOCK_SIZE - 17) < (context->count[1] % SHA512_BLOCK_SIZE));
    const uint32_t length = (context->count[0] + context->count[1]) << 3;
    const uint32_t count  = blockSize << 7;

    ::memset(context->buffer + context->count[1], 0, count - context->count[1]);
    context->buffer[context->count[1]] = 0x80;

    SHA256Unpack(context->buffer + count - 4, length);
    SHA512Transform(context, context->buffer, blockSize);

    size /= sizeof(uint64_t);
    for (uint32_t i = 0; i < size; ++i)
        SHA512Unpack(&digest[i << 3], context->state[i]);
}

__BEGIN_DECLS
/**
 * The routine SHA384Init initializes the SHA512 context.
 */
void SHA384Init(SHA512_CTX* context)
{
    context->state[0] = 0xcbbb9d5dc1059ed8ULL;
    context->state[1] = 0x629a292a367cd507ULL;
    context->state[2] = 0x9159015a3070dd17ULL;
    context->state[3] = 0x152fecd8f70e5939ULL;
    context->state[4] = 0x67332667ffc00b31ULL;
    context->state[5] = 0x8eb44a8768581511ULL;
    context->state[6] = 0xdb0c2e0d64f98fa7ULL;
    context->state[7] = 0x47b5481dbefa4fa4ULL;
    context->count[1] = context->count[0] = 0;
}

/**
 * The routine SHA384 terminates the SHA384 computation and
 * ends with the desired message digest in digest[0..size - 1].
 */
void SHA384Final(SHA512_CTX* context, u_char digest[SHA384_DIGEST_LENGTH])
{
    SHAFinalImpl(context, digest, SHA384_DIGEST_LENGTH);
}

/**
 * The routine SHA512Init initializes the SHA512 context.
 */
void SHA512Init(SHA512_CTX* context)
{
    context->state[0] = 0x6a09e667f3bcc908ULL;
    context->state[1] = 0xbb67ae8584caa73bULL;
    context->state[2] = 0x3c6ef372fe94f82bULL;
    context->state[3] = 0xa54ff53a5f1d36f1ULL;
    context->state[4] = 0x510e527fade682d1ULL;
    context->state[5] = 0x9b05688c2b3e6c1fULL;
    context->state[6] = 0x1f83d9abfb41bd6bULL;
    context->state[7] = 0x5be0cd19137e2179ULL;
    context->count[1] = context->count[0] = 0;
}

/**
 * The routine SHA512Update updates the SHA512 context to account
 * for the presence of each of the characters data[0..size - 1] in
 * the message whose digest is being computed.
 */
void SHA512Update(SHA512_CTX* context, const u_char* data, uint32_t size)
{
    const uint32_t temp = SHA512_BLOCK_SIZE - context->count[1];
    uint32_t remainSize = (size < temp ? size : temp);

    ::memcpy(&context->buffer[context->count[1]], data, remainSize);
    if (context->count[1] + size < SHA512_BLOCK_SIZE)
    {
        context->count[1] += size;
    }
    else
    {
        const uint32_t newSize   = size - remainSize;
        const uint32_t blockSize = newSize / SHA512_BLOCK_SIZE;
        const u_char* remainData = data + remainSize;

        SHA512Transform(context, context->buffer, 1);
        SHA512Transform(context, remainData, blockSize);

        remainSize = newSize % SHA512_BLOCK_SIZE;
        ::memcpy(context->buffer, &remainData[blockSize << 7], remainSize);
        context->count[1] = remainSize;
        context->count[0] += (blockSize + 1) << 7;
    }
}

/**
 * The routine SHA512 terminates the SHA512 computation and
 * ends with the desired message digest in digest[0...63].
 */
void SHA512Final(SHA512_CTX* context, u_char digest[SHA512_DIGEST_LENGTH])
{
    SHAFinalImpl(context, digest, SHA512_DIGEST_LENGTH);
}
__END_DECLS