#include "qgeosectioncontent.h"
#include <types.h>
#include <QGraphicsItem>
#include <QGraphicsWidget>
#include <QGraphicsObject>
#include <QPainter>

// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.


QGeoSectionContent::QGeoSectionContent(QRectF ticks, QGraphicsItem *parent):
    QGraphicsWidget(parent),_ticks(ticks)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);

}


void QGeoSectionContent::keyPressEvent(QKeyEvent *event)
{

}

void QGeoSectionContent::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void QGeoSectionContent::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

    this->setSelected(true);
    this->update();
}

void QGeoSectionContent::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setSelected(false);
    this->update();
}

void QGeoSectionContent::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{

    QRectF boundingRect=this->boundingRect();
    float depth=event->pos().y()/boundingRect.height()* ticks().height()+ticks().top();
    QPointF p(0,depth);
    QString dataDes=dataAtPos(event->pos());
    emit hoverData(p,dataDes);

}

void QGeoSectionContent::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

}

void QGeoSectionContent::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void QGeoSectionContent::drawGrid(QPainter *painter)
{

    QRectF boundingRect=this->boundingRect();
    QFont font=painter->font();

    font.setPixelSize(8);
    painter->setFont(font);

    //    ticks的 top对齐到y=0，ticks的bottom对齐到boundingRect的底部
    if(this->isSelected()){
        painter->fillRect(boundingRect,MUCH_LIGHT_RED);
    }else{
        painter->fillRect(boundingRect,Qt::white);
    }

    if(_showGrid){
        painter->setPen(QPen(QBrush(LIGHT_GRAY),1,Qt::DashDotLine));

        for(int ypos=boundingRect.top();ypos<boundingRect.bottom();ypos+=_gridSize.height()){
            painter->drawLine(0,ypos,boundingRect.width(),ypos);
        }

        for(int xpos=boundingRect.left();xpos<boundingRect.right();xpos+=_gridSize.width()){
            painter->drawLine(xpos,0,xpos,boundingRect.height());
        }
    }
    drawGround(painter);

}

void QGeoSectionContent::drawGround(QPainter *painter)
{

    QRectF boundingRect=this->boundingRect();
    //    ticks的 top对齐到y=0，ticks的bottom对齐到boundingRect的底部
    float yscale = boundingRect.height()/ticks().height();
    float ypos=(0-ticks().top())*yscale;
    QRectF box{0,ypos-5,boundingRect.width(),5};
    painter->fillRect(box,Qt::BDiagPattern);
    painter->setPen(QPen(QBrush(Qt::black),2,Qt::SolidLine));
    painter->drawLine(0,ypos,boundingRect.width(),ypos);

}
