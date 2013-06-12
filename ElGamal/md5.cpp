#include <stdlib.h>
#include <string.h>

#include "md5.h"

#define ULONG unsigned long
#define UCHAR unsigned char
#define UINT unsigned int

#undef c2l
#define c2l(c,l) \
( l = ((unsigned long)(*((c)++))), \
  l |= (((unsigned long)(*((c)++)))<<8), \
  l |= (((unsigned long)(*((c)++)))<<16), \
  l |= (((unsigned long)(*((c)++)))<<24) )

#undef p_c2l
#define p_c2l(c,l,n) \
{ switch ( n ) \
  { case 0: l =((unsigned long)(*((c)++))); \
    case 1: l|=((unsigned long)(*((c)++)))<<8; \
    case 2: l|=((unsigned long)(*((c)++)))<<16; \
    case 3: l|=((unsigned long)(*((c)++)))<<24; \
  } \
}

// NOTE the pointer is not incremented at the end of this
#undef c2l_p
#define c2l_p(c,l,n) \
{ l = 0; \
  (c) += n; \
  switch ( n ) \
  { case 3: l =((unsigned long)(*(--(c))))<<16; \
    case 2: l|=((unsigned long)(*(--(c))))<<8; \
    case 1: l|=((unsigned long)(*(--(c)))); \
  } \
}

#undef p_c2l_p
#define p_c2l_p(c,l,sc,len) \
{ switch ( sc ) \
  { case 0: l = ((unsigned long)(*((c)++))); \
      if ( --len == 0 ) break; \
    case 1: l |= ((unsigned long)(*((c)++)))<<8; \
      if ( --len == 0 ) break; \
    case 2: l |= ((unsigned long)(*((c)++)))<<16; \
  } \
}

#undef l2c
#define l2c(l,c) \
( *((c)++)=(unsigned char)(((l))&0xff), \
  *((c)++)=(unsigned char)(((l)>>8)&0xff), \
  *((c)++)=(unsigned char)(((l)>>16)&0xff), \
  *((c)++)=(unsigned char)(((l)>>24)&0xff) )

// NOTE - c is not incremented as per l2c
#undef l2cn
#define l2cn(l1,l2,c,n) \
{ c += n; \
  switch ( n ) \
  { case 8: *(--(c)) = (unsigned char)(((l2)>>24)&0xff); \
    case 7: *(--(c)) = (unsigned char)(((l2)>>16)&0xff); \
    case 6: *(--(c)) = (unsigned char)(((l2)>>8)&0xff); \
    case 5: *(--(c)) = (unsigned char)(((l2))&0xff); \
    case 4: *(--(c)) = (unsigned char)(((l1)>>24)&0xff); \
    case 3: *(--(c)) = (unsigned char)(((l1)>>16)&0xff); \
    case 2: *(--(c)) = (unsigned char)(((l1)>>8)&0xff); \
    case 1: *(--(c)) = (unsigned char)(((l1))&0xff); \
  } \
}

/* A nice byte order reversal from Wei Dai <weidai@eskimo.com> */
#if defined(WIN32)
/* 5 instructions with rotate instruction, else 9 */
#define Endian_Reverse32(a) \
{ unsigned long l = (a); \
  (a) = ((ROTATE(l,8)&0x00FF00FF)|(ROTATE(l,24)&0xFF00FF00)); \
}
#else
/* 6 instructions with rotate instruction, else 8 */
#define Endian_Reverse32(a) \
{ unsigned long l = (a); \
  l = (((l&0xFF00FF00)>>8L)|((l&0x00FF00FF)<<8L)); \
  (a) = ROTATE(l,16L); \
}
#endif
/*
#define  F(x,y,z)  (((x) & (y))  |  ((~(x)) & (z)))
#define  G(x,y,z)  (((x) & (z))  |  ((y) & (~(z))))
*/

/* As pointed out by Wei Dai <weidai@eskimo.com>, the above can be
 * simplified to the code below.  Wei attributes these optimisations
 * to Peter Gutmann's SHS code, and he attributes it to Rich Schroeppel.
 */
#define F(b,c,d) ((((c) ^ (d)) & (b)) ^ (d))
#define G(b,c,d) ((((b) ^ (c)) & (d)) ^ (c))
#define H(b,c,d) ((b) ^ (c) ^ (d))
#define I(b,c,d) (((~(d)) | (b)) ^ (c))

#undef ROTATE
#if defined(WIN32)
#define ROTATE(a,n) _lrotl(a,n)
#else
#define ROTATE(a,n) (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))
#endif

