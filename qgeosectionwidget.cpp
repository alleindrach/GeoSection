#include "qgeosectionwidget.h"

#include <QPainter>
#include <QPair>
#include <QTextItem>
#include <QGraphicsScene>
#include <QLabel>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QGraphicsSceneHoverEvent>
#include "types.h"
#include <QGraphicsItem>
//#include <GraphicsItemFlag>
#include <QGraphicsLayoutItem>

QGeoSectionWidget::QGeoSectionWidget(QGeoSectionTitle *title, QGeoSectionContent *content, QGraphicsItem *parent):
    QGraphicsWidget(parent),_title(title),_content(content)
{

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);

    QGraphicsLinearLayout *topLayout = new QGraphicsLinearLayout;
    topLayout->setOrientation(Qt::Vertical);
    topLayout->setContentsMargins(0,0,0,0);
    topLayout->setItemSpacing(0,0);
    topLayout->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    topLayout->addItem(title);

    topLayout->addItem(content);
    content->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    this->setLayout(topLayout);
    title->setParentItem(this);

    connect(content,&QGeoSectionContent::hoverData,this,&QGeoSectionWidget::on_hover_data);
}

void QGeoSectionWidget::on_hover_data(QPointF pos,  QString des)
{
    emit this->hoverData(pos,_title->title(),des);
}





