#include "mainwindow.h"
#include <QApplication>
#include <QHeaderView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.properGeometry();
    return a.exec();
}
