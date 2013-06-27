#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "superlong.h"
#include "md5.h"
#include <QFile>
#include <QTextStream>
#include <fstream>

using namespace std;

// ВЫРАБАТЫВАЕМ ЭЦП

void MainWindow::Subscribe()
{
    Inicialization(); // инициализация и генерация входных параметров

    string str, M;
    superlong p, q, b, r, a, x, e, y, a1, v;

    // считываем значения
    M = ui->lineEdit_M->text().toStdString();       // сообщение M
    p.get( ui->lineEdit_p->text().toStdString() );  //
    q.get( ui->lineEdit_q->text().toStdString() );  //
    b.get( ui->lineEdit_b->text().toStdString() );  // b такое что b^q = 1 ( mod p )
    r.get( ui->lineEdit_r->text().toStdString() );  //
    a.get( ui->lineEdit_a->text().toStdString() );  //

    // ВЫРАБАТЫВАЕМ ЭЦП

    //**************************************************************

    // секретный ключ x = b ^ r ( mod p )
    x = powm( b, r, p );

    // первая половинка ЭЦП (хэш от сообщения) e = h( M )
    e.get( hash( QString::fromStdString( M )).toStdString() );

    // вторая половинка ЭЦП y = ( a * e + r ) ( mod q )
    y = ( a * e + r ) % q;

    // открытый ключ v = b ^ ( q - a ) ( mod p )
    a1 = q - a;
    v = powm( b, a1, p );

    //**************************************************************

    // вывод результатов на экран
    x.give( str );
    ui->lineEdit_x->setText( QString::fromStdString( str ) );
    e.give( str );
    ui->lineEdit_e->setText( QString::fromStdString( str ) );
    y.give( str );
    ui->lineEdit_y->setText( QString::fromStdString( str ) );
    v.give( str );
    ui->lineEdit_v->setText( QString::fromStdString( str ) );
}

// ПРОВЕРЯЕМ ЭЦП

void MainWindow::Verify()
{
    superlong p, q, b, temp, temp2, temp1, x, e, y, v;

    // считываем значения
    p.get( ui->lineEdit_p->text().toStdString() );  //
    q.get( ui->lineEdit_q->text().toStdString() );  //
    x.get( ui->lineEdit_x->text().toStdString() );  // секретный ключ x = b ^ r ( mod p )
    b.get( ui->lineEdit_b->text().toStdString() );  // b такое что b^q = 1 ( mod p )
    e.get( ui->lineEdit_e->text().toStdString() );  // первая половинка ЭЦП (хэш от сообщения) e = h( M )
    y.get( ui->lineEdit_y->text().toStdString() );  // вторая половинка ЭЦП y = ( a * e + r ) ( mod q )
    v.get( ui->lineEdit_v->text().toStdString() );  // открытый ключ v = b ^ ( q - a ) ( mod p )

    // ПРОВЕРЯЕМ ЭЦП

    //**************************************************************

    // temp = ( v^e * b^y ) ( mod p )
    temp1 = powm( v, e, p );
    temp2 = powm( b, y, p );
    temp = temp1 * temp2;
    temp %= p;

    // вывод результата на экран
    if ( temp == x ) ui->lineEdit->setText( QString::fromLocal8Bit("Подпись подлинная!") );
    else ui->lineEdit->setText( QString::fromLocal8Bit("Подпись подделанная!") );
}

// инициализация и генерация входных параметров
void MainWindow::Inicialization() {
    if ( ui->lineEdit_M->text().isEmpty() ) ui->lineEdit_M->setText( "MESSAGE" );
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // закрашиваем общедоступные и скрытые параметры в нужные цвета
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Base,Qt::green);
    ui->lineEdit_p->setPalette(*palette);
    ui->lineEdit_q->setPalette(*palette);
    ui->lineEdit_b->setPalette(*palette);
    ui->lineEdit_v->setPalette(*palette);
    palette->setColor(QPalette::Base,Qt::yellow);
    ui->lineEdit_e->setPalette(*palette);
    ui->lineEdit_y->setPalette(*palette);

    connect(ui->pushButton_Subscribe, SIGNAL( clicked() ), SLOT( Subscribe() ));
    connect(ui->pushButton_Verify, SIGNAL( clicked() ), SLOT( Verify() ));

    string str;

    // стение сообщения из файла
    ifstream M_file("M.txt");
    M_file >> str;
    ui->lineEdit_M->setText( QString::fromStdString( str ) );
    M_file.close();

    // чтение данных из файла
    ifstream data_file("data.txt");
    data_file >> str;
    ui->lineEdit_p->setText( QString::fromStdString( str ) );
    data_file >> str;
    ui->lineEdit_q->setText( QString::fromStdString( str ) );
    data_file >> str;
    ui->lineEdit_b->setText( QString::fromStdString( str ) );
    data_file >> str;
    ui->lineEdit_r->setText( QString::fromStdString( str ) );
    data_file >> str;
    ui->lineEdit_a->setText( QString::fromStdString( str ) );
    data_file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
