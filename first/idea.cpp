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

// нахождение обратного элемента в кольце по модулю 2^16 + 1
static uint16 mulInv( uint16 x ) {
    uint16 t0, t1;
    uint16 q, y;
    if ( x <= 1 ) return x;    /* 0 and 1 are self-inverse */
    t1 = 0x10001L / x;  /* Since x >= 2, this fits into 16 bits */
    y = 0x10001L % x;
    if ( y == 1 ) return low16( 1 - t1 );
    t0 = 1;
    do {
        q = x / y;
        x = x % y;
        t0 += q * t1;
        if ( x == 1 ) return t0;
        q = y / x;
        y = y % x;
        t1 += q * t0;
    }
    while ( y != 1 );
    return low16( 1 - t1 );
} /* mukInv */

// генерация ключей для шифрирования
static void ideaExpandKey(byte const *userkey, word16 *EK)
{
    int i,j;
    for ( j = 0; j < 8; j++ ) {
        EK[j] = ( userkey[0] << 8 ) + userkey[1];
        userkey += 2;
    }
    for ( i = 0; j < IDEAKEYLEN; j++) {
        i++;
        EK[i + 7] = EK[i & 7] << 9 | EK[( i + 1 ) & 7] >> 7;
        EK += i & 8;
        i &= 7;
    }
} /* ideaExpandKey */

// генерация ключей для дешифрирования
static void ideaInvertKey( word16 const *EK, word16 DK[IDEAKEYLEN] )
{
    int i;
    uint16 t1, t2, t3;
    word16 temp[IDEAKEYLEN];
    word16 *p = temp + IDEAKEYLEN;
    t1 = mulInv( *EK++ );
    t2 = -*EK++;
    t3 = -*EK++;
    *--p = mulInv( *EK++ );
    *--p = t3;
    *--p = t2;
    *--p = t1;
    for ( i = 0; i < IDEAROUNDS - 1; i++ ) {
        t1 = *EK++;
        *--p = *EK++;
        *--p = t1;
        t1 = mulInv( *EK++ );
        t2 = -*EK++;
        t3 = -*EK++;
        *--p = mulInv( *EK++ );
        *--p = t2;
        *--p = t3;
        *--p = t1;
    }
    t1 = *EK++;
    *--p = *EK++;
    *--p = t1;
    t1 = mulInv( *EK++ );
    t2 = -*EK++;
    t3 = -*EK++;
    *--p = mulInv( *EK++ );
    *--p = t3;
    *--p = t2;
    *--p = t1; /* копируеи и разрушаем temp */
    memcpy( DK, temp, sizeof( temp ) );
    for ( i = 0; i < IDEAKEYLEN; i++ ) {
        temp[i] = 0;
    }

} /* ideaInvertKey */

static void ideaCipher( byte *inbuf, byte *outbuf, word16 *key )
{
    uint16 x1, x2, x3, x4, s2, s3;
    word16 *in, *out;

    // временные переменные для умн ожения по модулю 2^16 + 1
    uint16 t16;
    word32 t32;

    // количество раундов
    int r = IDEAROUNDS;

    // Исходный незашифрованный блок делится на четыре подблока по 16 бит каждый
    in = (word16 *)inbuf;
    x1 = *in++;
    x2 = *in++;
    x3 = *in++;
    x4 = *in;

    x1 = ( x1 >> 8 ) | ( x1 << 8 );
    x2 = ( x2 >> 8 ) | ( x2 << 8 );
    x3 = ( x3 >> 8 ) | ( x3 << 8 );
    x4 = ( x4 >> 8 ) | ( x4 << 8 );

    // 8 одинаковых раундов
    do {
        // умножение по модулю 2^16 + 1
        MUL( x1, *key++ );
        // сложение по модулю 2^16
        x2 += *key++;
        // сложение по модулю 2^16
        x3 += *key++;
        // умножение по модулю 2^16 + 1
        MUL( x4, *key++ );

        // побитовое исключающее ИЛИ
        s3 = x3;
        x3 ^= x1;
        // умножение по модулю 2^16 + 1
        MUL( x3, *key++ );
        // побитовое исключающее ИЛИ
        s2 = x2;
        x2 ^= x4;
        // сложение по модулю 2^16
        x2 += x3;
        // умножение по модулю 2^16 + 1
        MUL( x2, *key++ );
        // сложение по модулю 2^16
        x3 += x2;

        // побитовое исключающее ИЛИ
        x1 ^= x2;   x4 ^= x3;
        x2 ^= s3;   x3 ^= s2;
    }
    while (--r);

    // выходное преобразование
    // умножение по модулю 2^16 + 1
    MUL( x1, *key++ );
    // сложение по модулю 2^16
    x3 += *key++;
    x2 += *key++;
    // умножение по модулю 2^16 + 1
    MUL( x4, *key );

    out = (word16 *)outbuf;

    *out++ = ( x1 >> 8 ) | ( x1 << 8 );
    *out++ = ( x3 >> 8 ) | ( x3 << 8 );
    *out++ = ( x2 >> 8 ) | ( x2 << 8 );
    *out = ( x4 >> 8 ) | ( x4 <<  8 );

} /* ideaCipher */


