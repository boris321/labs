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

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void Start();
    void Inicialization(); // инициализация и генерация входных параметров
    void generate_p();
    void generate_q();
    void generate_n();
    void generate_fi();
    void generate_e();
    void generate_m();
};

#endif // MAINWINDOW_H
