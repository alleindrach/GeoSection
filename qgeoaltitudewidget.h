#ifndef QGEOALTITUDEWIDGET_H
#define QGEOALTITUDEWIDGET_H

// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.

#include "types.h"
#include <QPainter>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <qgeosectioncontent.h>

//高程的标注栏
class QGeoAltitudeWidget:public QGeoSectionContent
{
        Q_OBJECT
    public:
        QGeoAltitudeWidget(QRectF ticks,QGraphicsItem * parent = nullptr);
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                           QWidget *widget) override;
        ~QGeoAltitudeWidget(){

        }
        virtual QString dataAtPos(QPointF pos);
        QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const override;


private:

};

#endif // QGEOALTITUDEWIDGET_H
