#ifndef TYPES_H
#define TYPES_H

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

class QSampleMatcher:public QObject{
    Q_OBJECT
public:
    explicit QSampleMatcher(int ln=0,int rn=0,QObject *parent=nullptr);
    int leftSampleNo;
    int rightSampleNo;
    QMap<int ,QSampleMatcher*> intersactions;
    QString toStr();

    void PrintIntersactions();
};
Q_DECLARE_METATYPE(QSampleMatcher*);

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
class  QSection:public QObject{
    Q_OBJECT
public:
    explicit QSection(QWellbore *left=nullptr ,QWellbore * right=nullptr,float distance=0,QObject *parent=nullptr);
    //    ~QSection();
    void  process();
    QString name();
    float top();
    float bottom();
    float width();
    QWellbore *left() const;
    void setLeft(QWellbore *left);

    QWellbore *right() const;
    void setRight(QWellbore *right);

    float distance() const;
    void setDistance(float distance);

    QList<QGeoFormation *> formations() const;
    void setFormations(const QList<QGeoFormation *> &formations);
    void ProcessSamples(QWellbore* left ,QWellbore *right, QMap<int ,QList<int>> & leftLinkedsampleMap,QMap<int ,QList<int>> & rightLinkedsampleMap,bool rev=false);
    void AddFormation(QGeoFormation* formation);
    void drawGround(QPainter * painter);
    void drawOneGround(QPainter *painter, QTransform& transform2,QPolygonF & poly,QLineF & line);
    void drawGround(QPainter * painter,QTransform& transform);
    float depth();
private:
    QWellbore *_left;
    QWellbore *_right;
    float _distance;
    //    QList<QLayerConnect*> connects;
    QMap<QString,QSampleMatcher*>samplePairs;
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
class QGeoFormation:public QObject{
    Q_OBJECT
public:
    explicit QGeoFormation(QPainterPath contour,QString desc,QObject * parent=nullptr);
    explicit QGeoFormation(QVector<QPointF> contour,QString desc ,QObject * parent=nullptr);
    QPainterPath contour;
//    QVector<QPointF> verts;

    QString desc() const;
    void merge(QVector<QPointF> contour);
    void sub(QVector<QPointF> contour);
    void setDesc(const QString &desc);
    void paint(QPainter * painter, QTransform& transform2);
private:
    QString _desc;
};

#endif // TYPES_H
