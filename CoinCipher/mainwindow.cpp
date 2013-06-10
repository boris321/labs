#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "superlong.h"
#include <string>
#include <fstream>

using namespace std;

// ����

void MainWindow::Play()
{
    Inicialization(); // ������������� � ��������� ������� ����������

    QString messageB, keyB, cipherB, cipherA, guessA, guessB, resultB, key2B, key2A, messageA, resultA;

    // ��� �������� ����/����� � ����
    messageB = ui->lineEdit_messageBob->text(); // ����� ����
    keyB = ui->lineEdit_keyBob->text();         // ���� ����
    // ��� ������������� ���������
    cipherB = crypt( messageB, keyB, true );
    // ��� ���������� ����� ����
    cipherA = cipherB;

    // ����� ������ ������� ����/�����
    guessA = ui->lineEdit_guessAlisa->text();   // ������� �����
    // ����� ���������� ���� �������
    guessB = guessA;

    // ��� ���������� ���������
    if ( guessB == messageB ) resultB = "I know I'm a Loser!";
    else resultB = "I'm a Winner!";
    // ��� �������� ���� ��� ��������
    key2B = ui->lineEdit_key2Bob->text(); // ���� ��� ��������
    // ��� ���������� ����
    key2A = key2B;

    // ����� �������������� ���������
    messageA = crypt( cipherA, key2A, false );
    // ���� �������������� ��������� �� �������� "OREL" ��� "RESHKA", �� ��� ����
    if ( messageA != "OREL" && messageA != "RESHKA" ) resultA = "Bob a Liar!";
    else{
        if ( messageA == guessA ) resultA = "I'm a Winner!";
        else resultA = "I'm a Loser!";
    }

    // ����� ����������� �� �����
    ui->lineEdit_cipherBob->setText( cipherB );
    ui->lineEdit_cipherAlisa->setText( cipherA );
    ui->lineEdit_guessBob->setText( guessB );
    ui->lineEdit_key2Alisa->setText( key2A );
    ui->lineEdit_messageAlisa->setText( messageA );
    ui->label_ResultBob->setText( resultB );
    ui->label_ResultAlisa->setText( resultA );
}

// ������������� � ��������� ������� ����������

void MainWindow::Inicialization() {
    QString str;
    str = ui->lineEdit_messageBob->text();
    if ( str.isEmpty() ) generate_message();
    str = ui->lineEdit_keyBob->text();
    if ( str.isEmpty() ) generate_key();
    str = ui->lineEdit_guessAlisa->text();
    if ( str.isEmpty() ) generate_guess();
    str = ui->lineEdit_key2Bob->text();
    if ( str.isEmpty() ) ui->lineEdit_key2Bob->setText( ui->lineEdit_keyBob->text() );
}

void MainWindow::generate_message() {
    QString str = ui->lineEdit_messageBob->text();
    if ( str.isEmpty() ) str = "RESHKA";
    if ( str == "RESHKA" ) str = "OREL";
    else str = "RESHKA";
    ui->lineEdit_messageBob->setText( str );
}

void MainWindow::generate_key() {
    QString str;
    for ( unsigned int i = 0 ; i < 8 ; i++ ) str += rand() % 10 + '0';
    ui->lineEdit_keyBob->setText( str );
    ui->lineEdit_key2Bob->setText( str );
}

void MainWindow::generate_key2() {
    QString str;
    for ( unsigned int i = 0 ; i < 8 ; i++ ) str += rand() % 10 + '0';
    ui->lineEdit_key2Bob->setText( str );
}

void MainWindow::generate_guess() {
    QString str = ui->lineEdit_guessAlisa->text();
    if ( str.isEmpty() ) str = "RESHKA";
    if ( str == "RESHKA" ) str = "OREL";
    else str = "RESHKA";
    ui->lineEdit_guessAlisa->setText( str );
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_Play, SIGNAL( clicked() ), SLOT( Play() ));
    connect(ui->pushButton_messageBob, SIGNAL( clicked() ), SLOT( generate_message() ));
    connect(ui->pushButton_keyBob, SIGNAL( clicked() ), SLOT( generate_key() ));
    connect(ui->pushButton_guessAlisa, SIGNAL( clicked() ), SLOT( generate_guess() ));
    connect(ui->pushButton_key2Bob, SIGNAL( clicked() ), SLOT( generate_key2() ));
}

MainWindow::~MainWindow()
{
    delete ui;
}
