/**
 * MD5 in cpp
 */

#include "md.h"
#include "platform.h"

#define S11  7
#define S12 12
#define S13 17
#define S14 22
#define S21  5
#define S22  9
#define S23 14
#define S24 20
#define S31  4
#define S32 11
#define S33 16
#define S34 23
#define S41  6
#define S42 10
#define S43 15
#define S44 21

/**
 * F, G, H and I are basic MD5 functions
 */
#define F(x, y, z)  (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z)  (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z)  ((x) ^ (y) ^ (z))
#define I(x, y, z)  ((y) ^ ((x) | (~z)))

/**
 * ROTATE_LEFT rotates x left n bits
 */
#define ROTATE_LEFT(x, n)   (((x) << (n)) | ((x) >> (32-(n))))

/**
 * FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4
 * Rotation is separate from addition to prevent recomputation
 */
#define FF(a, b, c, d, x, s, ac) \
    { \
        (a) += F((b), (c), (d)) + (x) + (ac); \
        (a) = ROTATE_LEFT((a), (s)); \
        (a) += (b); \
    }

#define GG(a, b, c, d, x, s, ac) \
    { \
        (a) += G((b), (c), (d)) + (x) + (ac); \
        (a) = ROTATE_LEFT((a), (s)); \
        (a) += (b); \
    }

#define HH(a, b, c, d, x, s, ac) \
    { \
        (a) += H((b), (c), (d)) + (x) + (ac); \
        (a) = ROTATE_LEFT((a), (s)); \
        (a) += (b); \
    }

#define II(a, b, c, d, x, s, ac) \
    { \
        (a) += I((b), (c), (d)) + (x) + (ac); \
        (a) = ROTATE_LEFT((a), (s)); \
        (a) += (b); \
    }

static const u_char PADDING[] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/**
 * Basic MD5 step. Transforms buffer based on in.
 */
static void MD5Transform(uint32_t state[4], uint32_t in[16])
{
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3];

    /* Round 1 */
    FF(a, b, c, d, in[ 0], S11, 3614090360U); /* 1 */
    FF(d, a, b, c, in[ 1], S12, 3905402710U); /* 2 */
    FF(c, d, a, b, in[ 2], S13,  606105819U); /* 3 */
    FF(b, c, d, a, in[ 3], S14, 3250441966U); /* 4 */
    FF(a, b, c, d, in[ 4], S11, 4118548399U); /* 5 */
    FF(d, a, b, c, in[ 5], S12, 1200080426U); /* 6 */
    FF(c, d, a, b, in[ 6], S13, 2821735955U); /* 7 */
    FF(b, c, d, a, in[ 7], S14, 4249261313U); /* 8 */
    FF(a, b, c, d, in[ 8], S11, 1770035416U); /* 9 */
    FF(d, a, b, c, in[ 9], S12, 2336552879U); /* 10 */
    FF(c, d, a, b, in[10], S13, 4294925233U); /* 11 */
    FF(b, c, d, a, in[11], S14, 2304563134U); /* 12 */
    FF(a, b, c, d, in[12], S11, 1804603682U); /* 13 */
    FF(d, a, b, c, in[13], S12, 4254626195U); /* 14 */
    FF(c, d, a, b, in[14], S13, 2792965006U); /* 15 */
    FF(b, c, d, a, in[15], S14, 1236535329U); /* 16 */

    /* Round 2 */
    GG(a, b, c, d, in[ 1], S21, 4129170786U); /* 17 */
    GG(d, a, b, c, in[ 6], S22, 3225465664U); /* 18 */
    GG(c, d, a, b, in[11], S23,  643717713U); /* 19 */
    GG(b, c, d, a, in[ 0], S24, 3921069994U); /* 20 */
    GG(a, b, c, d, in[ 5], S21, 3593408605U); /* 21 */
    GG(d, a, b, c, in[10], S22,   38016083U); /* 22 */
    GG(c, d, a, b, in[15], S23, 3634488961U); /* 23 */
    GG(b, c, d, a, in[ 4], S24, 3889429448U); /* 24 */
    GG(a, b, c, d, in[ 9], S21,  568446438U); /* 25 */
    GG(d, a, b, c, in[14], S22, 3275163606U); /* 26 */
    GG(c, d, a, b, in[ 3], S23, 4107603335U); /* 27 */
    GG(b, c, d, a, in[ 8], S24, 1163531501U); /* 28 */
    GG(a, b, c, d, in[13], S21, 2850285829U); /* 29 */
    GG(d, a, b, c, in[ 2], S22, 4243563512U); /* 30 */
    GG(c, d, a, b, in[ 7], S23, 1735328473U); /* 31 */
    GG(b, c, d, a, in[12], S24, 2368359562U); /* 32 */

    /* Round 3 */
    HH(a, b, c, d, in[ 5], S31, 4294588738U); /* 33 */
    HH(d, a, b, c, in[ 8], S32, 2272392833U); /* 34 */
    HH(c, d, a, b, in[11], S33, 1839030562U); /* 35 */
    HH(b, c, d, a, in[14], S34, 4259657740U); /* 36 */
    HH(a, b, c, d, in[ 1], S31, 2763975236U); /* 37 */
    HH(d, a, b, c, in[ 4], S32, 1272893353U); /* 38 */
    HH(c, d, a, b, in[ 7], S33, 4139469664U); /* 39 */
    HH(b, c, d, a, in[10], S34, 3200236656U); /* 40 */
    HH(a, b, c, d, in[13], S31,  681279174U); /* 41 */
    HH(d, a, b, c, in[ 0], S32, 3936430074U); /* 42 */
    HH(c, d, a, b, in[ 3], S33, 3572445317U); /* 43 */
    HH(b, c, d, a, in[ 6], S34,   76029189U); /* 44 */
    HH(a, b, c, d, in[ 9], S31, 3654602809U); /* 45 */
    HH(d, a, b, c, in[12], S32, 3873151461U); /* 46 */
    HH(c, d, a, b, in[15], S33,  530742520U); /* 47 */
    HH(b, c, d, a, in[ 2], S34, 3299628645U); /* 48 */

    /* Round 4 */
    II(a, b, c, d, in[ 0], S41, 4096336452U); /* 49 */
    II(d, a, b, c, in[ 7], S42, 1126891415U); /* 50 */
    II(c, d, a, b, in[14], S43, 2878612391U); /* 51 */
    II(b, c, d, a, in[ 5], S44, 4237533241U); /* 52 */
    II(a, b, c, d, in[12], S41, 1700485571U); /* 53 */
    II(d, a, b, c, in[ 3], S42, 2399980690U); /* 54 */
    II(c, d, a, b, in[10], S43, 4293915773U); /* 55 */
    II(b, c, d, a, in[ 1], S44, 2240044497U); /* 56 */
    II(a, b, c, d, in[ 8], S41, 1873313359U); /* 57 */
    II(d, a, b, c, in[15], S42, 4264355552U); /* 58 */
    II(c, d, a, b, in[ 6], S43, 2734768916U); /* 59 */
    II(b, c, d, a, in[13], S44, 1309151649U); /* 60 */
    II(a, b, c, d, in[ 4], S41, 4149444226U); /* 61 */
    II(d, a, b, c, in[11], S42, 3174756917U); /* 62 */
    II(c, d, a, b, in[ 2], S43,  718787259U); /* 63 */
    II(b, c, d, a, in[ 9], S44, 3951481745U); /* 64 */

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

