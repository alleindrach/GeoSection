#include "qgeosectionscene.h"
#include <QLabel>
#include <QGraphicsScene>
QMap<QString,QBrush> QGeoSectionScene::_legends=QMap<QString,QBrush>();
QGeoSectionScene::QGeoSectionScene(QObject *parent ):QGraphicsScene(parent)
{


    QLabel * wellTitleLabel=new QLabel();

    wellTitleLabel->setMinimumHeight(24);
    wellTitleLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    wellTitleLabel->setStyleSheet("background-color:white;color:black; ");

    wellTitleLabel->setText("WellSections");
    wellTitleLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    _wellTitle = this->addWidget(wellTitleLabel);

//    _wellTitle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    QGraphicsLinearLayout *topLayout = new QGraphicsLinearLayout;
    topLayout->setOrientation(Qt::Vertical);
    topLayout->setContentsMargins(0,0,0,0);
    topLayout->setItemSpacing(0,0);

    topLayout->addItem(_wellTitle);

    QGraphicsWidget * sectionsBoxWidget = new QGraphicsWidget;
    _sectionLayout = new QGraphicsLinearLayout;
    _sectionLayout->setOrientation(Qt::Horizontal);
    _sectionLayout->setContentsMargins(0,0,0,0);
    _sectionLayout->setItemSpacing(0,0);

    sectionsBoxWidget->setLayout(_sectionLayout);
//    sectionsBoxWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    topLayout->addItem(sectionsBoxWidget);

    _form = new QGraphicsWidget;
    _form->setContentsMargins(8,8,0,0);
    _form->setLayout(topLayout);

    _form->setPos(0,0);
//    _form->setGeometry(QRectF(0,0,100,1000));
//    _form->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    this->addItem(_form);
}


void QGeoSectionScene::AddSection(QGeoSectionWidget *section, int pos, int sf)
{
    if(pos<0||pos>=_sections.size()){
        _sections.append(section);
    }else{
        _sections.insert(pos,section);
    }
    _sectionLayout->addItem(section);
    _sectionLayout->addStretch(sf);
    for(int i=0;i<_sections.size();i++)
    {
        _sectionLayout->setItemSpacing(i,0);
    }
    if(section->titleContentHight()>this->_titleHeight){
        for(int i=0;i<_sections.size();i++)
        {
            QGeoSectionWidget * t=_sections[i];
            t->setTitleHeight(section->titleContentHight());
        }
        _titleHeight=section->titleContentHight();
    }else{
        section->setTitleHeight(this->_titleHeight);
    }
    //    section->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
}

void QGeoSectionScene::addLegend(QString desc, Qt::BrushStyle style, QColor color)
{
    QBrush  brush=QBrush(color,style);
//    QTransform  transform;
//    transform.scale(10,10);
//    brush.setTransform(transform);
    QGeoSectionScene::_legends[desc]=brush;
}

QBrush& QGeoSectionScene::getLegend(QString desc)
{
     if(QGeoSectionScene::_legends.contains(desc)){
         return QGeoSectionScene::_legends[desc];
     }else{
         return QGeoSectionScene::_legends.first();
     }
}
