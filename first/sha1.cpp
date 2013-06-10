#include <string>

using namespace std;

unsigned int SHA1CircularShift(unsigned int X, int s);
string SHA1IntToHex(unsigned int A);

//�������� ������� SHA1
string SHA1hash(string text)
{
        unsigned int i, k; // �������� ��� ������
         unsigned int h0 = 0x67452301, h1 = 0xEFCDAB89, h2 = 0x98BADCFE, h3 = 0x10325476, h4 = 0xC3D2E1F0; // ��������� �������� ���������� ��� ������ ���������
        //unsigned int h0 = 1732584193, h1 = 4023233417, h2 = 2562383102, h3 = 271733878, h4 = 3285377520; // ��������� �������� ���������� ��� ������ ���������
         unsigned int K1 = 0x5A827999, K2 = 0x6ED9EBA1, K3 = 0x8F1BBCDC, K4 = 0xCA62C1D6; // ��������� ��� ������ ���������
        //unsigned int K1 = 1518500249, K2 = 1859775393, K3 = 2400959708, K4 = 3395469782; // ��������� ��� ������ ���������
        unsigned int a, b, c, d, e; // ������ ��� ������ ���������
        unsigned int t, f, K; // ��������������� ����������
        unsigned long L; // ����� ��������� � �����
        unsigned long temp; // ��������������� ����������
        unsigned char *Stream = new unsigned char[64]; // ���������� ���������
        unsigned int *W = new unsigned int[80]; // ������ 32-������ ����

        // ����� ��������� � �����
        L = (unsigned long)(text.size() << 3);

        // ��������� ���� ����������� �� �����, ������� 512 ���.
        // ��������� ��������� ��� � ����� ���������
        text += (unsigned char) 128;
        temp = text.size() % 64;
        // ���� ��������� ���� ����� ����� ����� 448, �� ����� 512 ���
        if (temp > 56)
        {
                text.resize(text.size()+(64-temp));
                // ��������� ���� ������ �� ����� 512-������� �����
                for (i=temp; i<text.size(); i++)
                        text[i] = 0;
                // ����� ����� ��������� ��� ���� 512-������ ����,
                // ������� ����������� ������ �� 448 ��� ������

        }
        // ��������� ����, ����� ����� ����� ����� ������ (512 - 64 = 448) ���.

        temp = text.size();
        text.resize(text.size() + (56 - (text.size() % 64)));
        for (i=temp; i<text.size(); i++)
                text[i] = 0;

        // ����� ���� � ���������� 64 ���� ������������ ����� ��������� ��������� � �����
        temp = text.size();
        text.resize(text.size()+8);
        // ��������� ����� ��������� ��������� (�� ��������������� ���������) ��� ����� 64-������ Big-endian (�� �������� � ��������) �����, � �����
        for (i=text.size()-1; i>=temp; i--)
        {
                if (L > 0)
                {
                        text[i] = (unsigned char)L;
                        L >>= 8;
                }
                else
                        text[i] = 0;
        }

        for (i=0; i<text.size(); i+=64)
        {
                // �������� ���� ��������� 512 ���
                for (k=0; k<64; k++)
                        Stream[k] = (unsigned char) text[k+i];

                //������������ ������� 32-������ ����
                // ��� 0<=k<=15 W[k] = ����� ��������� ���������
                for(k = 0; k < 16; k++)
                {
                        W[k] = ((unsigned int) Stream[k * 4]) << 24;
                        W[k] |= ((unsigned int) Stream[k * 4 + 1]) << 16;
                        W[k] |= ((unsigned int) Stream[k * 4 + 2]) << 8;
                        W[k] |= ((unsigned int) Stream[k * 4 + 3]);
                }

                // ��� 16<=k<=79
                for (k=16; k<80; k++)
                        W[k] = SHA1CircularShift((W[k-3] ^ W[k-8] ^ W[k-14] ^ W[k-16]), 1); // ^ - xor (����������� ���)

                // ������������� ���-�������� ���� �����
                a = h0;
                b = h1;
                c = h2;
                d = h3;
                e = h4;

                // �������� ����
                for (k=0; k<80; k++) {

                    if (k >=0 && k < 20) {
                        f = (b & c) | ((~b) & d);
                        K = K1;
                    }
                    else if (k >=20 && k < 40) {
                        f = b ^ c ^ d;
                        K = K2;
                    }
                    else if (k >=40 && k < 60) {
                        f = (b & c) | (b & d) | (c & d);
                        K = K3;
                    }
                    else if (k >=60 && k < 80) {
                        f = b ^ c ^ d;
                        K = K4;
                    }

                    //t = (a << 5) + f + e + K + W[k];
                    t = SHA1CircularShift(a, 5) + f + e + K + W[k];
                    e = d;
                    d = c;
                    c = SHA1CircularShift(b, 30);
                    //c = b << 30;
                    b = a;
                    a = t;
                }

                // ��������� ���-�������� ���� ����� � ����������:
                h0 = h0 + a;
                h1 = h1 + b;
                h2 = h2 + c;
                h3 = h3 + d;
                h4 = h4 + e;
        }

        // �������� ���-��������:
        string str = SHA1IntToHex(h0) + SHA1IntToHex(h1) + SHA1IntToHex(h2) + SHA1IntToHex(h3) + SHA1IntToHex(h4);
//        string str = SHA1IntToHex(h0) + " " + SHA1IntToHex(h1) + " " + SHA1IntToHex(h2) + " " + SHA1IntToHex(h3) + " " + SHA1IntToHex(h4);
        return str;
}

// ����������� �����
unsigned int SHA1CircularShift(unsigned int X, int s)
{
        return (X << s) | (X >> (32-s));
}

// ������� �� int � ����������������� �������
string SHA1IntToHex(unsigned int A)
{
        string str = ""; // ������ ��� ������ ����������
        unsigned char *Temp = new unsigned char[8]; // ��������������� ����������
        unsigned char i; // ������� ��� �����
        unsigned int t; // ��������������� ����������

        // ������� �� ���������� ������� � �����������������
        for (i = 0; i < 8; i += 2, A >>= 8)
        {
                t = (unsigned char)A;
                Temp[i] = (unsigned char)(t >> 4);
                Temp[i + 1] = (unsigned char)(t % 16);
        }
        for (i=0; i<8 ; i+=2)
        {
                // 48+0 - '0', 87+10 - 'a'
                str += Temp[7-i-1] < 10 ? (char)(Temp[7-i-1] + 48) : (char)(Temp[7-i-1] + 87);
                str += Temp[7-i] < 10 ? (char)(Temp[7-i] + 48) : (char)(Temp[7-i] + 87);
        }
        delete []Temp;
        return str;
}


