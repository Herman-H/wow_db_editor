#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug("Starting program");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
