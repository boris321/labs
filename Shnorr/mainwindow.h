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

private slots:
    void Subscribe(); // ���������� ���
    void Verify(); // ��������� ���
    void Inicialization(); // ������������� � ��������� ������� ����������
};

#endif // MAINWINDOW_H
