/**
 * SHA-1 in cpp
 */

#include "md.h"
#include <string.h>
#include "platform.h"

/* Help macros */
#define SHA1_ROL(value, bits)   (((value) << (bits)) | ((value) >> (32 - (bits))))
#define SHA1_BLK(i)             (block[i&15] = SHA1_ROL(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i&15], 1))
#define SHA1_BLK0(i)            (block[i] = (SHA1_ROL(block[i],24) & 0xFF00FF00) | (SHA1_ROL(block[i],8) & 0x00FF00FF))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define SHA1_R0(v,w,x,y,z,i)    z += ((w&(x^y))^y)     + SHA1_BLK0(i) + 0x5A827999 + SHA1_ROL(v,5); w=SHA1_ROL(w,30)
#define SHA1_R1(v,w,x,y,z,i)    z += ((w&(x^y))^y)     + SHA1_BLK(i)  + 0x5A827999 + SHA1_ROL(v,5); w=SHA1_ROL(w,30)
#define SHA1_R2(v,w,x,y,z,i)    z += (w^x^y)           + SHA1_BLK(i)  + 0x6ED9EBA1 + SHA1_ROL(v,5); w=SHA1_ROL(w,30)
#define SHA1_R3(v,w,x,y,z,i)    z += (((w|x)&y)|(w&x)) + SHA1_BLK(i)  + 0x8F1BBCDC + SHA1_ROL(v,5); w=SHA1_ROL(w,30)
#define SHA1_R4(v,w,x,y,z,i)    z += (w^x^y)           + SHA1_BLK(i)  + 0xCA62C1D6 + SHA1_ROL(v,5); w=SHA1_ROL(w,30)

/**
 * Hash a single 512-bit block. This is the core of the algorithm.
 */
