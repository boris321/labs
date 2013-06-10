#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "superlong.h"
#include "md5.h"
#include <string>
#include <fstream>
#include <QTime>

using namespace std;

// ИГРА - ОСНОВНОЙ АЛГОРИТМ

void MainWindow::Play()
{
    Inicialization(); // инициализация и генерация входных параметров

    QString messageB, hashB, hashA, guessA, guessB, resultB, message2B, message2A, hash2A, resultA;

    // Боб выбирает орел/решка
    messageB = ui->lineEdit_messageBob->text(); // выбор Боба
    // Боб определяет хэш сообщения
    hashB = hash( messageB );
    // Боб отправляет Алисе хэш
    hashA = hashB;

    // Алиса делает догадку орел/решка
    guessA = ui->lineEdit_guessAlisa->text();   // догадка Алисы
    // Алиса отправляет Бобу догадку
    guessB = guessA;

    // Боб определяет результат
    if ( guessB[ 0 ] == messageB[ 0 ] ) resultB = "I know I'm a Loser!";
    else resultB = "I'm a Winner!";
    // Боб выбирает сообщение для отправки
    message2B = ui->lineEdit_message2Bob->text(); // сообщение для отправки
    // Боб отправляет ключ
    message2A = message2B;

    // Алиса определяет хэш сообщение
    hash2A = hash( message2A );
    // Если хэши не совпадают, то Боб лжец
    if ( hash2A != hashA ) resultA = "Bob a Liar!";
    else{
        if ( message2A[ 0 ] == guessA[ 0 ] ) resultA = "I'm a Winner!";
        else resultA = "I'm a Loser!";
    }

    // Вывод результатов на экран
    ui->lineEdit_hashBob->setText( hashB );
    ui->lineEdit_hashAlisa->setText( hashA );
    ui->lineEdit_guessBob->setText( guessB );
    ui->lineEdit_message2Alisa->setText( message2A );
    ui->lineEdit_hash2Alisa->setText( hash2A );
    ui->label_ResultBob->setText( resultB );
    ui->label_ResultAlisa->setText( resultA );
}

// ИНИЦИАЛИЗАЦИЯ И ГЕНЕРАЦИЯ ВХОДНЫХ ПАРАМЕТРОВ

void MainWindow::Inicialization() {
    QString str;
    str = ui->lineEdit_messageBob->text();
    if ( str.isEmpty() ) generate_message();
    str = ui->lineEdit_guessAlisa->text();
    if ( str.isEmpty() ) generate_guess();
    str = ui->lineEdit_message2Bob->text();
    if ( str.isEmpty() ) ui->lineEdit_message2Bob->setText( ui->lineEdit_messageBob->text() );
}

void MainWindow::generate_message() {
    QString str = ui->lineEdit_messageBob->text();
    if ( str.isEmpty() ) str = "1";
    if ( str.left( 1 ) == "1" ) str = "0";
    else str = "1";
    for ( unsigned int i = 0 ; i < 7 ; i++ ) str += rand() % 10 + '0';
    ui->lineEdit_messageBob->setText( str );
    ui->lineEdit_message2Bob->setText( str );
}

void MainWindow::generate_guess() {
    QString str = ui->lineEdit_guessAlisa->text();
    if ( str.isEmpty() ) str = "1";
    if ( str.left( 1 ) == "1" ) str = "0";
    else str = "1";
    str += "*******";
    ui->lineEdit_guessAlisa->setText( str );
}

void MainWindow::generate_message2() {
    QString str = ui->lineEdit_message2Bob->text();
    if ( str.isEmpty() ) str = "1";
    if ( str.left( 1 ) == "1" ) str = "0";
    else str = "1";
    for ( unsigned int i = 0 ; i < 7 ; i++ ) str += rand() % 10 + '0';
    ui->lineEdit_message2Bob->setText( str );
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_Play, SIGNAL( clicked() ), SLOT( Play() ));
    connect(ui->pushButton_messageBob, SIGNAL( clicked() ), SLOT( generate_message() ));
    connect(ui->pushButton_guessAlisa, SIGNAL( clicked() ), SLOT( generate_guess() ));
    connect(ui->pushButton_message2Bob, SIGNAL( clicked() ), SLOT( generate_message2() ));

    // инициализация генератора случайных чисел
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
