#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "types.h"
#include "qgeosectionscene.h"
#include "qgeolayerswidget.h"
#include <QVector>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    _statusComponenet = new QLabel(this);
    QFont font=_statusComponenet->font();
    font.setPixelSize(10);
    _statusComponenet->setFont(font);
    _statusComponenet->setMinimumWidth(160);
    _statusData= new QLabel(this);

    _statusData->setFont(font);
    _statusData->setMinimumWidth(160);

    _statusDepth=new QLabel(this);
    _statusDepth->setFont(font);
    _statusDepth->setMinimumWidth(100);

    ui->statusbar->addWidget(_statusComponenet,0);
    ui->statusbar->addWidget(_statusDepth,0);
    ui->statusbar->addWidget(_statusData,0);


    QWellbore * W63=new QWellbore("GWzk63",this);

    W63->AddSample(0.53,3.33,"粉质黏土2");
    W63->AddSample(3.33,4.53,"淤泥");
    W63->AddSample(4.53,5.53,"耕土");

    QWellbore *W56=new QWellbore("GWzk56",this);

    W56->AddSample(-1.68,-0.98,"粉质黏土2");
    W56->AddSample(-0.98,1,"淤泥");
    W56->AddSample(1,3.32,"沙泥");
    W56->AddSample(3.32,4.32,"淤泥");
    W56->AddSample(4.32,6.03,"耕土");

    QWellbore * W49=new QWellbore("GWzk49",this);
    W49->AddSample(3.03,6.03,"耕土");
    W49->AddSample(0.33,3.03,"粉质黏土2");

    QWellbore * W42=new QWellbore("GWzk42",this);
    W42->AddSample(3.17,5.57,"淤泥");
    W42->AddSample(0.57,3.17,"粉质黏土2");

    QWellbore * W35=new QWellbore("GWzk35",this);
    W35->AddSample(4.65,5.05,"耕土");
    W35->AddSample(3.25,4.65,"粉质黏土1");
    W35->AddSample(1.05,3.25,"粉质黏土2");

    QWellbore  *W28 =new QWellbore("GWzk28",this);
    W28->AddSample(4.82,5.22,"耕土");
    W28->AddSample(3.42,4.82,"粉质黏土1");
    W28->AddSample(0.22,3.42,"粉质黏土2");

    QWellbore *W21=new QWellbore("GWzk21",this);
    W21->AddSample(4.10,5.10,"耕土");
    W21->AddSample(2.10,4.10,"粉质黏土2");

    QList<QWellbore *> wells={W35,W42,W49,W56,W63,W21,W28};
    QVector<QSection*>sections;
//    QVector<QGeoSectionWidget *> sectionWigets;
    float top=9999999999,bottom=-999999999;
    float width=0;
    int s=wells.length();
    for(int i=0;i<1;i++){
        float w=10;
        //    for(int i=0;i<wells.length()-1;i++){
        QSection *sec=new QSection(wells[i],wells[i+1],w,this);
        width+=w;
        //        QGeoSectionWidget *widget=new QGeoSectionWidget(sec,ui->graphicsView);
        if(sec->top()<top){
            top=sec->top();
        }
        if(sec->bottom()>bottom){
            bottom=sec->bottom();
        }
        sec->process();
        sections.append(sec);
    }
    QGeoSectionScene * scene=new QGeoSectionScene(ui->graphicsView);
    scene->setBackgroundBrush(QBrush(Qt::gray));
    ui->graphicsView->setContentsMargins(0,0,0,0);
    ui->graphicsView->setScene(scene);
    QGeoSectionScene::addLegend("粉质黏土2",Qt::DiagCrossPattern,QColor(100,100,100));
    QGeoSectionScene::addLegend("淤泥",Qt::Dense5Pattern,QColor(100,100,100));
    QGeoSectionScene::addLegend("耕土",Qt::Dense6Pattern,QColor(100,100,100));
    QGeoSectionScene::addLegend("粉质黏土1",Qt::CrossPattern,QColor(100,100,100));
    QGeoSectionScene::addLegend("沙泥",Qt::Dense7Pattern,QColor(100,100,100));
    QGeoSectionScene::addLegend(DEFAULT_LEGEND ,Qt::Dense6Pattern,QColor(10,10,10));

    for(int i=0;i<sections.length();i++){
        QSection *sec=sections[i];
        QGeoLayersWidget * content=new QGeoLayersWidget(sec,QRectF(QPointF(0,top),QPointF(sec->distance(),bottom)));

        QGeoSectionTitle * title=new QGeoSectionTitle(sec->name(),((QGeoSectionScene*)(ui->graphicsView->scene()))->topWidget());
        QGeoSectionWidget * track=new QGeoSectionWidget(title,content,((QGeoSectionScene*)(ui->graphicsView->scene()))->topWidget());
        int sf=sec->width()*100/width;
//        connect(track,&QGeoSectionWidget::hoverData,this,&MainWindow::on_hover_data);
//        sectionWigets.append(track);
        scene->AddSection(track,-1,sf);
    }
    this->resize(this->size()+QSize(1,1));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionOpen_triggered()
{


}
//void MainWindow::on_hover_data(QPointF pos,QString comp, QString des)
//{
//    _statusComponenet->setText(comp);
//    _statusData->setText(des);
//    _statusDepth->setText(QString().asprintf(("MD:%6.2f"),pos.y()));
//}
