#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "superlong.h"
#include <QTime>

using namespace std;

// используемые большие числа
superlong p, g, xB, yB, yA, bA, kA, rA, rB, cB, cA, b2A, k2A, b2B, k2B, r2B;

// игра
void MainWindow::Play()
{
    Inicialization(); // инициализация и генерация входных параметров

    // Доверенный центр выбирает простое p и порождающий элемент g
    p.get( ui->lineEdit_p->text().toStdString() );
    g.get( ui->lineEdit_g->text().toStdString() );

    // Боб выбирает x
    xB.get( ui->lineEdit_xBob->text().toStdString() );
    // Боб вычисляет y = g^x ( mod p )
    yB = powm( g, xB, p);
    // Боб отправляет Алисе y
    yA = yB;

    // Алиса выбирает орел/решка b и k
    bA.get( ui->lineEdit_bAlisa->text().toStdString() );
    kA.get( ui->lineEdit_kAlisa->text().toStdString() );
    // Алиса вычисляет r = y^b * g^k ( mod p )
    rA = ( powm( yA, bA, p ) * powm( g, kA, p )) % p;
    // Алиса отправляет Бобу r
    rB = rA;

    // Боб выбирает орла/решку c
    cB.get( ui->lineEdit_cBob->text().toStdString() );
    // Боб отправляет Алисе с
    cA = cB;

    // Алиса определяет результат
    if ( cA == bA ) resultA = "I know I'm a Loser!";
    else resultA = "I'm a Winner!";
    // Алиса выбирает орла/решку b и k, которых отправит Бобу
    b2A.get( ui->lineEdit_b2Alisa->text().toStdString() );
    k2A.get( ui->lineEdit_k2Alisa->text().toStdString() );
    // Алиса отправляет Бобу b2 и k2
    b2B = b2A;
    k2B = k2A;

    // Боб вычисляет r = ( y^b * g^k ) ( mod p )
    r2B = ( powm( yB, b2B, p ) * powm( g, k2B, p )) % p;
    // Боб определяет результат
    // если значения r не совпадают => Алиса хотела схитрить
    if ( r2B != rB ) resultB = "Alice a Liar!";
    else {
        // если догадка Боба совпала с загадкой Алисы
        if ( b2B == cB ) resultB = "I'm a Winner!";
        else resultB = "I'm a Loser!";
    }

    Show(); // вывод результата игры на экран
}

 // инициализация и генерация входных параметров
void MainWindow::Inicialization () {

    if ( ui->lineEdit_p->text().isEmpty() ) {
        generate_p();
    }
    if ( ui->lineEdit_g->text().isEmpty() ) {
        generate_g();
    }
    if ( ui->lineEdit_xBob->text().isEmpty() ) {
        generate_xB();
    }
    if ( ui->lineEdit_bAlisa->text().isEmpty() ) {
        ui->lineEdit_bAlisa->setText( QString::number( rand() % 2 ) );
    }
    if ( ui->lineEdit_kAlisa->text().isEmpty() ) {
        generate_kA();
    }
    if ( ui->lineEdit_bAlisa->text().isEmpty() ) {
        ui->lineEdit_bAlisa->setText( QString::number( rand() % 2 ) );
    }
    if ( ui->lineEdit_cBob->text().isEmpty() ) {
        ui->lineEdit_cBob->setText( QString::number( rand() % 2 ) );
    }
    if ( ui->lineEdit_b2Alisa->text().isEmpty() ) {
        ui->lineEdit_b2Alisa->setText( QString::number( rand() % 2 ) );
    }
    if ( ui->lineEdit_k2Alisa->text().isEmpty() ) {
        generate_k2A();
    }
}

// вывод результата игры на экран
void MainWindow::Show() {
    yB.give( str );
    ui->lineEdit_yBob->setText( QString::fromStdString( str ));
    yA.give( str );
    ui->lineEdit_yAlisa->setText( QString::fromStdString( str ));
    rA.give( str );
    ui->lineEdit_rAlisa->setText( QString::fromStdString( str ));
    rB.give( str );
    ui->lineEdit_rBob->setText( QString::fromStdString( str ));
    cA.give( str );
    ui->lineEdit_cAlisa->setText( QString::fromStdString( str ));
    b2B.give( str );
    ui->lineEdit_b2Bob->setText( QString::fromStdString( str ));
    k2B.give( str );
    ui->lineEdit_k2Bob->setText( QString::fromStdString( str ));
    r2B.give( str );
    ui->lineEdit_r2Bob->setText( QString::fromStdString( str ));
    ui->label_ResultAlisa->setText( resultA );
    ui->label_ResultBob->setText( resultB );
}

