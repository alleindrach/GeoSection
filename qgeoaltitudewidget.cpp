#include "qgeoaltitudewidget.h"
#include <types.h>
#include <qgeosectioncontent.h>
#include <qgeosectionscene.h>
QGeoAltitudeWidget::QGeoAltitudeWidget(QRectF ticks, QGraphicsItem *parent): QGeoSectionContent(ticks,parent)
{

}

void QGeoAltitudeWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int barleft=10;
    int barwidth=4;
    painter->save();
    QTransform oriTransform=painter->transform();
    QRectF boundingRect=this->boundingRect();
    painter->fillRect(boundingRect,Qt::white);
    float xscale = 1;
    float yscale = boundingRect.height()/(this->ticks().height()+GROUND_THICKNESS*2);

    painter->setPen(QPen(QBrush(DARK_RED),1,Qt::PenStyle::SolidLine));

    QFont font=painter->font();
    font.setPixelSize(8);

    painter->setFont(font);
    QTransform  transform;

    transform.scale(xscale,-yscale);
    transform.translate(0-this->ticks().left(),-(this->ticks().bottom()+GROUND_THICKNESS));
    QPointF lt=QPointF(barleft,_ticks.top());
    QPointF rb=QPointF(barleft+barwidth,_ticks.bottom());
    QPointF mlt =transform.map(lt);
    QPointF mrb=transform.map(rb);
    painter->fillRect(QRectF(mlt,mrb),DARK_RED);

    int span=ceil(_ticks.height())*100;
    int ascale=ceil(span/100);


    qDebug()<<"span:"<<span<<",ascale:"<<ascale;
    float b =int(_ticks.top()*100/ascale)*ascale/100;
    qDebug()<<"step from:"<<b;
    while(b<_ticks.bottom()){
        QLineF l=QLineF(transform.map(QPointF(barleft,b)),transform.map(QPointF(barleft+barwidth,b)));
        painter->drawLine(l);
        painter->drawText(transform.map(QPointF(barleft+barwidth,b)),QString::number(b,'f',2));
        b+=(float(ascale)/10.0);
        qDebug()<<"step:"<<b;
    }
    painter->resetTransform();
    painter->setTransform(oriTransform,false);
    painter->restore();
}

QString QGeoAltitudeWidget::dataAtPos(QPointF pos)
{
    return "";
}

QSizeF QGeoAltitudeWidget::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    if(which==Qt::PreferredSize){
        return QSizeF(50,500);
    }else {
        return QGraphicsWidget::sizeHint(which,constraint);
    }

}
