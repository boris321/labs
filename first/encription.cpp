#include "des.h"
#include "gost.h"
#include "aes.h"
#include "md5.h"
#include "gosthash.h"
#include "sha1.h"
#include "ripemd.h"
#include <QString>
#include <fstream>
#include <string>

using namespace std;

#define size_max 2048
#define filename_r "result"

void DES( const QString filename_i, const QString filename_k, bool enc, const QString mode, const QString filename_v )
{
    unsigned char i[size_max] = {0};
    unsigned char o[size_max] = {0};
    unsigned char k[8] = {0};
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

    fread( k, 1, 8, file_k );
    des_key_schedule schedule;
    des_set_key_unchecked(&k, schedule);

    while ( ( size = fread(i, 1, size_max, file_i ) )) {
        blocks = size / 8;
        if ( size % 8  > 0 ) blocks++;
        if ( enc ) {
            if ( mode == "ECB" ) des_enc_ecb( schedule, i, o, blocks );
            if ( mode == "CBC" ) des_enc_cbc( schedule, v, i, o, blocks );
            if ( mode == "CFB" ) des_enc_cfb( schedule, v, i, o, blocks );
            if ( mode == "OFB" ) des_enc_ofb( schedule, v, i, o, blocks );
            fwrite( o, 1, 8 * blocks, file_o );
            for ( j = 0; j < size_max; j++ ) i[j] = NULL;
        }
        else {
            if ( mode == "ECB" ) des_dec_ecb( schedule, i, o, blocks );
            if ( mode == "CBC" ) des_dec_cbc( schedule, v, i, o, blocks );
            if ( mode == "CFB" ) des_dec_cfb( schedule, v, i, o, blocks );
            if ( mode == "OFB" ) des_dec_ofb( schedule, v, i, o, blocks );
            for ( j = 0; o[j] && j < size_max ; j++ );
            fwrite( o, 1, j, file_o);
        }
    }

    fclose(file_i);
    fclose(file_k);
    fclose(file_o);
}

void GOST( const QString filename_i, const QString filename_k, bool enc, const QString mode, const QString filename_v )
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

    fread( k, 1, 32, file_k );
    gost_ctx ctx;
    gost_subst_block *subst_block;
    subst_block = &GostR3411_94_TestParamSet;

    gost_init(&ctx, subst_block);
    gost_key(&ctx, k);

    while ( ( size = fread(i, 1, size_max, file_i ) ) ) {
        blocks = size / 8;
        if ( size % 8  > 0 ) blocks++;
        if ( enc ) {
            if ( mode == "ECB" ) gost_enc_ecb( &ctx, i, o, blocks );
            if ( mode == "CBC" ) gost_enc_cbc( &ctx, v, i, o, blocks );
            if ( mode == "CFB" ) gost_enc_cfb( &ctx, v, i, o, blocks );
            if ( mode == "OFB" ) gost_enc_ofb( &ctx, v, i, o, blocks );
            if ( mode == QString::fromLocal8Bit( "Имитовставка" ) ) gost_mac( &ctx, 32, i, size, o );
            fwrite( o, 1, 8 * blocks, file_o );
            for ( j = 0; j < size_max; j++ ) i[j] = NULL;
            if ( mode == QString::fromLocal8Bit( "Имитовставка" ) ) {
                fclose( file_o );
                file_o = fopen( filename_r, "wb" );
                fwrite( o, 1, 4, file_o );
            }
        }
        else {
            if ( mode == "ECB" ) gost_dec_ecb( &ctx, i, o, blocks );
            if ( mode == "CBC" ) gost_dec_cbc( &ctx, v, i, o, blocks );
            if ( mode == "CFB" ) gost_dec_cfb( &ctx, v, i, o, blocks );
            if ( mode == "OFB" ) gost_dec_ofb( &ctx, v, i, o, blocks );
            for ( j = 0; o[j] && j < size_max ; j++ );
            fwrite( o, 1, j, file_o );
        }
    }

    fclose( file_i );
    fclose( file_o );
    fclose( file_k );
    fclose( file_v );
}

