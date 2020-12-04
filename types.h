#ifndef TYPES_H
#define TYPES_H
// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.

#include <QMetaType>                            //自定义参数类型的头文件----***
#include <QList>
#include <QObject>
#include <QMap>
#include <QtDebug>
#include <QPointF>
#define _USE_MATH_DEFINES
#include <math.h>
#include <QPainterPath>
#define  EARTH_RADIUS 6378.137

#define DARK_RED qRgb(155, 0, 2)
#define MUCH_LIGHT_RED  qRgb(252, 170, 170)
#define LIGHT_RED  qRgb(218, 100, 100)
#define LIGHT_GRAY qRgb(220, 220, 220)
#define TRANSPARENT_DARK_GREEN QColor(9,92,12,100)
#define GEO_TITLE_ITEM_HEIGHT 16
#define GEO_SECTION_MIN_WIDTH 10
#define X_SCALE 1
#define Y_SCALE 1
#define GROUND_THICKNESS 0.10 //m
#define TICK_WIDTH 5
#define DEFAULT_LEGEND "default"
#define BRUSH_X_SCALE 5
#define BRUSH_Y_SCALE 5


enum LayerMatch{
    One2One,
    One2Many,
    Many2One,
    NoMatch,
};
enum ConnectPoint{
    Top,
    Middle,
    Bottom,
};
extern double rad(double d) ;

extern double getDistance(double lat1, double lng1, double lat2, double lng2);
class QGeoFormation;
class QSampleLinks;
class QSection;
//地层采样，
//如果seal=1，表明是一个虚拟的封顶、封底层，厚度=0
class QGeoSample : public QObject{
    Q_OBJECT
public:
    explicit QGeoSample(float top,float bot=0,QString des="",int seal=0,QObject *parent=nullptr);
    explicit QGeoSample(QGeoSample & another);
    explicit QGeoSample(QGeoSample * another);
    ~QGeoSample(){};


    float Thickness();

    bool vanish() const;
    void setVanish(bool value);

    QString desc() const;
    void setDesc(const QString &desc);

    float top() const;
    void setTop(float top);

    float bottom() const;
    void setBottom(float bottom);

    QList<QGeoSample *> paired() const;
    void setPaired(const QList<QGeoSample *> &paired);

    QList<QGeoSample *> bottomLinked() const;
    void setBottomLinked(const QList<QGeoSample *> &bottomLinked);

    int index() const;
    void setIndex(int index);

    QGeoFormation* formation() const;
    void setFormation( QGeoFormation *formation);
    void paint(QPainter *painter,QTransform &transform,int align, float width,bool last=false);
    bool fake() const;
    void setFake(bool fake);

private:
    //是否尖灭
    bool _vanish=false;
    int _index=0;
    float _top=0;
    float _bottom=0;                                //底
    int  _seal=0;
    QString _desc="";
    QList<QGeoSample*>_paired;//同质的地层
    QList<QGeoSample*>_bottomLinked;//底层连接到的层
    QGeoFormation * _formation=nullptr;
};
Q_DECLARE_METATYPE(QGeoSample*);
//井眼间地层匹配结构，intersactions是交线
class QSampleMatcher:public QObject{
    Q_OBJECT
public:
    explicit QSampleMatcher(int ln=0,int rn=0,QSection *parent=nullptr);
    int leftSampleNo;
    int rightSampleNo;
    QMap<int ,QSampleMatcher*> intersactions;
    QString toStr();
    QSection * section;
    void PrintIntersactions();
    float slop() const;
};
Q_DECLARE_METATYPE(QSampleMatcher*);

typedef bool  (*connector_comparor)( const QSampleMatcher * arg1, const QSampleMatcher * arg2);
//井眼间地层关联结构
class QSampleLink:public QObject{
    Q_OBJECT
public:
    explicit QSampleLink(int ln=0,ConnectPoint lc=Top,int rn=0,ConnectPoint rc=Top,QObject *parent=nullptr);
    int leftSampleNo;
    int rightSampleNo;
    float leftDepth;
    float rightDepth;
    ConnectPoint leftConnectPoint;
    ConnectPoint rightConnectPoint;
    QString toStr();

