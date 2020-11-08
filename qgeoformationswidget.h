#ifndef QGEOLAYERSWIDGET_H
#define QGEOLAYERSWIDGET_H

#include "types.h"
#include <QPainter>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <qgeosectioncontent.h>
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
        void drawLayers(QPainter *painter);
        bool _last=false;

};

#endif // QGEOLAYERSWIDGET_H
