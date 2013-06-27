#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "superlong.h"
#include <string>
#include <fstream>
#include <QTime>

using namespace std;


// ПРОТОКОЛ - ОСНОВНОЙ АЛГОРИТМ

void MainWindow::Start()
{
    Inicialization(); // инициализация и генерация входных параметров

    string str;

    // используемые переменные
    superlong pB, qB, nB, eB, n, e, mA, meA, me, fiB, dB, mB;

    // ПРОТОКОЛ - ОСНОВНОЙ АЛГОРИТМ

    // **************************************************************************

    // Боб выбирает простые p и q
    pB.get( ui->lineEdit_pBob->text().toStdString() );  // простое число
    qB.get( ui->lineEdit_qBob->text().toStdString() );  // простое число
    // Боб вычисляет
    nB = pB * qB;                   // n = p * q
    fiB = ( pB - 1) * ( qB - 1 );   // fi (Функция Эйлера)
    // Боб выбирает e (открытая экспонента)
    eB.get( ui->lineEdit_eBob->text().toStdString() );
    // Боб вычисляет d = e^-1 (закрытая экспонента)
    dB = evklid_ext_x( eB, fiB );
    // Боб публикует n и e
    n = nB;
    e = eB;

    // Алиса выбирает сообщение m
    mA.get( ui->lineEdit_mAlisa->text().toStdString() );
    // Алиса зашифровывает сообщение
    meA = powm( mA, e, n );
    // Алиса публикует зашифрованное сообщение me
    me = meA;

    // Боб расшифровывет сообщение
    mB = powm( me, dB, n );

    // **************************************************************************

    // вывод результата на экран
    nB.give( str );
    ui->lineEdit_nBob->setText( QString::fromStdString( str ) );
    eB.give( str );
    ui->lineEdit_eBob->setText( QString::fromStdString( str ) );
    n.give( str );
    ui->lineEdit_n->setText( QString::fromStdString( str ) );
    e.give( str );
    ui->lineEdit_e->setText( QString::fromStdString( str ) );
    mA.give( str );
    ui->lineEdit_mAlisa->setText( QString::fromStdString( str ) );
    meA.give( str );
    ui->lineEdit_meAlisa->setText( QString::fromStdString( str ) );
    me.give( str );
    ui->lineEdit_me->setText( QString::fromStdString( str ) );
    fiB.give( str );
    ui->lineEdit_fiBob->setText( QString::fromStdString( str ) );
    dB.give( str );
    ui->lineEdit_dBob->setText( QString::fromStdString( str ) );
    mB.give( str );
    ui->lineEdit_mBob->setText( QString::fromStdString( str ) );
}

 // инициализация и генерация входных параметров
void MainWindow::Inicialization()
{
    if ( ui->lineEdit_pBob->text().isEmpty() ) generate_p();
    if ( ui->lineEdit_qBob->text().isEmpty() ) generate_q();
    if ( ui->lineEdit_eBob->text().isEmpty() ) generate_e();
    if ( ui->lineEdit_mAlisa->text().isEmpty() ) generate_m();
}

void MainWindow::generate_p()
{
    superlong p;
    string str;
    p = generatep( "1000000000000" );
    p.give( str );
    ui->lineEdit_pBob->setText( QString::fromStdString( str ));
}

void MainWindow::generate_q()
{
    superlong q;
    string str;
    q = generatep( "1000000000000" );
    q.give( str );
    ui->lineEdit_qBob->setText( QString::fromStdString( str ));
}

void MainWindow::generate_n() {
    if ( ui->lineEdit_pBob->text().isEmpty() ) generate_p();
    if ( ui->lineEdit_qBob->text().isEmpty() ) generate_q();
    superlong p, q, n;
    string str;
    p.get( ui->lineEdit_pBob->text().toStdString() );
    q.get( ui->lineEdit_pBob->text().toStdString() );
    n = p * q;
    n.give( str );
    ui->lineEdit_nBob->setText( QString::fromStdString( str ));
}

void MainWindow::generate_fi() {
    if ( ui->lineEdit_pBob->text().isEmpty() ) generate_p();
    if ( ui->lineEdit_qBob->text().isEmpty() ) generate_q();
    superlong p, q, fi;
    string str;
    p.get( ui->lineEdit_pBob->text().toStdString() );
    q.get( ui->lineEdit_pBob->text().toStdString() );
    fi = ( p - 1 ) * ( q - 1 );
    fi.give( str );
    ui->lineEdit_fiBob->setText( QString::fromStdString( str ));
}

void MainWindow::generate_e() {
    if ( ui->lineEdit_fiBob->text().isEmpty() ) generate_fi();
    superlong fi, e;
    string str;
    fi.get( ui->lineEdit_pBob->text().toStdString() );
    e = generatep( fi );
    e.give( str );
    ui->lineEdit_eBob->setText( QString::fromStdString( str ));
}

void MainWindow::generate_m() {
    ui->lineEdit_mAlisa->setText( "2013" );
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_Start, SIGNAL( clicked() ), SLOT( Start() ));
    connect(ui->pushButton_pBob, SIGNAL( clicked() ), SLOT( generate_p() ));
    connect(ui->pushButton_qBob, SIGNAL( clicked() ), SLOT( generate_q() ));
    connect(ui->pushButton_eBob, SIGNAL( clicked() ), SLOT( generate_e() ));

    // инициализация генератора случайных чисел
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
