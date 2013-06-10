void aes_enc_ecb( unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void aes_dec_ecb( unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void aes_enc_cbc( unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void aes_dec_cbc( unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void aes_enc_cfb( unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void aes_dec_cfb( unsigned char *iv, unsigned char *clear, unsigned char *cipher, unsigned int blocks );
void aes_enc_ofb( const byte *iv, const byte *clear, byte *cipher, int blocks );
void aes_dec_ofb( const byte *iv, byte *cipher, byte *clear, int blocks );