static void SHA1Transform(uint32_t state[5], const u_char buffer[64])
{
    /* Copy state[] to working vars */
    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t e = state[4];

    uint32_t block[16];
    ::memcpy(block, buffer, 64);

    /* 4 rounds of 20 operations each. Loop unrolled. */
    SHA1_R0(a,b,c,d,e, 0);
    SHA1_R0(e,a,b,c,d, 1);
    SHA1_R0(d,e,a,b,c, 2);
    SHA1_R0(c,d,e,a,b, 3);
    SHA1_R0(b,c,d,e,a, 4);
    SHA1_R0(a,b,c,d,e, 5);
    SHA1_R0(e,a,b,c,d, 6);
    SHA1_R0(d,e,a,b,c, 7);
    SHA1_R0(c,d,e,a,b, 8);
    SHA1_R0(b,c,d,e,a, 9);
    SHA1_R0(a,b,c,d,e,10);
    SHA1_R0(e,a,b,c,d,11);
    SHA1_R0(d,e,a,b,c,12);
    SHA1_R0(c,d,e,a,b,13);
    SHA1_R0(b,c,d,e,a,14);
    SHA1_R0(a,b,c,d,e,15);
    SHA1_R1(e,a,b,c,d,16);
    SHA1_R1(d,e,a,b,c,17);
    SHA1_R1(c,d,e,a,b,18);
    SHA1_R1(b,c,d,e,a,19);
    SHA1_R2(a,b,c,d,e,20);
    SHA1_R2(e,a,b,c,d,21);
    SHA1_R2(d,e,a,b,c,22);
    SHA1_R2(c,d,e,a,b,23);
    SHA1_R2(b,c,d,e,a,24);
    SHA1_R2(a,b,c,d,e,25);
    SHA1_R2(e,a,b,c,d,26);
    SHA1_R2(d,e,a,b,c,27);
    SHA1_R2(c,d,e,a,b,28);
    SHA1_R2(b,c,d,e,a,29);
    SHA1_R2(a,b,c,d,e,30);
    SHA1_R2(e,a,b,c,d,31);
    SHA1_R2(d,e,a,b,c,32);
    SHA1_R2(c,d,e,a,b,33);
    SHA1_R2(b,c,d,e,a,34);
    SHA1_R2(a,b,c,d,e,35);
    SHA1_R2(e,a,b,c,d,36);
    SHA1_R2(d,e,a,b,c,37);
    SHA1_R2(c,d,e,a,b,38);
    SHA1_R2(b,c,d,e,a,39);
    SHA1_R3(a,b,c,d,e,40);
    SHA1_R3(e,a,b,c,d,41);
    SHA1_R3(d,e,a,b,c,42);
    SHA1_R3(c,d,e,a,b,43);
    SHA1_R3(b,c,d,e,a,44);
    SHA1_R3(a,b,c,d,e,45);
    SHA1_R3(e,a,b,c,d,46);
    SHA1_R3(d,e,a,b,c,47);
    SHA1_R3(c,d,e,a,b,48);
    SHA1_R3(b,c,d,e,a,49);
    SHA1_R3(a,b,c,d,e,50);
    SHA1_R3(e,a,b,c,d,51);
    SHA1_R3(d,e,a,b,c,52);
    SHA1_R3(c,d,e,a,b,53);
    SHA1_R3(b,c,d,e,a,54);
    SHA1_R3(a,b,c,d,e,55);
    SHA1_R3(e,a,b,c,d,56);
    SHA1_R3(d,e,a,b,c,57);
    SHA1_R3(c,d,e,a,b,58);
    SHA1_R3(b,c,d,e,a,59);
    SHA1_R4(a,b,c,d,e,60);
    SHA1_R4(e,a,b,c,d,61);
    SHA1_R4(d,e,a,b,c,62);
    SHA1_R4(c,d,e,a,b,63);
    SHA1_R4(b,c,d,e,a,64);
    SHA1_R4(a,b,c,d,e,65);
    SHA1_R4(e,a,b,c,d,66);
    SHA1_R4(d,e,a,b,c,67);
    SHA1_R4(c,d,e,a,b,68);
    SHA1_R4(b,c,d,e,a,69);
    SHA1_R4(a,b,c,d,e,70);
    SHA1_R4(e,a,b,c,d,71);
    SHA1_R4(d,e,a,b,c,72);
    SHA1_R4(c,d,e,a,b,73);
    SHA1_R4(b,c,d,e,a,74);
    SHA1_R4(a,b,c,d,e,75);
    SHA1_R4(e,a,b,c,d,76);
    SHA1_R4(d,e,a,b,c,77);
    SHA1_R4(c,d,e,a,b,78);
    SHA1_R4(b,c,d,e,a,79);

    /* Add the working vars back into state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

__BEGIN_DECLS
/**
 * SHA1Init - Initialize new context
 */
void SHA1Init(SHA1_CTX* context)
{
    LOGD("SHA1Init context = %p\n", context);
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->count[0] = context->count[1] = 0;
}

/**
 * Run your data through this.
 */
void SHA1Update(SHA1_CTX* context, const u_char* data, uint32_t size)
{
    uint32_t i, j = context->count[0];
    if ((context->count[0] += size << 3) < j)
        context->count[1] += (size >> 29) + 1;

    j = (j >> 3) & 63;
    if ((j + size) > 63) {
        memcpy(&context->buffer[j], data, (i = 64 - j));
        SHA1Transform(context->state, context->buffer);
        for ( ; i + 63 < size; i += 64)
            SHA1Transform(context->state, &data[i]);
        j = 0;
    } else {
        i = 0;
    }

    memcpy(&context->buffer[j], &data[i], size - i);
}

/**
 * Add padding and return the message digest.
 */
void SHA1Final(SHA1_CTX* context, u_char digest[SHA1_DIGEST_LENGTH])
{
    u_char data[8];
    for (int32_t i = 0; i < 8; ++i)    /* Endian independent */
        data[i] = (u_char)((context->count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 255);

    SHA1Update(context, (const u_char*)"\200", 1);
    while ((context->count[0] & 504) != 448)
        SHA1Update(context, (const u_char*)"\0", 1);

    SHA1Update(context, data, 8);   /* Should cause a SHA1Transform() */
    for (int32_t i = 0; i < SHA1_DIGEST_LENGTH; ++i)
        digest[i] = (u_char)((context->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
}
__END_DECLS