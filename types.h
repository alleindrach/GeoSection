#ifndef TYPES_H
#define TYPES_H

#include <QMetaType>                            //自定义参数类型的头文件----***
#include <QList>
#include <QObject>
#include <QMap>
#include <QtDebug>
#define _USE_MATH_DEFINES
#include <math.h>
#define  EARTH_RADIUS 6378.137

#define DARK_RED qRgb(155, 0, 2)
#define MUCH_LIGHT_RED  qRgb(252, 170, 170)
#define LIGHT_RED  qRgb(218, 100, 100)
#define LIGHT_GRAY qRgb(220, 220, 220)
#define GEO_TITLE_ITEM_HEIGHT 16
#define GEO_SECTION_MIN_WIDTH 10

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

class QGeoLayer : public QObject{
    Q_OBJECT
public:
    explicit QGeoLayer(float top,float bot=0,QString des="",QObject *parent=nullptr);
    ~QGeoLayer(){};
    float top=0;
    float bottom=0;                                //底
    QString desc="";                      //简单描述
    int index=0;
    void SetIndex(int index);
    int Index();
    float Thickness();
    bool vanish=false;
    bool getVanish() const;
    void setVanish(bool value);
};
Q_DECLARE_METATYPE(QGeoLayer*);

class QLayerMatcher:public QObject{
    Q_OBJECT
public:
    explicit QLayerMatcher(int ln=0,int rn=0,QObject *parent=nullptr);
    int leftLayerNo;
    int rightLayerNo;
    QMap<int ,QLayerMatcher*> intersactions;
    QString toStr();

    void PrintIntersactions();
};
Q_DECLARE_METATYPE(QLayerMatcher*);

class QLayerLink:public QObject{
    Q_OBJECT
public:
    explicit QLayerLink(int ln=0,ConnectPoint lc=Top,int rn=0,ConnectPoint rc=Top,QObject *parent=nullptr);
    int leftLayerNo;
    int rightlayerNo;
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
Q_DECLARE_METATYPE(QLayerLink*);
class  QWellbore:public QObject{
    Q_OBJECT
public:
    explicit QWellbore(QString n="",QObject *parent=nullptr);

    QGeoLayer * AddLayer(float top,float bot,QString desc);
    QList<QGeoLayer*>  layers;
    QString name;
    void Print();
    void Seal();
    float top();
    float bottom();
};

Q_DECLARE_METATYPE(QWellbore*);
class  QSection:public QObject{
    Q_OBJECT
public:
    explicit QSection(QWellbore *left=nullptr ,QWellbore * right=nullptr,float distance=0,QObject *parent=nullptr);
    //    ~QSection();
    QWellbore *left;
    QWellbore *right;
    float distance;
    //    QList<QLayerConnect*> connects;
    QMap<QString,QLayerMatcher*>layerPairs;
    QList<QLayerLink *>  layerLinks;
    void  Process();
    QString name();
    float Top();
    float Bottom();
    float Width();
};
Q_DECLARE_METATYPE(QSection*);

#endif // TYPES_H