#define R0(a,b,c,d,k,s,t) \
{ a += ((k)+(t)+F((b),(c),(d))); \
  a = ROTATE(a,s); \
  a += b; \
};\

#define R1(a,b,c,d,k,s,t) \
{ a += ((k)+(t)+G((b),(c),(d))); \
  a = ROTATE(a,s); \
  a += b; \
};

#define R2(a,b,c,d,k,s,t) \
{ a += ((k)+(t)+H((b),(c),(d))); \
  a = ROTATE(a,s); \
  a += b; \
};

#define R3(a,b,c,d,k,s,t) \
{ a += ((k)+(t)+I((b),(c),(d))); \
  a = ROTATE(a,s); \
  a += b; \
};

#define INIT_DATA_A (unsigned long)0x67452301L
#define INIT_DATA_B (unsigned long)0xefcdab89L
#define INIT_DATA_C (unsigned long)0x98badcfeL
#define INIT_DATA_D (unsigned long)0x10325476L

static void md5_block(MD5_CTX *c,register ULONG *X,int num)
{ register ULONG A,B,C,D;

  A = c->A;
  B = c->B;
  C = c->C;
  D = c->D;
  for ( ; ; )
  { /* Round 1 */
    R0(A,B,C,D,X[ 0], 7,0xd76aa478L);
    R0(D,A,B,C,X[ 1],12,0xe8c7b756L);
    R0(C,D,A,B,X[ 2],17,0x242070dbL);
    R0(B,C,D,A,X[ 3],22,0xc1bdceeeL);
    R0(A,B,C,D,X[ 4], 7,0xf57c0fafL);
    R0(D,A,B,C,X[ 5],12,0x4787c62aL);
    R0(C,D,A,B,X[ 6],17,0xa8304613L);
    R0(B,C,D,A,X[ 7],22,0xfd469501L);
    R0(A,B,C,D,X[ 8], 7,0x698098d8L);
    R0(D,A,B,C,X[ 9],12,0x8b44f7afL);
    R0(C,D,A,B,X[10],17,0xffff5bb1L);
    R0(B,C,D,A,X[11],22,0x895cd7beL);
    R0(A,B,C,D,X[12], 7,0x6b901122L);
    R0(D,A,B,C,X[13],12,0xfd987193L);
    R0(C,D,A,B,X[14],17,0xa679438eL);
    R0(B,C,D,A,X[15],22,0x49b40821L);
    /* Round 2 */
    R1(A,B,C,D,X[ 1], 5,0xf61e2562L);
    R1(D,A,B,C,X[ 6], 9,0xc040b340L);
    R1(C,D,A,B,X[11],14,0x265e5a51L);
    R1(B,C,D,A,X[ 0],20,0xe9b6c7aaL);
    R1(A,B,C,D,X[ 5], 5,0xd62f105dL);
    R1(D,A,B,C,X[10], 9,0x02441453L);
    R1(C,D,A,B,X[15],14,0xd8a1e681L);
    R1(B,C,D,A,X[ 4],20,0xe7d3fbc8L);
    R1(A,B,C,D,X[ 9], 5,0x21e1cde6L);
    R1(D,A,B,C,X[14], 9,0xc33707d6L);
    R1(C,D,A,B,X[ 3],14,0xf4d50d87L);
    R1(B,C,D,A,X[ 8],20,0x455a14edL);
    R1(A,B,C,D,X[13], 5,0xa9e3e905L);
    R1(D,A,B,C,X[ 2], 9,0xfcefa3f8L);
    R1(C,D,A,B,X[ 7],14,0x676f02d9L);
    R1(B,C,D,A,X[12],20,0x8d2a4c8aL);
    /* Round 3 */
    R2(A,B,C,D,X[ 5], 4,0xfffa3942L);
    R2(D,A,B,C,X[ 8],11,0x8771f681L);
    R2(C,D,A,B,X[11],16,0x6d9d6122L);
    R2(B,C,D,A,X[14],23,0xfde5380cL);
    R2(A,B,C,D,X[ 1], 4,0xa4beea44L);
    R2(D,A,B,C,X[ 4],11,0x4bdecfa9L);
    R2(C,D,A,B,X[ 7],16,0xf6bb4b60L);
    R2(B,C,D,A,X[10],23,0xbebfbc70L);
    R2(A,B,C,D,X[13], 4,0x289b7ec6L);
    R2(D,A,B,C,X[ 0],11,0xeaa127faL);
    R2(C,D,A,B,X[ 3],16,0xd4ef3085L);
    R2(B,C,D,A,X[ 6],23,0x04881d05L);
    R2(A,B,C,D,X[ 9], 4,0xd9d4d039L);
    R2(D,A,B,C,X[12],11,0xe6db99e5L);
    R2(C,D,A,B,X[15],16,0x1fa27cf8L);
    R2(B,C,D,A,X[ 2],23,0xc4ac5665L);
    /* Round 4 */
    R3(A,B,C,D,X[ 0], 6,0xf4292244L);
    R3(D,A,B,C,X[ 7],10,0x432aff97L);
    R3(C,D,A,B,X[14],15,0xab9423a7L);
    R3(B,C,D,A,X[ 5],21,0xfc93a039L);
    R3(A,B,C,D,X[12], 6,0x655b59c3L);
    R3(D,A,B,C,X[ 3],10,0x8f0ccc92L);
    R3(C,D,A,B,X[10],15,0xffeff47dL);
    R3(B,C,D,A,X[ 1],21,0x85845dd1L);
    R3(A,B,C,D,X[ 8], 6,0x6fa87e4fL);
    R3(D,A,B,C,X[15],10,0xfe2ce6e0L);
    R3(C,D,A,B,X[ 6],15,0xa3014314L);
    R3(B,C,D,A,X[13],21,0x4e0811a1L);
    R3(A,B,C,D,X[ 4], 6,0xf7537e82L);
    R3(D,A,B,C,X[11],10,0xbd3af235L);
    R3(C,D,A,B,X[ 2],15,0x2ad7d2bbL);
    R3(B,C,D,A,X[ 9],21,0xeb86d391L);
    /* end */

    A += c->A&0xffffffffL;
    B += c->B&0xffffffffL;
    c->A = A;
    c->B = B;
    C += c->C&0xffffffffL;
    D += c->D&0xffffffffL;
    c->C = C;
    c->D = D;
    X += 16;
    num -= 64;
    if ( num <= 0 ) break;
  }
}

