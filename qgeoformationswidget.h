#ifndef QGEOLAYERSWIDGET_H
#define QGEOLAYERSWIDGET_H
// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.
#include "types.h"
#include <QPainter>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <qgeosectioncontent.h>
//地层栏
class QGeoFormationsWidget:public QGeoSectionContent
{

        Q_OBJECT
    public:
        QGeoFormationsWidget(QSection * section,QRectF ticks,QGraphicsItem * parent = nullptr);
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                           QWidget *widget) override;
        ~QGeoFormationsWidget(){

        }
        virtual QString dataAtPos(QPointF pos);
        QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const override;
        bool last() const;
        void setLast(bool last);

private:
        QSection* _section;
        //绘制两个相邻井眼间的地层图
        void drawLayers(QPainter *painter);
        bool _last=false;

};

#endif // QGEOLAYERSWIDGET_H
