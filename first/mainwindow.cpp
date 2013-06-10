#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QRegExp>
#include <QMessageBox>
#include <string>

using namespace std;

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    connect(ui->pushButtonOpenInput,    SIGNAL( clicked() ),                        SLOT( openInput() ));
    connect(ui->pushButtonOpenKey,      SIGNAL( clicked() ),                        SLOT( openKey() ));
    connect(ui->pushButtonOpenVector,   SIGNAL( clicked() ),                        SLOT( openVector() ));
    connect(ui->pushButtonSaveResult,   SIGNAL( clicked() ),                        SLOT( saveResult() ));
    connect(ui->pushButtonEncrypt,      SIGNAL( clicked() ),                        SLOT( encrypt() ));
    connect(ui->pushButtonDecrypt,      SIGNAL( clicked() ),                        SLOT( decrypt() ));
    connect(ui->pushButtonVector,       SIGNAL( clicked() ),                        SLOT( generateVector() ));
    connect(ui->pushButtonKey,          SIGNAL( clicked() ),                        SLOT( generateKey() ));
    connect(ui->pushButtonDisplay,      SIGNAL( clicked() ),                        SLOT( display() ));
    connect(ui->pushButtonHide,         SIGNAL( clicked() ),                        SLOT( hide() ));
    connect(ui->pushButtonAbout,        SIGNAL( clicked() ),                        SLOT( about() ));
    connect(ui->pushButtonFileInput,    SIGNAL( clicked() ),                        SLOT( claerInput() ));
    connect(ui->pushButtonResult,       SIGNAL( clicked() ),                        SLOT( result() ));
    connect(ui->comboBoxAlgorithm,      SIGNAL( currentIndexChanged( QString )),    SLOT( changedAlgorithm() ));
    connect(ui->comboBoxMode,           SIGNAL( currentIndexChanged( QString )),    SLOT( changedAlgorithm() ));
    connect(ui->comboBoxGOST,           SIGNAL( currentIndexChanged( QString )),    SLOT( changedAlgorithm() ));

    enc = true;
    changedAlgorithm();
    hide();
    update();
    about();
}

void MainWindow::openKey()
{
    filenameKey = QFileDialog::getOpenFileName( this, QString::fromLocal8Bit( "Открыть файл с ключём" ) );
    update();
}

void MainWindow::openVector()
{
    filenameVector = QFileDialog::getOpenFileName( this, QString::fromLocal8Bit( "Открыть файл с вектором" ) );
    update();
}

void MainWindow::openInput()
{
    QString str = QFileDialog::getOpenFileName( this, QString::fromLocal8Bit( "Открыть файл с данныими" ) );
    ui->lineEditInputFile->setText( str );
    update();
}

void MainWindow::saveResult()
{
    QString str = QFileDialog::getOpenFileName( this, QString::fromLocal8Bit( "Сохранить файл с результатом" ) );
    if ( !str.isEmpty() ) {
        saveR( str );
    }
}

void MainWindow::display()
{
    // mode
    ui->pushButtonMode->        setVisible( false );
    // modes
    ui->comboBoxMode->          setVisible( false );
    ui->comboBoxGOST->          setVisible( false );
    ui->comboBoxAES->           setVisible( false );
    // key
    ui->pushButtonKey->         setVisible( false );
    ui->lineEditKey->           setVisible( false );
    ui->pushButtonOpenKey->     setVisible( false );
    // vector
    ui->pushButtonVector->      setVisible( false );
    ui->lineEditVector->        setVisible( false );
    ui->pushButtonOpenVector->  setVisible( false );
    // display
    ui->pushButtonDisplay->     setVisible( false );
    ui->pushButtonHide->        setVisible( true );
    // decrypt
    ui->pushButtonDecrypt->     setVisible( false );
    // about
    ui->pushButtonAbout->       setVisible( false );
    ui->textEditAbout->         setVisible( false );
    // input
    ui->pushButtonInput->       setVisible( true );
    ui->textEditInput->         setVisible( true );
    // result
    ui->pushButtonResult->      setVisible( true );
    ui->textEditResult->        setVisible( true );

    QString algorithm = ui->comboBoxAlgorithm->currentText();
    QString mode;

    if ( algorithm == "DES" ) {
        // mode
        ui->pushButtonMode->        setVisible( true );
        // modes
        ui->comboBoxMode->          setVisible( true );
        // key
        ui->pushButtonKey->         setVisible( true );
        ui->lineEditKey->           setVisible( true );
        ui->pushButtonOpenKey->     setVisible( true );
        // decrypt
        ui->pushButtonDecrypt->     setVisible( true );

        mode = ui->comboBoxMode->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            // vector
            ui->pushButtonVector->      setVisible( true );
            ui->lineEditVector->        setVisible( true );
            ui->pushButtonOpenVector->  setVisible( true );
        }
    }

    if ( algorithm == "GOST" ) {
        // mode
        ui->pushButtonMode->        setVisible( true );
        // modes
        ui->comboBoxGOST->          setVisible( true );
        // key
        ui->pushButtonKey->         setVisible( true );
        ui->lineEditKey->           setVisible( true );
        ui->pushButtonOpenKey->     setVisible( true );
        // decrypt
        ui->pushButtonDecrypt->     setVisible( true );

        mode = ui->comboBoxGOST->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            // vector
            ui->pushButtonVector->      setVisible( true );
            ui->lineEditVector->        setVisible( true );
            ui->pushButtonOpenVector->  setVisible( true );
        }
        if ( mode == QString::fromLocal8Bit( "Имитовставка" ) ) {
            // decrypt
            ui->pushButtonDecrypt->     setVisible( false );
        }
    }

    if ( algorithm == "AES" ) {
        // mode
        ui->pushButtonMode->        setVisible( true );
        // modes
        ui->comboBoxMode->          setVisible( true );
        ui->comboBoxAES->          setVisible( true );
        // key
        ui->pushButtonKey->         setVisible( true );
        ui->lineEditKey->           setVisible( true );
        ui->pushButtonOpenKey->     setVisible( true );
        // decrypt
        ui->pushButtonDecrypt->     setVisible( true );

        mode = ui->comboBoxMode->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            // vector
            ui->pushButtonVector->      setVisible( true );
            ui->lineEditVector->        setVisible( true );
            ui->pushButtonOpenVector->  setVisible( true );
        }
    }

    if ( algorithm == "IDEA" ) {
        // mode
        ui->pushButtonMode->        setVisible( true );
        // modes
        ui->comboBoxMode->          setVisible( true );
        // key
        ui->pushButtonKey->         setVisible( true );
        ui->lineEditKey->           setVisible( true );
        ui->pushButtonOpenKey->     setVisible( true );
        // decrypt
        ui->pushButtonDecrypt->     setVisible( true );

        mode = ui->comboBoxMode->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            // vector
            ui->pushButtonVector->      setVisible( true );
            ui->lineEditVector->        setVisible( true );
            ui->pushButtonOpenVector->  setVisible( true );
        }
    }

    lengthBlock();
    lengthKey();
    update();
}

