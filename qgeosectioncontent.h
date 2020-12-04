#ifndef QGEOSECTIONCONTENT_H
#define QGEOSECTIONCONTENT_H
// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneEvent>
#include <QVector>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
//用于显示数据系的GraphicsItem
//data ：float 为y轴坐标，QString分量为标注
class QGeoSectionContent : public QGraphicsWidget
{
    Q_OBJECT
public:
    QGeoSectionContent(QRectF ticks,QGraphicsItem * parent = nullptr);
    ~QGeoSectionContent(){

    }
    //    virtual QRectF boundingRect() const  override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override=0;
    template<typename Base, typename T>
    inline bool instanceof(const T *ptr) {
        return dynamic_cast<const Base*>(ptr) != nullptr;
    }
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent (QGraphicsSceneHoverEvent * event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    //画背景网格
    void drawGrid(QPainter *painter);
    //绘制地表和边界
    void drawGround(QPainter * painter);
    bool showGrid(){return _showGrid;}
    void setShowGrid(bool v){ _showGrid=v;};
    //坐标系，x向：距离，单位 米，y向；高程，单位：米
    QRectF& ticks(){return _ticks;}
    QRectF ticks()const {return _ticks;};
    //网格间距
    QSize gridSize(){return _gridSize;};
    virtual QString dataAtPos(QPointF pos)=0;
private:
    //    QRectF  _boundingRect;
    bool _showGrid{true};
    QSize _gridSize{20,20};
protected:
    QRectF _ticks;//xy轴的刻度范围
public:
signals:
    void hoverData(QPointF pos,QString des);
};
#endif // QGEOSECTIONCONTENT_H
