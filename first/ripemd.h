#ifndef RIPEMD_160
#define RIPEMD_160

#include <Windows.h>

#include <string>
using namespace std;

class MemBlock {

public:
    MemBlock() { memset(dataBlock, 0, 8*4*5*80*3); }    // 3 ����� �� ���� �����
    ~MemBlock() {}

public:
    // ���� ������ ��� 80 �������� - �������� ����� � �����, (8 ��� * 4 ���� * 5 DWORD * 80 ����)   [12800]     ���� ���-160 ���
    char dataBlock[8*4*5*80*3];

    char* setOffset(int offset) {
        return &(this->dataBlock[offset]);
    }
};

class RIPEMD160 {
public:
    RIPEMD160() {}
    ~RIPEMD160() {}

    // ����� ���������� ����
    bool compress(unsigned char *strptr, DWORD lswlen);

public:
    //MemBlock *curMem;
    int offset;

    BYTE hash[20];
};

#endif
