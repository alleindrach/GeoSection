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
    void AddSection(QGeoSectionWidget *section, int pos=0,int stretchFactor=0);
    QGraphicsWidget *  topWidget(){
        return _form;
    }
private:

    QGraphicsProxyWidget * _wellTitle;
    QGraphicsWidget  * _form;
    QGraphicsLinearLayout * _sectionLayout;
    QVector<QGeoSectionWidget *> _sections;
    int _titleHeight{0};
};

#endif // QGEOSECTIONSCENE_H
