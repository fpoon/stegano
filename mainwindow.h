#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

#include "coder.h"

#define MODE_NONE 0
#define MODE_MP3  1
#define MODE_WAV  2
#define MODE_BMP  3
#define MODE_JPG  4

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbOpen_clicked();
    void on_pbSave_clicked();

    void on_pbRead_clicked();

    void on_pbHide_clicked();

private:
    Ui::MainWindow *ui;
    int mode = MODE_NONE;
    void changeMode(QString file);
    std::string getMessageFromWidget();
    Coder * coder = nullptr;
};

#endif // MAINWINDOW_H
