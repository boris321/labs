#include "ripemd.h"
#include <stdio.h>

DWORD K1[5] = {0x00000000, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E};
DWORD K2[5] = {0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0x00000000};

BYTE indicesR1[80] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    7,  4, 13,  1, 10,  6, 15,  3, 12,  0,  9,  5,  2, 14, 11,  8,
    3, 10, 14,  4,  9, 15,  8,  1,  2,  7,  0,  6, 13, 11,  5, 12,
    1,  9, 11, 10,  0,  8, 12,  4, 13,  3,  7, 15, 14,  5,  6,  2,
    4,  0,  5,  9,  7, 12,  2, 10, 14,  1,  3,  8, 11,  6, 15, 13
};
BYTE indicesR2[80] = {
    5, 14,  7,  0,  9,  2, 11,  4, 13,  6, 15,  8,  1, 10,  3, 12,
    6, 11,  3,  7,  0, 13,  5, 10, 14, 15,  8, 12,  4,  9,  1,  2,
    15,  5,  1,  3,  7, 14,  6,  9, 11,  8, 12,  2, 10,  0,  4, 13,
    8,  6,  4,  1,  3, 11, 15,  0,  5, 12,  2, 13,  9,  7, 10, 14,
    12, 15, 10,  4,  1,  5,  8,  7,  6,  2, 13, 14,  0,  3,  9, 11
};
BYTE shiftS1[80] = {
    11, 14, 15, 12,  5,  8,  7,  9, 11, 13, 14, 15,  6,  7,  9,  8,
    7,  6,  8, 13, 11,  9,  7, 15,  7, 12, 15,  9, 11,  7, 13, 12,
    11, 13,  6,  7, 14,  9, 13, 15, 14,  8, 13,  6,  5, 12,  7,  5,
    11, 12, 14, 15, 14, 15,  9,  8,  9, 14,  5,  6,  8,  6,  5, 12,
    9, 15,  5, 11,  6,  8, 13, 12,  5, 12, 13, 14, 11,  8,  5,  6
};
BYTE shiftS2[80] = {
    8,  9,  9, 11, 13, 15, 15,  5,  7,  7,  8, 11, 14, 14, 12,  6,
    9, 13, 15,  7, 12,  8,  9, 11,  7,  7, 12,  7,  6, 15, 13, 11,
    9,  7, 15, 11,  8,  6,  6, 14, 12, 13,  5, 14, 13, 13,  7,  5,
    15,  5,  8, 11, 14, 14,  6, 14,  6,  9, 12,  9, 12,  5, 15,  8,
    8,  5, 12,  9, 12,  5, 14,  6,  8, 13,  6,  5, 15, 13, 11, 11
};


/* ROL(x, n) циклически сдвигает x на n бит влево */
/* x должен быть unsigned 32 тип и 0 <= n < 32. */
DWORD ROL_func(DWORD x, int n)  { return ((x) << (n)) | ((x) >> (32-(n))); }

DWORD F_func(DWORD x, DWORD y, DWORD z) { return (x) ^ (y) ^ (z); }		// 0 <= j <= 15
DWORD G_func(DWORD x, DWORD y, DWORD z) { return ((x) & (y)) | (~(x) & (z)); }	// 16 < j < 31
DWORD H_func(DWORD x, DWORD y, DWORD z) { return ((x) | ~(y)) ^ (z); }		// 32 < j < 47
DWORD I_func(DWORD x, DWORD y, DWORD z) { return ((x) & (z)) | ((y) & ~(z)); }	// 48 < j < 63
DWORD J_func(DWORD x, DWORD y, DWORD z) { return (x) ^ ((y) | ~(z)); }		// 64 < j < 79

DWORD f(int index, DWORD x, DWORD y, DWORD z) {
    if ( index >= 0 && index <= 15 ) {    // >= 0 and <= 15    от 0 до 15   - всего 16
        return F_func(x, y, z);
    }
    if ( index >= 16 && index <= 31 ) {   // 16 - 31
        return G_func(x, y, z);
    }
    if ( index >= 32 && index <= 47 ) {
        return H_func(x, y, z);
    }
    if ( index >= 48 && index <= 63 ) {
        return I_func(x, y, z);
    }
    if ( index >= 64 && index <= 79 ) {
        return J_func(x, y, z);
    }
    return 0;
}

DWORD retK(int index, DWORD *K) {
    //if ( index >= 0 && index <= 15) return  K[0];
    //if ( index >= 16 && index <= 31) return K[1];
    //if ( index >= 32 && index <= 47) return K[2];
    //if ( index >= 48 && index <= 63) return K[3];
    //if (index >= 64 && index <= 79) return K[4];
    return K[4];
}

