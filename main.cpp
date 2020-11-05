#include "mainwindow.h"

#include <QApplication>
#include "types.h"
int main(int argc, char *argv[])
{
//    qRegisterMetaType<QGeoLayer>("QGeoLayer");
//    qRegisterMetaType<QWellbore>("QWellbore");


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