__BEGIN_DECLS
/**
 * The routine MD5Init initializes the message-digest context.
 */
void MD5Init(MD5_CTX* context)
{
    context->state[0] = 0x67452301;
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
    context->count[0] = context->count[1] = 0;
}

/**
 * The routine MD5Update updates the message-digest context to account
 * for the presence of each of the characters data[0..size - 1] in the
 * message whose digest is being computed.
 */
void MD5Update(MD5_CTX* context, const u_char* data, uint32_t size)
{
    /* compute number of bytes mod 64 */
    int mdi = (int)((context->count[0] >> 3) & 0x3F);

    /* update number of bits */
    if ((context->count[0] + (size << 3)) < context->count[0])
        context->count[1]++;

    context->count[0] += (size << 3);
    context->count[1] += (size >> 29);

    while (size--)
    {
        /* add new character to buffer, increment mdi */
        context->buffer[mdi++] = *data++;

        /* transform if necessary */
        uint32_t buf[16];
        if (mdi == 0x40)
        {
            for (uint32_t i = 0, j = 0; i < 16; ++i, j += 4)
                buf[i] = ((((uint32_t)context->buffer[j + 3]) << 24) | (((uint32_t)context->buffer[j + 2]) << 16) | (((uint32_t)context->buffer[j + 1]) << 8) | ((uint32_t)context->buffer[j]));

            MD5Transform(context->state, buf);
            mdi = 0;
        }
    }
}

/**
 * The routine MD5Final terminates the message-digest computation
 * and ends with the desired message digest in digest[0...15].
 */
void MD5Final(MD5_CTX* context, u_char digest[MD5_DIGEST_LENGTH])
{
    uint32_t buf[16];

    /* save number of bits */
    buf[14] = context->count[0];
    buf[15] = context->count[1];

    /* compute number of bytes mod 64 */
    const int mdi = (int)((context->count[0] >> 3) & 0x3F);

    /* pad out to 56 mod 64 */
    ::MD5Update(context, PADDING, (mdi < 56 ? 56 - mdi : 120 - mdi));

    /* append length buf bits and transform */
    for (uint32_t i = 0, j = 0; i < 14; ++i, j += 4)
        buf[i] = ((((uint32_t)context->buffer[j + 3]) << 24) | (((uint32_t)context->buffer[j + 2]) << 16) | (((uint32_t)context->buffer[j + 1]) << 8) | ((uint32_t)context->buffer[j]));

    MD5Transform(context->state, buf);

    /* store message digest in buffer */
    for (uint32_t i = 0, j = 0; i < 4; ++i, j += 4)
    {
        digest[j]     = (u_char)(context->state[i] & 0xFF);
        digest[j + 1] = (u_char)((context->state[i] >> 8) & 0xFF);
        digest[j + 2] = (u_char)((context->state[i] >> 16) & 0xFF);
        digest[j + 3] = (u_char)((context->state[i] >> 24) & 0xFF);
    }
}
__END_DECLS