// генерирование p
void MainWindow::generate_p() {
    p = generatep( "1000000" );
    p.give( str );
    ui->lineEdit_p->setText( QString::fromStdString( str ));
    generate_g();
    generate_xB();
    generate_kA();
}

// генерирование g
void MainWindow::generate_g() {
    if ( ui->lineEdit_p->text().isEmpty() ) generate_p();
    else {
        g.get( ui->lineEdit_p->text().toStdString() );
        g = generate( p );
        g.give( str );
        ui->lineEdit_g->setText( QString::fromStdString( str ));
    }
}

// генерирование xB
void MainWindow::generate_xB() {
    if ( ui->lineEdit_p->text().isEmpty() ) generate_p();
    else {
        p.get( ui->lineEdit_p->text().toStdString() );
        xB = generate( p );
        xB.give( str );
        ui->lineEdit_xBob->setText( QString::fromStdString( str ));
    }
}

// генерирование bA
void MainWindow::generate_bA() {
    if ( ui->lineEdit_bAlisa->text().isEmpty() ) {
        ui->lineEdit_bAlisa->setText( "1" );
    }
    if ( ui->lineEdit_bAlisa->text() == "1" ) {
        ui->lineEdit_bAlisa->setText( "0" );
        ui->lineEdit_b2Alisa->setText( "0" );
    }
    else {
        ui->lineEdit_bAlisa->setText( "1" );
        ui->lineEdit_b2Alisa->setText( "1" );
    }
}

// генерирование kA
void MainWindow::generate_kA() {
    if ( ui->lineEdit_p->text().isEmpty() ) generate_p();
    else {
        p.get( ui->lineEdit_p->text().toStdString() );
        kA = generate( p );
        kA.give( str );
        ui->lineEdit_kAlisa->setText( QString::fromStdString( str ));
        ui->lineEdit_k2Alisa->setText( ui->lineEdit_kAlisa->text() );
    }
}

// генерирование cB
void MainWindow::generate_cB() {
    if ( ui->lineEdit_cBob->text().isEmpty() ) {
        ui->lineEdit_cBob->setText( "1" );
    }
    if ( ui->lineEdit_cBob->text() == "1" ) {
        ui->lineEdit_cBob->setText( "0" );
    }
    else {
        ui->lineEdit_cBob->setText( "1" );
    }
}

// генерирование b2A
void MainWindow::generate_b2A() {
    if ( ui->lineEdit_b2Alisa->text().isEmpty() ) {
        ui->lineEdit_b2Alisa->setText( "1" );
    }
    if ( ui->lineEdit_b2Alisa->text() == "1" ) {
        ui->lineEdit_b2Alisa->setText( "0" );
    }
    else {
        ui->lineEdit_b2Alisa->setText( "1" );
    }
}

// генерирование k2A
void MainWindow::generate_k2A() {
    if ( ui->lineEdit_p->text().isEmpty() ) generate_p();
    else {
        p.get( ui->lineEdit_p->text().toStdString() );
        k2A = generate( p );
        k2A.give( str );
        ui->lineEdit_k2Alisa->setText( QString::fromStdString( str ));
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_Start, SIGNAL( clicked() ), SLOT( Play() ));
    connect(ui->pushButton_p, SIGNAL( clicked() ), SLOT( generate_p() ));
    connect(ui->pushButton_g, SIGNAL( clicked() ), SLOT( generate_g() ));
    connect(ui->pushButton_xBob, SIGNAL( clicked() ), SLOT( generate_xB() ));
    connect(ui->pushButton_bAlisa, SIGNAL( clicked() ), SLOT( generate_bA() ));
    connect(ui->pushButton_kAlisa, SIGNAL( clicked() ), SLOT( generate_kA() ));
    connect(ui->pushButton_cBob, SIGNAL( clicked() ), SLOT( generate_cB() ));
    connect(ui->pushButton_b2Alisa, SIGNAL( clicked() ), SLOT( generate_b2A() ));
    connect(ui->pushButton_k2Alisa, SIGNAL( clicked() ), SLOT( generate_k2A() ));

    // инициализация генератора случайных чисел
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
