#ifndef QGEOSECTIONCONTENT_H
#define QGEOSECTIONCONTENT_H

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
    void drawGrid(QPainter *painter);
    void drawGround(QPainter * painter);
    bool showGrid(){return _showGrid;}
    void setShowGrid(bool v){ _showGrid=v;};
    QRectF& ticks(){return _ticks;}
    QRectF ticks()const {return _ticks;};
    QSize gridSize(){return _gridSize;};
    virtual QString dataAtPos(QPointF pos)=0;
private:
    //    QRectF  _boundingRect;
    bool _showGrid{true};
    QSize _gridSize{20,20};
    QRectF _ticks;//xy轴的刻度范围
public:
signals:
    void hoverData(QPointF pos,QString des);
};
#endif // QGEOSECTIONCONTENT_H