void AES(const QString filename_i, const QString filename_k, bool enc, QString mode, const QString filename_v )
{
    unsigned char i[size_max] = {0};
    unsigned char o[size_max] = {0};
    extern unsigned char Key[32];
    unsigned char v[16] = {0};

    FILE *file_i, *file_o, *file_k, *file_v;

    file_i = fopen( filename_i.toUtf8().constData(), "rb" );
    file_o = fopen( filename_r, "wb" );
    file_k = fopen( filename_k.toUtf8().constData(), "rb" );
    if ( ( file_v = fopen( filename_v.toUtf8().constData(), "rb" ) )) {
        fread( v, 1, 16, file_v );
    }

    unsigned int j;
    unsigned int size;
    unsigned int blocks;

    extern int Nr;
    extern int Nk;
    extern void KeyExpansion();
    if (mode.mid( 3, 3 ) == "128") Nr = 128;
    if (mode.mid( 3, 3 ) == "192") Nr = 192;
    if (mode.mid( 3, 3 ) == "256") Nr = 256;
    mode = mode.mid( 0, 3);
    Nk = Nr / 32;
    Nr = Nk + 6;

    fread( Key, 1, Nk * 4, file_k );
    KeyExpansion();

    while ( ( size = fread(i, 1, size_max, file_i ) ) ) {
        blocks = size / 16;
        if ( size % 16  > 0 ) blocks++;
        if ( enc ) {
            if ( mode == "ECB" ) aes_enc_ecb( i, o, blocks );
            if ( mode == "CBC" ) aes_enc_cbc( v, i, o, blocks );
            if ( mode == "CFB" ) aes_enc_cfb( v, i, o, blocks );
            if ( mode == "OFB" ) aes_enc_ofb( v, i, o, blocks );
            fwrite( o, 1, 16 * blocks, file_o );
            for ( j = 0; j < size_max; j++ ) i[j] = NULL;
        }
        else {
            if ( mode == "ECB" ) aes_dec_ecb( i, o, blocks );
            if ( mode == "CBC" ) aes_dec_cbc( v, i, o, blocks );
            if ( mode == "CFB" ) aes_dec_cfb( v, i, o, blocks );
            if ( mode == "OFB" ) aes_dec_ofb( v, i, o, blocks );
            for ( j = 0; o[j] && j < size_max ; j++ );
            fwrite( o, 1, j, file_o );
        }
    }

    fclose(file_i);
    fclose(file_o);
    fclose(file_k);
    fclose(file_v);
}

void MD5( const QString filename_i )
{
    FILE *file_i, *file_o;

    file_i = fopen( filename_i.toUtf8().constData(), "rb" );
    file_o = fopen( filename_r, "wb" );

    MD5_CTX c;
    unsigned char i[size_max] = {0};
    unsigned char o[16];

    unsigned int size = fread(i, 1, size_max, file_i);

    MD5_Init(&c);
    MD5_Update(&c, i, size);
    MD5_Final(o, &c);

    fwrite( o, 1, 16, file_o);

    fclose(file_i);
    fclose(file_o);
}

void GOSTHASH( const QString filename_i )
{

    FILE *file_i, *file_o;

    file_i = fopen( filename_i.toUtf8().constData(), "rb" );
    file_o = fopen( filename_r, "wb" );

    gost_hash_ctx ctx;
    gost_subst_block *subst_block = &GostR3411_94_TestParamSet;
    byte i[size_max] = {0};
    unsigned char o[32];

    unsigned int size = fread(i, 1, size_max, file_i);

    init_gost_hash_ctx(&ctx, subst_block);
    start_hash(&ctx);
    hash_block(&ctx, i, size);
    finish_hash(&ctx, o);
    done_gost_hash_ctx(&ctx);

    fwrite( o, 1, 32, file_o);

    fclose(file_i);
    fclose(file_o);
}

void SHA1( const QString filename_i )
{
    FILE *file_i, *file_o;

    file_i = fopen( filename_i.toUtf8().constData(), "rb" );
    file_o = fopen( filename_r, "wb" );

    unsigned char i[size_max] = {0};
    unsigned char o[40];

    unsigned int size = fread(i, 1, size_max, file_i);

    // хэширование
    string istr, ostr;
    istr.resize( size );
    ostr.resize( 0 );
    unsigned int j;
    for ( j = 0; j < size; j++ ) {
        istr[j] = i[j];
    }
    ostr = SHA1hash( istr );
    for ( j = 0; j < 40; j++ ) {
        o[j] = ostr[j];
    }

    fwrite( o, 1, 40, file_o);

    fclose(file_i);
    fclose(file_o);
}

void RIPEMD( const QString filename_i )
{
    FILE *file_i, *file_o;

    file_i = fopen( filename_i.toUtf8().constData(), "rb" );
    file_o = fopen( filename_r, "wb" );

    unsigned char i[size_max] = {0};
    unsigned int size = fread(i, 1, size_max, file_i);

    RIPEMD160 ripemd160;
    ripemd160.compress(i, size);

    fwrite( ripemd160.hash, 1, 20, file_o);

    fclose(file_i);
    fclose(file_o);
}
