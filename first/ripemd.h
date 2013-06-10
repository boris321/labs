#ifndef RIPEMD_160
#define RIPEMD_160

#include <Windows.h>

#include <string>
using namespace std;

class MemBlock {

public:
    MemBlock() { memset(dataBlock, 0, 8*4*5*80*3); }    // 3 блока то есть можно
    ~MemBlock() {}

public:
    // блок данных для 80 итераций - выгрузка будет в битах, (8 бит * 4 байт * 5 DWORD * 80 штук)   [12800]     один хэш-160 бит
    char dataBlock[8*4*5*80*3];

    char* setOffset(int offset) {
        return &(this->dataBlock[offset]);
    }
};

class RIPEMD160 {
public:
    RIPEMD160() {}
    ~RIPEMD160() {}

    // метод вычисления хэша
    bool compress(unsigned char *strptr, DWORD lswlen);

public:
    //MemBlock *curMem;
    int offset;

    BYTE hash[20];
};

#endif
