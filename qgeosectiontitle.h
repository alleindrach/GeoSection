#ifndef QGEOSECTIONTITLE_H
#define QGEOSECTIONTITLE_H
// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.
#include <QGraphicsWidget>
#include <types.h>
#include <qgeotitle.h>
//相邻井眼间剖面图标题栏
class QGeoSectionTitle : public QGeoTitle
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