#define sb 8

void idea_key(idea_ctx *c, unsigned char *key){
        ideaExpandKey(key, c->ek);
        ideaInvertKey(c->ek, c->dk);
}

void ideacrypt( idea_ctx *ctx, unsigned char *data, unsigned char *cipher )
{
    unsigned char *d = data;
    ideaCipher( d, cipher, ctx->ek );
}

void ideadecrypt( idea_ctx *ctx, unsigned char *data, unsigned char *cipher )
{
    unsigned char *d = data;
    ideaCipher( d, cipher, ctx->dk );
}

/* IDEA зашифровка в ECB режиме */
void idea_enc_ecb( idea_ctx *ctx, unsigned char *clear, unsigned char *cipher, unsigned int blocks )
{
    unsigned int i;
    for ( i = 0; i < blocks; i++ ) {
        ideacrypt( ctx, clear, cipher );
        clear += 8;
        cipher += 8;
    }
}

/* IDEA расшифровка в ECB режиме */
void idea_dec_ecb( idea_ctx *ctx, unsigned char *cipher, unsigned char *clear, unsigned int blocks)
{
    unsigned int i;
    for ( i = 0; i < blocks; i++ ) {
        ideadecrypt( ctx, cipher, clear );
        clear += 8;
        cipher += 8;
    }
}

/* IDEA зашифровка в CBC режиме */
void idea_enc_cbc( idea_ctx *ctx, unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks )
{
    unsigned char cur_iv[8];
    unsigned char gamma[8];
    unsigned int i, j;
    const unsigned char *in;
    unsigned char *out;
    memcpy( cur_iv, iv, 8);
    for ( i = 0, in = clear, out = cipher; i < blocks; i++, in += 8, out += 8 ) {
        for ( j = 0; j < 8; j++ ) {
            cur_iv[j] = cur_iv[j] ^ in[j];
        }
        ideacrypt( ctx, cur_iv, gamma );
        for ( j = 0; j < 8; j++ ) {
            out[j] = cur_iv[j] = gamma[j];
        }
    }
}

/* IDEA расшифровка в CBC режиме */
void idea_dec_cbc( idea_ctx *ctx, unsigned char *iv, unsigned char *cipher, unsigned char *clear, unsigned int blocks )
{
    unsigned char cur_iv[8];
    unsigned char gamma[8];
    unsigned int i, j;
    unsigned char *in;
    unsigned char *out;
    memcpy( cur_iv, iv, 8);
    for ( i = 0, in = cipher, out = clear; i < blocks; i++, in += 8, out+=8 ) {
        ideadecrypt( ctx, in, gamma );
        for ( j = 0; j < 8; j++ ) {
            out[j] = cur_iv[j] ^ gamma[j];
            cur_iv[j] = in[j];
        }
    }
}

/* IDEA зашифровка в CFB режиме */
void idea_enc_cfb( idea_ctx *ctx, unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks )
{
    unsigned char cur_iv[8];
    unsigned char gamma[8];
    unsigned int i, j;
    const unsigned char *in;
    unsigned char *out;
    memcpy( cur_iv, iv, 8);
    for ( i = 0, in = clear, out = cipher; i < blocks; i++, in += 8, out += 8 ) {
        ideacrypt( ctx, cur_iv, gamma );
        for ( j = 0; j < 8; j++ ) {
            cur_iv[j] = out[j] = in[j] ^ gamma[j];
        }
    }
}