void MD5_Init(MD5_CTX *c)
{ c->A = INIT_DATA_A;
  c->B = INIT_DATA_B;
  c->C = INIT_DATA_C;
  c->D = INIT_DATA_D;
  c->Nl = 0;
  c->Nh = 0;
  c->num = 0;
}

void MD5_Update(MD5_CTX *c,register unsigned char *data,unsigned long len)
{ register ULONG *p;
  int sw,sc;
  ULONG l;

  if ( len == 0 ) return;

  l = (c->Nl+(len<<3))&0xffffffffL;
  /* 95-05-24 eay Fixed a bug with the overflow handling, thanks to
   * Wei Dai <weidai@eskimo.com> for pointing it out. */
  if ( l < c->Nl ) /* overflow */
    c->Nh++;
  c->Nh += (len>>29);
  c->Nl = l;

  if ( c->num != 0 )
  { p = c->data;
    sw = c->num>>2;
    sc = c->num&0x03;

    if ( (c->num+len) >= MD5_CBLOCK )
    { l = p[sw];
      p_c2l(data,l,sc);
      p[sw++] = l;
      for ( ; sw < MD5_LBLOCK; sw++ )
      { c2l(data,l);
        p[sw] = l;
      }
      len -= (MD5_CBLOCK-c->num);

      md5_block(c,p,64);
      c->num = 0;
      /* drop through and do the rest */
    }
    else
    { int ew,ec;

      c->num += (int)len;
      if ( (sc+len) < 4 ) /* ugly, add char's to a word */
      { l = p[sw];
        p_c2l_p(data,l,sc,len);
        p[sw] = l;
      }
      else
      { ew = (c->num>>2);
        ec = (c->num&0x03);
        l = p[sw];
        p_c2l(data,l,sc);
        p[sw++] = l;
        for ( ; sw < ew; sw++ )
        { c2l(data,l);
          p[sw] = l;
        }
        if ( ec )
        { c2l_p(data,l,ec);
          p[sw] = l;
        }
      }
      return;
    }
  }
  /* we now can process the input data in blocks of MD5_CBLOCK
   * chars and save the leftovers to c->data. */
#ifdef L_ENDIAN
  if ( (((unsigned long)data)%sizeof(ULONG)) == 0 )
  { sw = (int)len/MD5_CBLOCK;
    if ( sw > 0 )
    { sw *= MD5_CBLOCK;
      md5_block(c,(ULONG *)data,sw);
      data += sw;
      len -= sw;
    }
  }
#endif
  p = c->data;
  while ( len >= MD5_CBLOCK )
  {
#if defined(L_ENDIAN) || defined(B_ENDIAN)
    if ( p != (unsigned long *)data )
      memcpy(p,data,MD5_CBLOCK);
    data += MD5_CBLOCK;
#ifdef B_ENDIAN
    for ( sw = (MD5_LBLOCK/4); sw; sw-- )
    { Endian_Reverse32(p[0]);
      Endian_Reverse32(p[1]);
      Endian_Reverse32(p[2]);
      Endian_Reverse32(p[3]);
      p += 4;
    }
#endif
#else
    for ( sw = (MD5_LBLOCK/4); sw; sw-- )
    { c2l(data,l); *(p++) = l;
      c2l(data,l); *(p++) = l;
      c2l(data,l); *(p++) = l;
      c2l(data,l); *(p++) = l;
    }
#endif
    p = c->data;
    md5_block(c,p,64);
    len -= MD5_CBLOCK;
  }
  sc = (int)len;
  c->num = sc;
  if ( sc )
  { sw = sc>>2;  /* words to copy */
#ifdef L_ENDIAN
    p[sw] = 0;
    memcpy(p,data,sc);
#else
    sc &= 0x03;
    for ( ; sw; sw-- )
    { c2l(data,l);
      *(p++) = l;
    }
    c2l_p(data,l,sc);
    *p = l;
#endif
  }
}

