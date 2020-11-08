#ifndef QGEOTITLE_H
#define QGEOTITLE_H
#include <QGraphicsWidget>
#include <types.h>

class QGeoTitle :public QGraphicsWidget
{
    Q_OBJECT
public:
    QGeoTitle(  QGraphicsItem * parent);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override=0;
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const override=0;
    virtual int contentheight() const=0;
    virtual QString title()=0;
    virtual void setHeight(int height)=0;
private :
    int _height{0};
};

#endif // QGEOTITLE_H