/* DES расшифровка в CFB режиме */
void idea_dec_cfb( idea_ctx *ctx, unsigned char *iv, unsigned char *cipher, unsigned char *clear, unsigned int blocks )
{
    unsigned char cur_iv[8];
    unsigned char gamma[8];
    unsigned int i, j;
    const unsigned char *in;
    unsigned char *out;
    memcpy( cur_iv, iv, 8);
    for ( i = 0, in = cipher, out = clear; i < blocks; i++, in += 8, out += 8 ) {
        ideacrypt( ctx, cur_iv, gamma );
        for ( j = 0; j < 8; j++ ) {
            out[j] = ( cur_iv[j] = in[j] ) ^ gamma[j];
        }
    }
}

/* Зашифровка в OFB режиме */
void idea_enc_ofb( idea_ctx *ctx, unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks )
{
    unsigned char cur_iv[8];
    unsigned char gamma[8];
    unsigned int i, j;
    const unsigned char *in;
    unsigned char *out;
    memcpy( cur_iv, iv, 8);
    for ( i = 0, in = clear, out = cipher; i < blocks; i++, in += sb, out += sb ) {
        ideacrypt( ctx, cur_iv, gamma );
        for ( j = 0; j < sb; j++ ) {
            out[j] = in[j] ^ gamma[j];
        }
    }
}

/* Расшифровка в OFB режиме */
void idea_dec_ofb( idea_ctx *ctx, unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks )
{
    unsigned char cur_iv[8];
    unsigned char gamma[8];
    unsigned int i, j;
    const unsigned char *in;
    unsigned char *out;
    memcpy( cur_iv, iv, 8);
    for ( i = 0, in = clear, out = cipher; i < blocks; i++, in += sb, out += sb ) {
        ideacrypt( ctx, cur_iv, gamma );
        for ( j = 0; j < sb; j++ ) {
            out[j] = in[j] ^ gamma[j];
        }
    }
}

void IDEA( const QString filename_i, const QString filename_k, bool enc, const QString mode, const QString filename_v )
{
    unsigned char i[size_max] = {0};
    unsigned char o[size_max] = {0};
    unsigned char k[32] = {0};
    unsigned char v[8] = {0};

    FILE *file_i, *file_o, *file_k, *file_v;

    file_i = fopen( filename_i.toUtf8().constData(), "rb" );
    file_o = fopen( filename_r, "wb" );
    file_k = fopen( filename_k.toUtf8().constData(), "rb" );
    if ( ( file_v = fopen( filename_v.toUtf8().constData(), "rb" ) )) {
        fread( v, 1, 8, file_v );
    }

    unsigned int j;
    unsigned int size;
    unsigned int blocks;

    fread( k, 1, 16, file_k );
    idea_ctx ctx;
    idea_key( &ctx, k );

    while ( ( size = fread(i, 1, size_max, file_i ) )) {
        blocks = size / 8;
        if ( size % 8  > 0 ) blocks++;
        if ( enc ) {
            if ( mode == "ECB" ) idea_enc_ecb( &ctx, i, o, blocks );
            if ( mode == "CBC" ) idea_enc_cbc( &ctx, v, i, o, blocks );
            if ( mode == "CFB" ) idea_enc_cfb( &ctx, v, i, o, blocks );
            if ( mode == "OFB" ) idea_enc_ofb( &ctx, v, i, o, blocks );
            fwrite( o, 1, 8 * blocks, file_o );
            for ( j = 0; j < size_max; j++ ) i[j] = NULL;
        }
        else {
            if ( mode == "ECB" ) idea_dec_ecb( &ctx, i, o, blocks );
            if ( mode == "CBC" ) idea_dec_cbc( &ctx, v, i, o, blocks );
            if ( mode == "CFB" ) idea_dec_cfb( &ctx, v, i, o, blocks );
            if ( mode == "OFB" ) idea_dec_ofb( &ctx, v, i, o, blocks );
            for ( j = 0; o[j] && j < size_max ; j++ );
            fwrite( o, 1, j, file_o);
        }
    }

    fclose(file_i);
    fclose(file_o);
    fclose(file_k);
    fclose(file_v);
}
