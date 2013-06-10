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
    void Play(); // игра
    void Inicialization();  // инициализация и генерация входных параметров
    void Show(); // вывод результата игры на экран
    // генерирование входных параметров
    void generate_p();
    void generate_hB();
    void generate_tB();
    void generate_xA();
    void generate_bA();
    void generate_cB();
    void generate_x2A();

};

#endif // MAINWINDOW_H
