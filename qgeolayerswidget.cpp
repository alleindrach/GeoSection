#include "qgeolayerswidget.h"
#include <types.h>
#include <qgeosectioncontent.h>
#include <qgeosectionscene.h>
QGeoLayersWidget::QGeoLayersWidget( QSection  * section , QRectF ticks, QGraphicsItem *parent):
    QGeoSectionContent(ticks,parent),_section(section)
{
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}
QSizeF QGeoLayersWidget::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    if(which==Qt::PreferredSize){

        return QSizeF(300,500);

    }else {
        return QGraphicsWidget::sizeHint(which,constraint);
    }
}
void QGeoLayersWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->save();
    //    drawGrid(painter);
    //    for(int i=0;i<_curves.size();i++){
    //        QPair<QWMGeoCurveInfo, QPointF *> curve=_curves[i];
    //        drawCurve(painter,curve);
    //    }

    painter->setPen(QPen(QBrush(DARK_RED),1,Qt::SolidLine));
    QRectF boundingRect=this->boundingRect();
    painter->drawRect(boundingRect);
    painter->fillRect(boundingRect,Qt::white);
    painter->setClipRect(boundingRect);
    drawLayers(painter);
    painter->restore();
}

QString QGeoLayersWidget::dataAtPos(QPointF pos)
{
    //    QRectF boundingRect=this->boundingRect();
    //    QString result;
    //    QStringList r;
    //    if(_curves.size()>0){
    //        for(int i=0;i<_curves.size();i++){
    //            float yscale = _curves[i].first.ticks.height()/boundingRect.height();
    //            float depth = pos.y()*yscale;
    //            int pos=Utility::binarySearch(_curves[i].second,0,_curves[i].first.points-1,depth);
    //            QString des=QString("%1-%2").arg(_curves[i].first.title).arg(_curves[i].second[pos].x());
    //            r<<des;
    //        }
    //    }
    //    return r.join(",");
    return "";
}

void QGeoLayersWidget::drawLayers(QPainter * painter)
{

    QTransform oriTransform=painter->transform();
    QRectF boundingRect=this->boundingRect();
    //    QPointF scrollDiff=view->mapFromScene(QPointF(0,0));
    //    ticks的 top对齐到y=0，ticks的bottom对齐到boundingRect的底部


    float xscale = boundingRect.width()/this->ticks().width();
    float yscale = boundingRect.height()/(this->ticks().height()+GROUND_THICKNESS*2);

    painter->setPen(QPen(QBrush(Qt::darkGreen),0.01,Qt::PenStyle::DashLine));

    QTransform  transform;
    qDebug()<<"xs:"<<xscale<<",ys:"<<yscale;
    qDebug()<<"l:"<<this->ticks().left()<<",t:"<<this->ticks().top()<<",r:"<<this->ticks().right()<<",b:"<<this->ticks().bottom();
    transform.scale(xscale,yscale);
    transform.translate(0-this->ticks().left(),0-this->ticks().top()+GROUND_THICKNESS);
    painter->setWorldTransform(transform,true);

    _section->drawGround(painter);

    for(int i=0;i<_section->formations().size();i++){
        _section->formations()[i]->paint(painter);
    }

    for(int i=0;i<_section->left()->samples.size();i++){
        QGeoSample * sample=_section->left()->samples[i];
        sample->paint(painter,0,_section->width());
//        sample->paint(painter,1,_section->width());
    }
    for(int i=0;i<_section->right()->samples.size();i++){
        QGeoSample * sample=_section->right()->samples[i];
//        sample->paint(painter,0,_section->width());
        sample->paint(painter,1,_section->width());
    }
    painter->resetTransform();
    painter->setTransform(oriTransform,false);
}
