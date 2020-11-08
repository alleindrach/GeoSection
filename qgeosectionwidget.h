#ifndef QGEOSECTIONWIDGET_H
#define QGEOSECTIONWIDGET_H
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsWidget>
#include <types.h>
#include <qgeosectiontitle.h>
#include <qgeosectioncontent.h>
#include <qgeotitle.h>
class QGeoSectionWidget:public QGraphicsWidget
{
    Q_OBJECT
public:
    QGeoSectionWidget(QGeoTitle * title,QGeoSectionContent * content,QGraphicsItem * parent=nullptr);
    int titleContentHight(){
        return _title->contentheight();
    }
    void setTitleHeight(int v) {
        _title->setHeight(v);
    }
signals:
    void hoverData(QPointF pos,QString comp,QString des);
public slots:
    void on_hover_data(QPointF pos,QString des);
private:
    QGeoTitle* _title{nullptr};
    QGeoSectionContent * _content{nullptr};

};

#endif // QGEOSECTIONWIDGET_H
