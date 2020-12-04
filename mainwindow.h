#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.


#include <QMainWindow>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
//    void on_hover_data(QPointF pos,QString comp,QString des);
    void on_actionExport_triggered();

private:
    Ui::MainWindow *ui;

    QLabel * _statusComponenet;
    QLabel * _statusData;
    QLabel * _statusDepth;
};
#endif // MAINWINDOW_H
