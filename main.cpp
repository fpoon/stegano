#include "mainwindow.h"
#include <QApplication>

#include "loghelper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    registerLogPte(w.findChild<QPlainTextEdit*>("pteLog"));

    return a.exec();
}
