#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString qstr, resultA, resultB;
    string str;

private slots:
    void Play(); // ����
    void Inicialization();  // ������������� � ��������� ������� ����������
    void Show(); // ����� ���������� ���� �� �����
    // ������������� ������� ����������
    void generate_p();
    void generate_g();
    void generate_xB();
    void generate_bA();
    void generate_kA();
    void generate_cB();
    void generate_b2A();
    void generate_k2A();

};

#endif // MAINWINDOW_H
