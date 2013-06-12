#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    // генерирование входных параметров
    void generate_m();
};

#endif // MAINWINDOW_H
