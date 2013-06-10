#define DES_LONG unsigned long

#define DES_ENCRYPT 1
#define DES_DECRYPT 0

typedef unsigned char des_cblock[8];
typedef /* const */ unsigned char const_des_cblock[8];

typedef struct des_ks_struct
{ union
  { des_cblock cblock;
    DES_LONG deslong[2];
  } ks;
  int weak_key;
} des_key_schedule[16];

void des_set_odd_parity(des_cblock *key);
void descrypt(des_key_schedule ctx, const unsigned char *in, unsigned char *out);
void desdecrypt(des_key_schedule ctx, const unsigned char *in, unsigned char *out);
void des_ecb_encrypt(const_des_cblock *input,des_cblock *output,des_key_schedule ks,int enc);
void des_enc_ecb( des_key_schedule ctx, const unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void des_dec_ecb( des_key_schedule ctx, const unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void des_enc_cbc( des_key_schedule ctx, const unsigned char *iv, const unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void des_dec_cbc( des_key_schedule ctx, const unsigned char *iv, const unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void des_enc_cfb( des_key_schedule ctx, const unsigned char *iv, const unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void des_dec_cfb( des_key_schedule ctx, const unsigned char *iv, const unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void des_enc_ofb( des_key_schedule ctx, const unsigned char *iv, const unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void des_dec_ofb( des_key_schedule ctx, const unsigned char *iv, const unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void des_set_key_unchecked(const_des_cblock *key,des_key_schedule schedule);
