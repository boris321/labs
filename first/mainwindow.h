#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString filenameKey;
    QString filenameVector;
    bool enc;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void openInput();
    void openKey();
    void openVector();
    void saveResult();
    void encrypt();
    void decrypt();
    void display();
    void hide();
    void changedAlgorithm();
    void generateVector();
    void generateKey();
    void about();
    void claerInput();
    void update();
    void result();
    void result( bool );
    unsigned int lengthBlock();
    unsigned int lengthKey();
};

void DES        ( const QString, const QString, bool, QString, const QString );
void GOST       ( const QString, const QString, bool, QString, const QString );
void AES        ( const QString, const QString, bool, QString, const QString );
void IDEA       ( const QString, const QString, bool, QString, const QString );
void MD5        ( const QString );
void GOSTHASH   ( const QString );
void SHA1       ( const QString );
void RIPEMD     ( const QString );

char * QStringToCharStr(QString);
QString FileToQString(QString);
QString FileToQString16(QString);
void generateV( unsigned int );
void generateK( unsigned int );
void saveR( QString );
void CreateXOR( QString );

#endif // MAINWINDOW_H
