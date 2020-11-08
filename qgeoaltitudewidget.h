#ifndef QGEOALTITUDEWIDGET_H
#define QGEOALTITUDEWIDGET_H


#include "types.h"
#include <QPainter>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <qgeosectioncontent.h>


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