void MainWindow::hide()
{
    ui->pushButtonDisplay-> setVisible( true );
    ui->pushButtonHide->    setVisible( false );
    ui->pushButtonAbout->   setVisible( true );
    ui->textEditAbout->     setVisible( true );
    ui->pushButtonInput->   setVisible( false );
    ui->textEditInput->     setVisible( false );
    ui->pushButtonResult->  setVisible( false );
    ui->textEditResult->    setVisible( false );
    update();
}

void MainWindow::changedAlgorithm()
{
    bool flag = ui->pushButtonDisplay->isVisible();
    display();
    if ( flag ) {
        hide();
    }
    update();
}

void MainWindow::encrypt()
{
    QString algorithm = ui->comboBoxAlgorithm->currentText();
    QString mode;
    QString filenameInput = ui->lineEditInputFile->text();
    if ( filenameInput.isEmpty() ) {
        QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Не задан файл с данными"));
        return;
    }
    if ( algorithm == "DES" ) {
        mode = ui->comboBoxMode->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            if ( filenameVector.isEmpty() ) {
                QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Вектор не задан"));
                return;
            }
        }
    }
    if ( algorithm == "GOST" ) {
        mode = ui->comboBoxGOST->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            if ( filenameVector.isEmpty() ) {
                QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Вектор не задан"));
                return;
            }
        }
    }
    if ( algorithm == "AES" ) {
        mode = ui->comboBoxMode->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            if ( filenameVector.isEmpty() ) {
                QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Вектор не задан"));
                return;
            }
        }
        mode += ui->comboBoxAES->currentText();
    }
    if ( algorithm == "IDEA" ) {
        mode = ui->comboBoxMode->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            if ( filenameVector.isEmpty() ) {
                QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Вектор не задан"));
                return;
            }
        }
    }
    if ( algorithm == "DES" || algorithm == "GOST" || algorithm == "AES" || algorithm == "IDEA" ) {
        if ( filenameKey.isEmpty() ) {
            QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Ключ не задан"));
            return;
        }
    }

    if ( algorithm == "DES" )         DES         ( filenameInput, filenameKey, 1, mode, filenameVector );
    if ( algorithm == "GOST" )        GOST        ( filenameInput, filenameKey, 1, mode, filenameVector );
    if ( algorithm == "AES" )         AES         ( filenameInput, filenameKey, 1, mode, filenameVector );
    if ( algorithm == "IDEA" )        IDEA        ( filenameInput, filenameKey, 1, mode, filenameVector );
    if ( algorithm == "MD5" )         MD5         ( filenameInput );
    if ( algorithm == "GOSTHASH" )    GOSTHASH    ( filenameInput );
    if ( algorithm == "SHA1" )        SHA1        ( filenameInput );
    if ( algorithm == "RIPEMD" )      RIPEMD      ( filenameInput );

    if ( algorithm == "SHA1" ) {
        result( 0 );
    }
    else {
        result( 1 );
    }
    update();
}

