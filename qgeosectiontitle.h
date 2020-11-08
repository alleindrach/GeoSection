#ifndef QGEOSECTIONTITLE_H
#define QGEOSECTIONTITLE_H

#include <QGraphicsWidget>
#include <types.h>
class QGeoSectionTitle : public QGraphicsWidget
{
    Q_OBJECT
public:
    QGeoSectionTitle(QString title,  QGraphicsItem * parent);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const override;
    int contentheight() const{
        return GEO_TITLE_ITEM_HEIGHT;
    }
    void setHeight(int v){ _height=v;}
    QString title(){
        return _title;
    }
private :
    QString _idWell;
    QString _title;

    int _height{0};
};
#endif // QGEOSECTIONTITLE_H