    ConnectPoint getLeftConnectPoint() const;
    void setLeftConnectPoint(const ConnectPoint &value);
    ConnectPoint getRightConnectPoint() const;
    void setRightConnectPoint(const ConnectPoint &value);
};
Q_DECLARE_METATYPE(QSampleLink*);
//井眼结构，一个井眼包含多个地层采样samples
class  QWellbore:public QObject{
    Q_OBJECT
public:
    explicit QWellbore(QString n="",QObject *parent=nullptr);
    explicit QWellbore(QWellbore &);
    QGeoSample * AddSample(float top,float bot,QString desc,int seal=0);
    QGeoSample *  AddSample(QGeoSample * layer);

    QList<QGeoSample*>  samples;
    QString name() const;
    void setName(const QString &name);
    void print();
    void seal();
    float top();
    float bottom();
private:

    QString _name;
};

Q_DECLARE_METATYPE(QWellbore*);
//相邻双井间的地质纵剖面结构，
//井眼A 剖面 井眼B
//  |        |
//  | ——————-|
//  |   地 层 |
//  | ———————|
//  |        |
//  |        |
//  |        |
//  |        |
class  QSection:public QObject{
    Q_OBJECT
public:
    explicit QSection(QWellbore *left=nullptr ,QWellbore * right=nullptr,float distance=0,QObject *parent=nullptr);
    //    ~QSection();
    void  process(); //层间关系处理
    QString name();
    float top();//取双井最浅处
    float bottom();//取双井最深处
    float width();//实际上是井间距，
    QWellbore *left() const; //左侧井眼
    void setLeft(QWellbore *left);

    QWellbore *right() const;//右侧井眼
    void setRight(QWellbore *right);

    float distance() const;
    void setDistance(float distance);

    QList<QGeoFormation *> formations() const; //地层信息
    void setFormations(const QList<QGeoFormation *> &formations);
    void ProcessMissingFormations(QWellbore * left ,QWellbore * right,int leftNo ,int leftSize,int rightNo,int rightSize,bool rev);
    void processConnections(QMap<QString,QSampleMatcher*> &  samplePairs,connector_comparor);
    void ProcessSamples(QWellbore* left ,QWellbore *right, QMap<int ,QList<int>> & leftLinkedsampleMap,QMap<int ,QList<int>> & rightLinkedsampleMap,bool rev=false);
    void AddFormation(QGeoFormation* formation);
    void drawGround(QPainter * painter); //绘制剖面边界
    void drawOneGround(QPainter *painter, QTransform& transform2,QPolygonF & poly,QLineF & line);
    void drawGround(QPainter * painter,QTransform& transform);
    float depth(); //深度间距
private:
    QWellbore *_left;
    QWellbore *_right;
    float _distance;
    QList<QSampleLink *>  sampleLinks;
    QList<QGeoFormation*> _formations;
};
Q_DECLARE_METATYPE(QSection*);
class QSampleLinks:public QObject{
    Q_OBJECT
public :
    explicit QSampleLinks(QObject *parent=nullptr );
    void AddLink(QSampleLink * qlink);
    void Normalize();
private:
    QMap<QString,QSampleLink*> _links;
    QMap<int,QList<QSampleLink*>> _linksByLeft;
    QMap<int,QList<QSampleLink*>> _linksByRight;
};
Q_DECLARE_METATYPE(QSampleLinks*);
//地层，主要数据是其边界路径contour，是绘制时的依据，通常是由上下左右四边组成
class QGeoFormation:public QObject{
    Q_OBJECT
public:
    explicit QGeoFormation(QPainterPath contour,QString desc,QObject * parent=nullptr);
    explicit QGeoFormation(QVector<QPointF> contour,QString desc ,QObject * parent=nullptr);
    QPainterPath contour;
    QString desc() const;
    void merge(QVector<QPointF> contour);
    void sub(QVector<QPointF> contour);
    void setDesc(const QString &desc);
    void paint(QPainter * painter, QTransform& transform2);
private:
    QString _desc;
};

#endif // TYPES_H
