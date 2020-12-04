#include "mainwindow.h"

#include <QApplication>
#include "types.h"

// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.


int main(int argc, char *argv[])
{
//    qRegisterMetaType<QGeoLayer>("QGeoLayer");
//    qRegisterMetaType<QWellbore>("QWellbore");


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
