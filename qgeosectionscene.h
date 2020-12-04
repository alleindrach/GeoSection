#ifndef QGEOSECTIONSCENE_H
#define QGEOSECTIONSCENE_H
// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.
#include <QGraphicsScene>
#include <QObject>
#include <QWidget>
#include <types.h>
#include <QGraphicsLinearLayout>
#include <QVector>
#include "qgeosectionwidget.h"
//图像管理控件
class QGeoSectionScene : public QGraphicsScene
{
    Q_OBJECT
public:
    QGeoSectionScene(QObject *parent  = nullptr);
    ~QGeoSectionScene(){
//        QList<QString >keys=_legends.keys();
//        for(int i=0;i<keys.size();i++){
//            delete _legends[keys[i]];
//        }
    };
    //添加双井间剖面
    void AddSection(QGeoSectionWidget *section, int pos=-1,int stretchFactor=0);
    QGraphicsWidget *  topWidget(){
        return _form;
    }
   static  void addLegend(QString ,Qt::BrushStyle,QColor);
   static QBrush&  getLegend(QString);
   static QMap<QString,QBrush> _legends;
private:

    QGraphicsProxyWidget * _wellTitle; //图像头
    QGraphicsWidget  * _form; //顶层元素
    QGraphicsLinearLayout * _sectionLayout;
    QGraphicsLinearLayout * _sectionLayoutOuter;
    QVector<QGeoSectionWidget *> _sections;//双井间剖面数组，每两个相邻井眼对应一个
    int _titleHeight{0};

};

#endif // QGEOSECTIONSCENE_H
