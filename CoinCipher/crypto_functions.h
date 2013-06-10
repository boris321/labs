#include <QString>
#include <fstream>
#include "md5.h"
#include "des.h"

using namespace std;

int poWmod ( char *t, char *k, char *mod, char *res );
int NOD( char * ,char * , char * );
int NOD_rash( char * a, char * b, char * d, char * x, char * y );
int RobinTest(char *n);

// ���������� � ������� �� ������
superlong powm( superlong & sla, superlong & slb, superlong & slm )
{
    string str;
    char *t = new char [N];
    char *k = new char [N];
    char *mod = new char [N];
    char *res = new char [N];

    sla.give( str );
    strcpy( t, str.c_str() );
    slb.give( str );
    strcpy( k, str.c_str() );
    slm.give( str );
    strcpy( mod, str.c_str() );

    int result;
    result = poWmod( t, k, mod, res );

    superlong c;
    c.get( res );

    free(t);
    free(k);
    free(mod);
    free(res);

    return c;
}

// ���������� ����� �������� - �������� �������
superlong nod( superlong & sla, superlong & slb )
{
    string str;
    char *a = new char [N];
    char *b = new char [N];
    char *d = new char [N];

    sla.give( str );
    strcpy( a, str.c_str() );
    slb.give( str );
    strcpy( b, str.c_str() );

    int result;
    result = NOD( a, b, d );

    superlong c;
    c.get( d );

    free(a);
    free(b);
    free(d);

    return c;
}

// ���������� ����� �������� - ���������� �������� �������
superlong evklid_ext( superlong & sla, superlong & slb )
{
    string str;
    char *a = new char [N];
    char *b = new char [N];
    char *d = new char [N];
    char *x = new char [N];
    char *y = new char [N];

    sla.give( str );
    strcpy( a, str.c_str() );
    slb.give( str );
    strcpy( b, str.c_str() );

    int result;
    result = NOD_rash( a, b, d, x, y );

    superlong c;
    c.get( d );

    free(a);
    free(b);
    free(d);
    free(x);
    free(y);

    return c;
}

// ���������� ����� �������� - ���������� �������� �������
superlong evklid_ext_x( superlong & sla, superlong & slb )
{
    string str;
    char *a = new char [N];
    char *b = new char [N];
    char *d = new char [N];
    char *x = new char [N];
    char *y = new char [N];

    sla.give( str );
    strcpy( a, str.c_str() );
    slb.give( str );
    strcpy( b, str.c_str() );

    int result;
    result = NOD_rash( a, b, d, x, y );

    superlong c;
    c.get( y );
    if ( c < 0 ) {
        c = slb - c.abs();
    }

    free(a);
    free(b);
    free(d);
    free(x);
    free(y);

    return c;
}

// ��������� ����� �� ��������
bool isPrimitive( superlong & a ) {

    // ����������
    if ( a == 0 ) return 0;
    if ( a == 1 ) return 0;

    // ������� �������
    string str;
    superlong prime_number;
    ifstream prime_numbers( "prime_numbers.txt" );
    while ( prime_numbers >> str ) {
        prime_number.get( str );
        if ( a == prime_number ) {
            prime_numbers.close();
            return 1;
        }
        if ( a % prime_number == 0 ) {
            prime_numbers.close();
            return 0;
        }
    }
    prime_numbers.close();

    // ���� ������-�������
    a.give( str );
    int test;
    char *n = new char [N];
    strcpy( n, str.c_str() );
    for ( unsigned int i = 0 ; i < 5 ; i++ ) {
        test = RobinTest( n );
        if ( !test ) {
            free( n );
            return 0;
        }
    }
    free( n );
    return 1;
}

// ������������� �����
superlong generate( superlong & n )
{
    string str;
    n.give( str );
    unsigned int strsize = str.size();
    superlong a;

    str.clear();
    for ( unsigned int i = 0; i < strsize ; i++ ) {
        str += rand() % 10 + '0';
    }
    a.get( str );
    a %= n;

    return a;
}

// ������������� ������� �����
superlong generatep( const char * charstr )
{
    string str = charstr;
    unsigned int strsize = str.size();
    superlong p, n;
    n.get( charstr );

    do
    {
        str.clear();
        for ( unsigned int i = 0; i < strsize ; i++ ) {
            str += rand() % 10 + '0';
        }
        p.get( str );
        p %= n;
    }
    while ( !isPrimitive( p ) );

    return p;
}

// ������������� ������� �����
superlong generatep( superlong & n )
{
    string str;
    n.give( str );
    unsigned int strsize = str.size();
    superlong p;

    do
    {
        str.clear();
        for ( unsigned int i = 0; i < strsize ; i++ ) {
            str += rand() % 10 + '0';
        }
        p.get( str );
        p %= n;
    }
    while ( !isPrimitive( p ) );

    return p;
}

// ���������� ���
QString hash( QString message )
{
    MD5_CTX c;
    const unsigned int size_max = 1024;
    unsigned char i[ size_max ] = {0};
    unsigned char o[ 16 ];

    unsigned int size = message.size();
    strcpy( ( char* )i, message.toStdString().c_str() );

    MD5_Init( &c );
    MD5_Update( &c, i, size );
    MD5_Final( o, &c);

    string str;
    for ( unsigned int i = 0; i < 16 ; i++ ) str += o[ i ] % 10 + '0';

    return QString::fromStdString( str );
}

// ����������
QString crypt( QString input, QString key, bool enc )
{
    ofstream input_file( "input.txt" );
    input_file << input.toStdString();
    input_file.close();
    ofstream key_file( "key.txt" );
    key_file << key.toStdString();
    key_file.close();

    const unsigned int size_max = 1024;
    unsigned char i[size_max] = {0};
    unsigned char o[size_max] = {0};
    unsigned char k[8] = {0};

    FILE *file_i, *file_o, *file_k;

    file_i = fopen( "input.txt", "rb" );
    file_o = fopen( "output.txt", "wb" );
    file_k = fopen( "key.txt", "rb" );

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
            des_enc_ecb( schedule, i, o, blocks );
            fwrite( o, 1, 8 * blocks, file_o );
            for ( j = 0; j < size_max; j++ ) i[j] = NULL;
        }
        else {
            des_dec_ecb( schedule, i, o, blocks );
            for ( j = 0; o[j] && j < size_max ; j++ );
            fwrite( o, 1, j, file_o);
        }
    }

    fclose(file_i);
    fclose(file_k);
    fclose(file_o);

    string output;
    ifstream output_file( "output.txt" );
    output_file >> output;
    output_file.close();
    return QString::fromStdString( output );
}
