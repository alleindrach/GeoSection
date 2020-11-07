#ifndef QGEOSECTIONSCENE_H
#define QGEOSECTIONSCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QWidget>
#include <types.h>
#include <QGraphicsLinearLayout>
#include <QVector>
#include "qgeosectionwidget.h"
class QGeoSectionScene : public QGraphicsScene
{
    Q_OBJECT
public:
    QGeoSectionScene(QObject *parent  = nullptr);
    ~QGeoSectionScene(){
//        QList<QString >keys=_legends.keys();
//        for(int i=0;i<keys.size();i++){
//            delete _legends[keys[i]];
//        }
    };
    void AddSection(QGeoSectionWidget *section, int pos=-1,int stretchFactor=0);
    QGraphicsWidget *  topWidget(){
        return _form;
    }
   static  void addLegend(QString ,Qt::BrushStyle,QColor);
   static QBrush&  getLegend(QString);
   static QMap<QString,QBrush> _legends;
private:

    QGraphicsProxyWidget * _wellTitle;
    QGraphicsWidget  * _form;
    QGraphicsLinearLayout * _sectionLayout;
    QVector<QGeoSectionWidget *> _sections;
    int _titleHeight{0};

};

#endif // QGEOSECTIONSCENE_H
