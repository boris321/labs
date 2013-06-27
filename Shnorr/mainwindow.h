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
    void Subscribe(); // выработать Ё÷ѕ
    void Verify(); // проверить Ё÷ѕ
    void Inicialization(); // инициализаци€ и генераци€ входных параметров
};

#endif // MAINWINDOW_H
