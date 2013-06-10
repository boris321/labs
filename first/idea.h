#ifndef IDEA_H
#define IDEA_H

#include <QString>
#include <stdio.h>

#include <string.h>
typedef unsigned char boolean;     /* values are TRUE or FALSE */
typedef unsigned char byte; /* values are 0-255 */
typedef byte *byteptr;     /* pointer to byte */
typedef char *string;/* pointer to ASCII character string */
typedef unsigned short word16;     /* values are 0-65535 */
typedef unsigned long word32;      /* values are 0-4294967295 */

#ifndef TRUE
        #define FALSE 0
        #define TRUE (!FALSE)
#endif /* if TRUE not already defined */

#define IDEAKEYSIZE 16 #define IDEABLOCKSIZE 8
#define IDEAROUNDS 8
#define IDEAKEYLEN (6*IDEAROUNDS+4)

typedef struct{
        word16 ek[IDEAKEYLEN],
        dk[IDEAKEYLEN];
}idea_ctx;

/* End includes for IDEA.C */
#ifdef IDEA32       /* Use >16-bit temporaries */
        #define low16(x) ((x) & 0xFFFF)
        typedef unsigned int uint16;/* at LEAST 16 bits, maybe more */
#else
        #define low16(x) (x) /* this is only ever applied to uint16's */
        typedef word16 uint16;
#endif

// умножение по модулю 2^16 + 1
#define MUL(x,y) \
    ((t16 = (y)) ? \
        (x=low16(x)) ? \
            t32 = (word32)x*t16, \
            x = low16(t32), \
            t16 = t32>>16, \
            x = (x-t16)+(x<t16) \
        : \
    (x = 1-t16) \
    : \
    (x = 1-x))

#define size_max 2048
#define filename_r "result"

void idea_key(idea_ctx *c, unsigned char *key);
void idea_enc_ecb( idea_ctx *ctx, unsigned char *cipher, unsigned char *clear, unsigned int blocks);
void idea_dec_ecb( idea_ctx *ctx, unsigned char *cipher, unsigned char *clear, unsigned int blocks);
void idea_enc_cbc( idea_ctx *ctx, unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void idea_dec_cbc( idea_ctx *ctx, unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void idea_enc_cfb( idea_ctx *ctx, unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void idea_dec_cfb( idea_ctx *ctx, unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void idea_enc_ofb( idea_ctx *ctx, unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void idea_dec_ofb( idea_ctx *ctx, unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks );

#endif // IDEA_H