void MD5_Transform(MD5_CTX *c,unsigned char *b)
{ ULONG p[16];
#if !defined(L_ENDIAN)
  ULONG *q;
  int i;
#endif

#if defined(B_ENDIAN) || defined(L_ENDIAN)
  memcpy(p,b,64);
#ifdef B_ENDIAN
  q = p;
  for ( i = (MD5_LBLOCK/4); i; i-- )
  { Endian_Reverse32(q[0]);
    Endian_Reverse32(q[1]);
    Endian_Reverse32(q[2]);
    Endian_Reverse32(q[3]);
    q += 4;
  }
#endif
#else
  q = p;
  for ( i = (MD5_LBLOCK/4); i; i-- )
  { ULONG l;
    c2l(b,l); *(q++) = l;
    c2l(b,l); *(q++) = l;
    c2l(b,l); *(q++) = l;
    c2l(b,l); *(q++) = l;
  }
#endif
  md5_block(c,p,64);
}

void MD5_Final(unsigned char *md,MD5_CTX *c)
{ register int i,j;
  register ULONG l;
  register ULONG *p;
  static unsigned char end[4] = { 0x80, 0x00, 0x00, 0x00 };
  unsigned char *cp = end;

  /* c->num should definitly have room for at least one more byte. */
  p = c->data;
  j = c->num;
  i = j>>2;

  /* purify often complains about the following line as an
   * Uninitialized Memory Read.  While this can be true, the
   * following p_c2l macro will reset l when that case is true.
   * This is because j&0x03 contains the number of 'valid' bytes
   * already in p[i].  If and only if j&0x03 == 0, the UMR will
   * occur but this is also the only time p_c2l will do
   * l= *(cp++) instead of l|= *(cp++)
   * Many thanks to Alex Tang <altitude@cic.net> for pickup this
   * 'potential bug' */
#ifdef PURIFY
  if ( (j&0x03) == 0 ) p[i] = 0;
#endif
  l = p[i];
  p_c2l(cp,l,j&0x03);
  p[i] = l;
  i++;
  /* i is the next 'undefined word' */
  if ( c->num >= MD5_LAST_BLOCK )
  { for ( ; i < MD5_LBLOCK; i++ )
      p[i] = 0;
    md5_block(c,p,64);
    i = 0;
  }
  for ( ; i < (MD5_LBLOCK-2); i++ )
    p[i] = 0;
  p[MD5_LBLOCK-2] = c->Nl;
  p[MD5_LBLOCK-1] = c->Nh;
  md5_block(c,p,64);
  cp = md;
  l = c->A; l2c(l,cp);
  l = c->B; l2c(l,cp);
  l = c->C; l2c(l,cp);
  l = c->D; l2c(l,cp);

  /* clear stuff, md5_block may be leaving some stuff on the stack
   * but I'm not worried :-) */
  c->num = 0;
/*  memset((char *)&c,0,sizeof(c));*/
}

unsigned char *MD5(unsigned char *d,unsigned long n,unsigned char *md)
{ MD5_CTX c;
  static unsigned char m[MD5_DIGEST_LENGTH];

  if ( md == 0 ) md = m;
  MD5_Init(&c);
  MD5_Update(&c,d,n);
  MD5_Final(md,&c);
  memset(&c,0,sizeof(c)); /* security consideration */
  return md;
}