bool RIPEMD160::compress(unsigned char *strptr, DWORD lswlen) {

    DWORD h0 = 0x67452301UL, h1 = 0xEFCDAB89, h2 = 0x98BADCFE, h3 = 0x10325476, h4 = 0xC3D2E1F0;

    // нужно сделать в несколько блоков
    // заполнить массив 16 4 байтных двойных слов

    // количество блоков
    int t = 0;
    // округлить вверх
    float t0 = (float)lswlen / 64;
    int t2 = lswlen % 64;
    if ( t2 ) {
        t2 = lswlen + ( 64 - t2 );
        t0 = (float)t2 / 64;
    }
    // количество блоков по 512 бит
    t = (int)t0;

    // если длина сообщения 0 - всё равно как один блок будет
    if ( lswlen == 0 ) t = 1;

    DWORD X[16]; // сообщение сюда поместится

    DWORD A1, B1, C1, D1, E1;
    DWORD A2, B2, C2, D2, E2;
    DWORD T;

    unsigned char *msg = 0;
    int lenStrSub = lswlen;

    for (int i = 0; i < t; ++i ) {

        memset(X, 0, 16 * sizeof(DWORD));

        //int ifGt =
        // 12345678901234567890123456789012345678901234567890123456789012345678901234567890   - 80
        // 123456789012345678901234567890123456789012345678901234567890123    - 63
        // abcd

        msg = strptr + (i * 64);

        // от 0-62
        // for (int i2 = 0; i2 < (lswlen & 63); i2++) {
        if ( i != t - 1 ) {
            for ( int i2 = 0; i2 < 64; i2++ ) {
                X[i2 >> 2] ^= (DWORD) * msg++ << ( 8 * ( i2 & 3 ) );    // копирую в массив X из strptr  длина 64 байта - 512 бит
            }
        }
        else {
            if ( i == t - 1 ) {
                for (int i3 = 0; i3 < (lenStrSub & 63); i3++) {
                    X[i3 >> 2] ^= (DWORD) * msg++ << ( 8 * (i3 & 3 ) ); // копирую в массив X из strptr  длина 64 байта - 512 бит
                }

                int shift = ( 8 * ( lenStrSub & 3 ) + 7 );
                DWORD data = (DWORD) 1 << shift;
                X[( lenStrSub >> 2)  & 15] ^= data; // добавляю в конец сообщения бит = 1  единицу

                X[14] = lswlen << 3;    // добавляю длину в битах
                X[15] = (lswlen >> 29) | (0 << 3);
            }
        }

        lenStrSub -= 64;

        //int t = 1; // 1 блок

        //for(int i = 0; i < t-1; i++)
        //{
        A1 = h0;   B1 = h1;   C1 = h2;   D1 = h3;   E1 = h4;
        A2 = h0;   B2 = h1;   C2 = h2;   D2 = h3;   E2 = h4;

        /*(a) += F((b), (c), (d)) + (x) + 0x5a827999UL;
        (a) = ROL((a), (s)) + (e);
        (c) = ROL((c), 10);*/
        DWORD hr0, hr1, hr2, hr3, hr4;

        // 80 итераций-раундов, в два параллельных потока
        for ( int j = 0; j < 80; j++ ) {
            // первый поток
            T = ROL_func( ( A1 + f(j, B1, C1, D1) + X[indicesR1[j]] + retK(j, K1) ), shiftS1[j] ) + E1;
            A1 = E1;   E1 = D1;   D1 = ROL_func(C1, 10);   C1 = B1;   B1 = T;

            // второй поток
            T = ROL_func( ( A2 + f(79-j, B2, C2, D2) + X[indicesR2[j]] + retK(j, K2) ), shiftS2[j] ) + E2;
            A2 = E2;   E2 = D2;   D2 = ROL_func(C2, 10);   C2 = B2;   B2 = T;

            // на каждой итерации - вычисляем хэш
            T = h1 + C1 + D2;       hr1 = h2 + D1 + E2;   hr2 = h3 + E1 + A2;
            hr3 = h4 + A1 + B2;     hr4 = h0 + B1 + C2;   hr0 = T;
        }

        /* final result */
        T = h1 + C1 + D2;    h1 = h2 + D1 + E2;   h2 = h3 + E1 + A2;
        h3 = h4 + A1 + B2;   h4 = h0 + B1 + C2;   h0 = T;
    }

    // (a, b, c, d, e, x, s)  {  (a) += J((b), (c), (d)) + (x) + 0x50a28be6UL;   (a) = ROL((a), (s)) + (e);   (c) = ROL((c), 10);  }

    DWORD result[5];
    result[0] = h0; result[1] = h1; result[2] = h2; result[3] = h3; result[4] = h4;

    //h0 || h1 || h2 || h3 || h4
    for ( int i = 0; i < 20; i += 4 ) {
        hash[i    ] =  result[i >> 2];
        hash[i + 1] = (result[i >> 2] >> 8);
        hash[i + 2] = (result[i >> 2] >> 16);
        hash[i + 3] = (result[i >> 2] >> 24);
    }

    return true;
}

