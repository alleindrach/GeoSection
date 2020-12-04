#ifndef QGEOSECTIONWIDGET_H
#define QGEOSECTIONWIDGET_H
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsWidget>
#include <types.h>
#include <qgeosectiontitle.h>
#include <qgeosectioncontent.h>
#include <qgeotitle.h>

// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.

//相邻井眼间地址剖面图的包容器，包含标题和剖面图
class QGeoSectionWidget:public QGraphicsWidget
{
    Q_OBJECT
public:
    QGeoSectionWidget(QGeoTitle * title,QGeoSectionContent * content,QGraphicsItem * parent=nullptr);
    int titleContentHight(){
        return _title->contentheight();
    }
    void setTitleHeight(int v) {
        _title->setHeight(v);
    }
signals:
    void hoverData(QPointF pos,QString comp,QString des);
public slots:
    void on_hover_data(QPointF pos,QString des);
private:
    QGeoTitle* _title{nullptr};
    QGeoSectionContent * _content{nullptr};

};

#endif // QGEOSECTIONWIDGET_H
