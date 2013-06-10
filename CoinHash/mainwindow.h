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
    void Play(); // игра - основной алгоритм
    void Inicialization(); // инициализация и генерирование входных параметров
    // генерирование входных параметров
    void generate_message();
    void generate_guess();
    void generate_message2();
};

#endif // MAINWINDOW_H
