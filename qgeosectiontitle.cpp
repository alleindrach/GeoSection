#include "qgeosectiontitle.h"
#include <QGraphicsWidget>
#include "QGraphicsScene"
#include <QGraphicsLayoutItem>
#include <QSizeF>
#include <QPainter>

QGeoSectionTitle::QGeoSectionTitle(QSection *section, QGraphicsItem *parent):QGraphicsWidget(parent),_section(section)
{

}

void QGeoSectionTitle::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
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
    painter->drawText(titleBox,Qt::AlignLeft|Qt::AlignVCenter,_section->left()->name());
    if(this->last()){
        painter->drawText(titleBox,Qt::AlignRight|Qt::AlignVCenter,_section->right()->name());
    }
    painter->drawText(titleBox,Qt::AlignHCenter|Qt::AlignVCenter,QString::number(_section->distance()/X_SCALE,'f',2));
    painter->setPen(QPen(QBrush(DARK_RED),1,Qt::SolidLine));
    painter->drawRect(boundingRect);
    painter->restore();
}

QSizeF QGeoSectionTitle::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
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

bool QGeoSectionTitle::last() const
{
    return _last;
}

void QGeoSectionTitle::setLast(bool last)
{
    _last = last;
}
