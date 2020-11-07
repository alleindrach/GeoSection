#ifndef QGEOSECTIONTITLE_H
#define QGEOSECTIONTITLE_H

#include <QGraphicsWidget>
#include <types.h>
class QGeoSectionTitle : public QGraphicsWidget
{
    Q_OBJECT
public:
    QGeoSectionTitle(QSection *section,  QGraphicsItem * parent);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const override;
    int contentheight() const{
        return GEO_TITLE_ITEM_HEIGHT;
    }
    void setHeight(int v){ _height=v;}
    QString title(){
        return _section->name();
    }

    bool last() const;
    void setLast(bool last);

private :
    QSection *_section;
    int _height{0};
    bool _last=false;
};
#endif // QGEOSECTIONTITLE_H
