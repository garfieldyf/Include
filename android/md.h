/**
 * Message-Digest in C
 */

#ifndef _MSGDIGEST_INC_
#define	_MSGDIGEST_INC_

#include <sys/cdefs.h>
#include <sys/types.h>

#define MD5_DIGEST_LENGTH               16
#define MD5_DIGEST_STRING_LENGTH        33

#define SHA1_DIGEST_LENGTH              20
#define SHA1_DIGEST_STRING_LENGTH       41

#define SHA256_DIGEST_LENGTH            32
#define SHA256_DIGEST_STRING_LENGTH     65

#define SHA384_DIGEST_LENGTH            48
#define SHA384_DIGEST_STRING_LENGTH     97

#define SHA512_DIGEST_LENGTH            64
#define SHA512_DIGEST_STRING_LENGTH     129

/* Data structure for MD5 computation */
typedef struct {
    uint32_t state[4];
    uint32_t count[2];
    u_char buffer[64];
} MD5_CTX;

/* Data structure for SHA-1 computation */
typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    u_char buffer[64];
} SHA1_CTX;

/* Data structure for SHA-256 computation */
typedef struct {
    uint32_t state[8];
    uint32_t count[2];
    u_char buffer[128];
} SHA256_CTX;

/* Data structure for SHA-512 computation */
typedef struct {
    uint64_t state[8];
    uint32_t count[2];
    u_char buffer[256];
} SHA512_CTX;


__BEGIN_DECLS
void MD5Init(MD5_CTX *);
void MD5Update(MD5_CTX *, const u_char *, uint32_t);
void MD5Final(MD5_CTX *, u_char[MD5_DIGEST_LENGTH]);

void SHA1Init(SHA1_CTX *);
void SHA1Update(SHA1_CTX *, const u_char *, uint32_t);
void SHA1Final(SHA1_CTX *, u_char[SHA1_DIGEST_LENGTH]);

void SHA256Init(SHA256_CTX *);
void SHA256Update(SHA256_CTX *, const u_char *, uint32_t);
void SHA256Final(SHA256_CTX *, u_char[SHA256_DIGEST_LENGTH]);

void SHA384Init(SHA512_CTX *);
void SHA384Final(SHA512_CTX *, u_char[SHA384_DIGEST_LENGTH]);

void SHA512Init(SHA512_CTX *);
void SHA512Update(SHA512_CTX *, const u_char *, uint32_t);
void SHA512Final(SHA512_CTX *, u_char[SHA512_DIGEST_LENGTH]);
__END_DECLS

#endif /* _MSGDIGEST_INC_ */