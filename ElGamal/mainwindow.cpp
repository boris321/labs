#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "superlong.h"
#include "md5.h"
#include <QFile>
#include <QTextStream>
#include <fstream>
#include <QTime>

using namespace std;

#define size_max 2048

// ВЫРАБОАТЬ ПОДПИСЬ

void MainWindow::Subscribe()
{
    Inicialization(); // инициализация и генерация входных параметров

    string str;

    // инициализация переменных
    superlong m, p, g, k, x, y, r, s, p1, k1, temp;
    m.get( ui->lineEdit_m->text().toStdString() );
    p.get( ui->lineEdit_p->text().toStdString() );
    g.get( ui->lineEdit_g->text().toStdString() );
    k.get( ui->lineEdit_k->text().toStdString() );
    x.get( ui->lineEdit_x->text().toStdString() );

    // ВЫРАБОАТЬ ПОДПИСЬ

    // **************************************************************************

    // открытый ключ y
    y = powm( g, x, p);

    // половинка ЭЦП r
    r = powm( g, k, p);

    // половинка ЭЦП s = k^-1 * ( x * r - m ) ( mod p )
    p1 = p - 1;
    k1 = evklid_ext_x( k, p1 ); // находим обратный элемент k в кольце p-1
    temp = x * r;
    m %= p1;
    if ( m < temp ) {
        temp = p1 - ( temp - m ) % p1;
    }
    else {
        temp -= m;
    }
    s = k1 * temp;
    s %= p1;

    // **************************************************************************

    // вывод результатов на экран
    y.give( str );
    ui->lineEdit_y->setText( QString::fromStdString( str ) );
    r.give( str );
    ui->lineEdit_r->setText( QString::fromStdString( str ) );
    s.give(str);
    ui->lineEdit_s->setText( QString::fromStdString( str ) );
}

// ПРОВЕРКА ПОДПИСИ НА ПОДЛИННОСТЬ

void MainWindow::Verify()
{
    // инициализация переменных
    superlong m, p, g, k, x, y, r, s, v1, v2;
    m.get( ui->lineEdit_m->text().toStdString() );
    p.get( ui->lineEdit_p->text().toStdString() );
    g.get( ui->lineEdit_g->text().toStdString() );
    k.get( ui->lineEdit_k->text().toStdString() );
    x.get( ui->lineEdit_x->text().toStdString() );
    r.get( ui->lineEdit_r->text().toStdString() );
    y.get( ui->lineEdit_y->text().toStdString() );
    s.get( ui->lineEdit_s->text().toStdString() );

    // ПРОВЕРКА ПОДПИСИ НА ПОДЛИННОСТЬ

    // **************************************************************************

    if ( r < 1 || r == p - 1 ) {
        ui->lineEdit->setText( QString::fromLocal8Bit("Подпись подлинная!") );
    }
    else {
        v1 = powm( g, m, p );
        v2 = powm( y, r, p ) * powm( r, s, p ) % p;
        if ( v1 == v2 ) ui->lineEdit->setText( QString::fromLocal8Bit("Подпись подлинная!") );
        else ui->lineEdit->setText( QString::fromLocal8Bit("Подпись подделанная!") );
    }

    // **************************************************************************
}

// инициализация и генерация входных параметров
void MainWindow::Inicialization() {
    if ( ui->lineEdit_M->text().isEmpty() ) ui->lineEdit_M->setText( "MESSAGE" );
    generate_m();
}

// вычислить хэш сообщения M
void MainWindow::generate_m()
{
    QString str = ui->lineEdit_M->text();
    str = hash( str );
    ui->lineEdit_m->setText( str );
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Base,Qt::green);
    ui->lineEdit_p->setPalette(*palette);
    ui->lineEdit_g->setPalette(*palette);
    ui->lineEdit_y->setPalette(*palette);
    palette->setColor(QPalette::Base,Qt::yellow);
    ui->lineEdit_r->setPalette(*palette);
    ui->lineEdit_s->setPalette(*palette);

    connect(ui->pushButton_Subscribe, SIGNAL( clicked() ), SLOT( Subscribe() ));
    connect(ui->pushButton_Verify, SIGNAL( clicked() ), SLOT( Verify() ));
    connect(ui->pushButton_m, SIGNAL( clicked() ), SLOT( generate_m() ));

    string str;

    // считываем сообщение из файла
    ifstream M_file("M.txt");
    M_file >> str;
    ui->lineEdit_M->setText( QString::fromStdString( str ) );
    M_file.close();

    // считываем данные из файла
    ifstream data_file("data.txt");
    data_file >> str;
    ui->lineEdit_p->setText( QString::fromStdString( str ) );
    data_file >> str;
    ui->lineEdit_g->setText( QString::fromStdString( str ) );
    data_file >> str;
    ui->lineEdit_k->setText( QString::fromStdString( str ) );
    data_file >> str;
    ui->lineEdit_x->setText( QString::fromStdString( str ) );
    data_file.close();

    // инициализируем random
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
