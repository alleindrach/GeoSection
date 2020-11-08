#include "qgeoaltitudetitle.h"

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <types.h>
#include <QGraphicsWidget>
QGeoAltitudeTitle::QGeoAltitudeTitle(QRectF ticks, QGraphicsItem *parent):QGeoTitle(parent),_ticks(ticks)
{

}

void QGeoAltitudeTitle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QRectF boundingRect=this->boundingRect();
    QRectF titleBox(0,0,boundingRect.width(),GEO_TITLE_ITEM_HEIGHT);
    QFont font=painter->font();
    font.setPixelSize(8);
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(QPen(QBrush(Qt::white),1,Qt::SolidLine));
    painter->fillRect(titleBox,QBrush(DARK_RED));
    painter->drawText(titleBox,Qt::AlignCenter,this->title());
    painter->setPen(QPen(QBrush(DARK_RED),1,Qt::SolidLine));
    painter->drawRect(boundingRect);
    painter->restore();
}

QSizeF QGeoAltitudeTitle::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    if(which==Qt::PreferredSize){
        int height=_height;
        int width=GEO_SECTION_MIN_WIDTH;
        if(height<=0){
            height=GEO_TITLE_ITEM_HEIGHT;
        }
        if(this->parentItem()==nullptr){
            return QSizeF(GEO_SECTION_MIN_WIDTH,height);
        }else{
            QRectF parentBoundingRect=this->parentItem()->boundingRect();
            if(parentBoundingRect.width()>GEO_SECTION_MIN_WIDTH){
                width=parentBoundingRect.width();
            }
            return QSizeF(width,height);
        }
    }else {
        return QGraphicsWidget::sizeHint(which,constraint);
    }
}
