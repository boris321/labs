#include "mainwindow.h"
#include "des.h"
#include <QFile>
#include <QTime>

#define size_max 2048

char * QStringToCharStr(QString str) {
    char * charstr = new char [str.size()];
    string stdstr = str.toStdString();
    for (int i = 0; i < str.size(); i++) {
        charstr[i] = stdstr[i];
    }
    return charstr;
}

void generateV( unsigned int size )
{
    FILE *file_v;
    file_v = fopen( "generatevector", "wb" );
    qsrand( QTime( 0, 0, 0 ).secsTo( QTime::currentTime() ));
    unsigned char c[size];
    for ( unsigned int i = 0; i < size; i++ ) {
        c[i] = qrand() % 256;
    }
    fwrite( c, 1, size, file_v );
    fclose( file_v );
}

void generateK( unsigned int size )
{
    FILE *file_v;
    file_v = fopen( "generatekey", "wb" );
    qsrand( QTime( 0, 0, 0 ).secsTo( QTime::currentTime() ));
    unsigned char c[size];
    for ( unsigned int i = 0; i < size; i++ ) {
        c[i] = qrand() % 256;
    }
    fwrite( c, 1, size, file_v );
    fclose( file_v );
}

QString FileToQString( QString filename )
{
    QString str;

    FILE *f;
    f = fopen( filename.toUtf8().constData(), "rb" );
    unsigned char c[1024];
    unsigned int size = fread( c, 1, 1024, f );
    fclose(f);

    for ( unsigned int i = 0; i < size; i++ ) {
        str += c[i];
    }

    return str;
}

QString FileToQString16( QString filename )
{
    QString str;

    FILE *f;
    f = fopen( filename.toUtf8().constData(), "rb" );
    unsigned char c[1024];
    unsigned int size = fread( c, 1, 1024, f );
    fclose(f);

    unsigned char temp;
    for (unsigned int i = 0; i < size; i++) {
        temp = c[i] / 16;
        if (temp < 10) temp += '0';
        else {
            temp -= 10;
            temp += 'a';
        }
        str += temp;
        temp = c[i] % 16;
        if (temp < 10) {
            temp += '0';
        }
        else {
            temp -= 10;
            temp += 'a';
        }
        str += temp;
    }

    return str;
}

void CreateXOR( QString filename_i )
{
    FILE *file_i, *file_o;
    file_i = fopen( filename_i.toUtf8().constData(), "rb" );
    file_o = fopen( "xor", "wb" );
    unsigned char c[size_max];
    unsigned int size = fread( c, 1, size_max, file_i );
    for ( unsigned int i = 0; i < size; i++ ) {
        c[i] = c[i] ^ 255;
    }
    fwrite( c, 1, size, file_o );
    fclose( file_i );
    fclose( file_o );
}


void saveR( QString filename_o )
{
    FILE *file_i, *file_o;
    file_i = fopen( "result", "rb" );
    file_o = fopen( filename_o.toUtf8().constData(), "wb" );
    unsigned char c[size_max];
    unsigned int size = fread( c, 1, size_max, file_i );
    fwrite( c, 1, size, file_o );
    fclose( file_i );
    fclose( file_o );
}