void MainWindow::decrypt()
{
    QString algorithm = ui->comboBoxAlgorithm->currentText();
    QString mode;
    QString filenameInput = ui->lineEditInputFile->text();
    QString str;
    if ( filenameInput.isEmpty() ) {
        QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Не задан файл с данными"));
        return;
    }
    if ( algorithm == "DES" ) {
        mode = ui->comboBoxMode->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            if ( filenameVector.isEmpty() ) {
                QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Вектор не задан"));
                return;
            }
        }
    }
    if ( algorithm == "GOST" ) {
        mode = ui->comboBoxGOST->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            if ( filenameVector.isEmpty() ) {
                QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Вектор не задан"));
                return;
            }
        }
    }
    if ( algorithm == "AES" ) {
        mode = ui->comboBoxMode->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            if ( filenameVector.isEmpty() ) {
                QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Вектор не задан"));
                return;
            }
        }
        mode += ui->comboBoxAES->currentText();
    }
    if ( algorithm == "IDEA" ) {
        mode = ui->comboBoxMode->currentText();
        if ( mode == "CBC" || mode == "CFB" || mode == "OFB" ) {
            if ( filenameVector.isEmpty() ) {
                QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Вектор не задан"));
                return;
            }
        }
    }
    if ( algorithm == "DES" || algorithm == "GOST" || algorithm == "AES" || algorithm == "IDEA" ) {
        if ( filenameKey.isEmpty() ) {
            QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Ключ не задан"));
            return;
        }
    }

    if ( algorithm == "DES" )     DES     ( filenameInput, filenameKey, 0, mode, filenameVector );
    if ( algorithm == "GOST" )    GOST    ( filenameInput, filenameKey, 0, mode, filenameVector );
    if ( algorithm == "AES" )     AES     ( filenameInput, filenameKey, 0, mode, filenameVector );
    if ( algorithm == "IDEA" )    IDEA    ( filenameInput, filenameKey, 0, mode, filenameVector );

    result( 0 );
    update();
}

void MainWindow::generateVector()
{
    unsigned int size = lengthBlock();
    generateV( size );
    filenameVector = "generatevector";
    update();
}

void MainWindow::generateKey()
{
    unsigned int size = lengthKey();
    generateK( size );
    filenameKey = "generatekey";
    update();
}

void MainWindow::about()
{
    QString str = FileToQString( "about/about.txt" );
    ui->textEditAbout->setText( QString::fromLocal8Bit( QStringToCharStr( str ), str.size() ));
}

void MainWindow::claerInput()
{
    ui->lineEditInputFile->setText( "" );
    update();
}

unsigned int MainWindow::lengthKey()
{
    QString str = ui->comboBoxAlgorithm->currentText();
    unsigned int size;
    if ( str == "DES" )     size = 8;
    if ( str == "GOST" )    size = 32;
    if ( str == "AES" ) {
        str = ui->comboBoxAES->currentText();
        if ( str == "128" ) size = 16;
        if ( str == "192" ) size = 24;
        if ( str == "256" ) size = 32;
    }
    if ( str == "IDEA" ) size = 16;
    ui->lineEditKey->setMaxLength( size );
    return size;
}

unsigned int MainWindow::lengthBlock()
{
    QString str = ui->comboBoxAlgorithm->currentText();
    unsigned int size;
    if ( str == "DES" )     size = 8;
    if ( str == "GOST" )    size = 8;
    if ( str == "AES" )     size = 16;
    if ( str == "IDEA" )    size = 8;
    ui->lineEditVector->setMaxLength( size );
    return size;
}

void MainWindow::update()
{
    QString str;

    str = FileToQString( filenameKey );
    ui->lineEditKey->setText( QString::fromLocal8Bit( QStringToCharStr( str ), str.size() ) );

    str = FileToQString( filenameVector );
    ui->lineEditVector->setText( QString::fromLocal8Bit( QStringToCharStr( str ), str.size() ) );

    str = ui->lineEditInputFile->text();
    str = FileToQString( str );
    ui->textEditInput->setText( QString::fromLocal8Bit( QStringToCharStr( str ), str.size() ) );

    str = ui->comboBoxAlgorithm->currentText();
    str = FileToQString( "about/" + str + ".txt" );
    ui->textEditAbout->setText( QString::fromLocal8Bit( QStringToCharStr( str ), str.size() ));
}

void MainWindow::result( bool enc )
{
    QString str = FileToQString( "result" );
    if ( enc ) ui->textEditResult->setText( FileToQString16( "result" ));
    if ( !enc ) ui->textEditResult->setText( QString::fromLocal8Bit( QStringToCharStr( str ), str.size() ) );
    QMessageBox::information(this, QString::fromLocal8Bit("Готово"), QString::fromLocal8Bit("Готово"));
}

void MainWindow::result()
{
    QString str = FileToQString( "result" );
    if ( enc ) ui->textEditResult->setText( FileToQString16( "result" ));
    else ui->textEditResult->setText( QString::fromLocal8Bit( QStringToCharStr( str ), str.size() ) );
    enc = !enc;
}

MainWindow::~MainWindow()
{
    delete ui;
}
