#ifndef QALTITUDETITLE_H
#define QALTITUDETITLE_H
// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <types.h>
#include <QGraphicsWidget>
#include <qgeotitle.h>
//高程标题栏
class QGeoAltitudeTitle: public QGeoTitle
{
    Q_OBJECT
public:
    QGeoAltitudeTitle(QRectF ticks,  QGraphicsItem * parent);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const override;
    int contentheight() const{
        return GEO_TITLE_ITEM_HEIGHT;
    }
    void setHeight(int v){ _height=v;}
    QString title(){
        return "高程";
    }

private :
    QRectF _ticks;
    int _height{0};

};

#endif // QALTITUDETITLE_H
