#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "superlong.h"
#include <QTime>

using namespace std;

// ������������ ������� �����
superlong p, hB,tB, hA, tA, xA, bA, yA, yB, cB, cA, x2A, x2B, hxB, txB;

// ����
void MainWindow::Play()
{
    Inicialization(); // ������������� � ��������� ������� ����������

    // ���������� ����� �������� ������� p
    p.get( ui->lineEdit_p->text().toStdString() );

    // ��� �������� h � t
    hB.get( ui->lineEdit_hBob->text().toStdString() );
    tB.get( ui->lineEdit_tBob->text().toStdString() );
    // ��� ���������� ����� h � t
    hA = hB;
    tA = tB;

    // ����� �������� x � ����/����� b
    xA.get( ui->lineEdit_xAlisa->text().toStdString() );
    bA.get( ui->lineEdit_bAlisa->text().toStdString() );
    // ����� ��������� y � ����������� �� ���� ���� ��� ����� ��� �������
    if ( bA == 0 ) yA = powm( hA, xA, p );
    else yA = powm( tA, xA, p );
    // ����� ���������� ���� y
    yB = yA;

    // ��� �������� ����/����� c
    cB.get( ui->lineEdit_cBob->text().toStdString() );
    // ��� ���������� ����� �
    cA = cB;

    // ����� ���������� ���������
    // ���� ������� ���� �����
    if ( cA == bA ) resultA = "I know I'm a Loser!";
    else resultA = "I'm a Winner!";
    // ����� �������� x, ������� �������� ����
    x2A.get( ui->lineEdit_x2Alisa->text().toStdString() );
    // ����� ���������� ���� x
    x2B = x2A;

    // ��� ���������� ���������
    hxB = powm( hB, x2B, p );
    txB = powm( tB, x2B, p );
    // ���� �� y ����� t, �� y ����� h �� ������ - ����� ���� ���������, ���� ��� �� ���������� ..
    if ( hxB != yB && txB != yB ) resultB = "Alice a Liar!";
    else {
        // ������������� ����� � ����� � h � t
        if ( cB == 0 ) {
            if ( hxB == yB ) resultB = "I'm a Winner!";
            else resultB = "I'm a Loser!";
        }
        else {
            if ( txB == yB ) resultB = "I'm a Winner!";
            else resultB = "I'm a Loser!";
        }
    }

    Show(); // ����� ���������� ���� �� �����
}

 // ������������� � ��������� ������� ����������
void MainWindow::Inicialization () {

    if ( ui->lineEdit_p->text().isEmpty() ) {
        generate_p();
    }
    if ( ui->lineEdit_hBob->text().isEmpty() ) {
        generate_hB();
    }
    if ( ui->lineEdit_tBob->text().isEmpty() ) {
        generate_tB();
    }
    if ( ui->lineEdit_xAlisa->text().isEmpty() ) {
        generate_xA();
    }
    if ( ui->lineEdit_bAlisa->text().isEmpty() ) {
        ui->lineEdit_bAlisa->setText( QString::number( rand() % 2 ) );
    }
    if ( ui->lineEdit_cBob->text().isEmpty() ) {
        ui->lineEdit_cBob->setText( QString::number( rand() % 2 ) );
    }
    if ( ui->lineEdit_x2Alisa->text().isEmpty() ) {
        generate_x2A();
    }
}

// ����� ���������� ���� �� �����
void MainWindow::Show() {
    hA.give( str );
    ui->lineEdit_hAlisa->setText( QString::fromStdString( str ));
    tA.give( str );
    ui->lineEdit_tAlisa->setText( QString::fromStdString( str ));
    yA.give( str );
    ui->lineEdit_yAlisa->setText( QString::fromStdString( str ));
    yB.give( str );
    ui->lineEdit_yBob->setText( QString::fromStdString( str ));
    cA.give( str );
    ui->lineEdit_cAlisa->setText( QString::fromStdString( str ));
    x2B.give( str );
    ui->lineEdit_x2Bob->setText( QString::fromStdString( str ));
    hxB.give( str );
    ui->lineEdit_hxBob->setText( QString::fromStdString( str ));
    txB.give( str );
    ui->lineEdit_txBob->setText( QString::fromStdString( str ));
    ui->label_ResultAlisa->setText( resultA );
    ui->label_ResultBob->setText( resultB );
}

// ������������� p
void MainWindow::generate_p() {
    p = generatep( "1000000" );
    p.give( str );
    ui->lineEdit_p->setText( QString::fromStdString( str ));
    generate_hB();
    generate_tB();
    generate_xA();
}

// ������������� hB
void MainWindow::generate_hB() {
    if ( ui->lineEdit_p->text().isEmpty() ) generate_p();
    else {
        p.get( ui->lineEdit_p->text().toStdString() );
        hB = generate( p );
        hB.give( str );
        ui->lineEdit_hBob->setText( QString::fromStdString( str ));
    }
}

// ������������� tB
void MainWindow::generate_tB() {
    if ( ui->lineEdit_p->text().isEmpty() ) generate_p();
    else {
        p.get( ui->lineEdit_p->text().toStdString() );
        tB = generate( p );
        tB.give( str );
        ui->lineEdit_tBob->setText( QString::fromStdString( str ));
    }
}

// ������������� xA
void MainWindow::generate_xA() {
    if ( ui->lineEdit_p->text().isEmpty() ) generate_p();
    else {
        p.get( ui->lineEdit_p->text().toStdString() );
        xA = generate( p );
        xA.give( str );
        ui->lineEdit_xAlisa->setText( QString::fromStdString( str ));
        ui->lineEdit_x2Alisa->setText( ui->lineEdit_xAlisa->text() );
    }
}

// ������������� bA
void MainWindow::generate_bA() {
    if ( ui->lineEdit_bAlisa->text().isEmpty() ) {
        ui->lineEdit_bAlisa->setText( "1" );
    }
    if ( ui->lineEdit_bAlisa->text() == "1" ) {
        ui->lineEdit_bAlisa->setText( "0" );
    }
    else {
        ui->lineEdit_bAlisa->setText( "1" );
    }
}

// ������������� cB
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

// ������������� x2A
void MainWindow::generate_x2A() {
    if ( ui->lineEdit_p->text().isEmpty() ) generate_p();
    else {
        p.get( ui->lineEdit_p->text().toStdString() );
        x2A = generate( p );
        x2A.give( str );
        ui->lineEdit_x2Alisa->setText( QString::fromStdString( str ));
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_Start, SIGNAL( clicked() ), SLOT( Play() ));
    connect(ui->pushButton_p, SIGNAL( clicked() ), SLOT( generate_p() ));
    connect(ui->pushButton_hBob, SIGNAL( clicked() ), SLOT( generate_hB() ));
    connect(ui->pushButton_tBob, SIGNAL( clicked() ), SLOT( generate_tB() ));
    connect(ui->pushButton_xAlisa, SIGNAL( clicked() ), SLOT( generate_xA() ));
    connect(ui->pushButton_bAlisa, SIGNAL( clicked() ), SLOT( generate_bA() ));
    connect(ui->pushButton_cBob, SIGNAL( clicked() ), SLOT( generate_cB() ));
    connect(ui->pushButton_x2Alisa, SIGNAL( clicked() ), SLOT( generate_x2A() ));

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
