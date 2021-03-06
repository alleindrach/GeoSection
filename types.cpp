#include "types.h"
#include <QObject>
#include <QtDebug>
#include  <QList>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>
#include <QPolygon>
#include <qgeosectionscene.h>
// Copyright 2020 Alleindrach@gmail.com 唐恒. All rights reserved.

//排序比较函数，比较两个地层匹配连线谁的交点更多
bool  more_intersactions(const QSampleMatcher * s1,const QSampleMatcher * s2)
{
    if(s1->intersactions.size()==s2->intersactions.size()){
        if(qAbs( s1->leftSampleNo-s1->rightSampleNo)>qAbs(s2->leftSampleNo-s2->rightSampleNo)){
            return true;
        }else{
            return false;
        }
    }
    return (s1->intersactions.size() > s2->intersactions.size());
}
//排序比较器，比较两个地层匹配连线谁的交点更多，如果相同，再比较其斜率，斜率不是绝对斜率，而是按照层位计算的斜率，比如跨两个地层的连线就比跨一个地层的连线斜率大
bool  more_intersactions_or_slop(const QSampleMatcher * s1,const QSampleMatcher * s2)
{
    if(s1->intersactions.size()==s2->intersactions.size()){
        if(qAbs( s1->leftSampleNo-s1->rightSampleNo)>qAbs(s2->leftSampleNo-s2->rightSampleNo)){
            return true;
        }else if(qAbs( s1->leftSampleNo-s1->rightSampleNo)<qAbs(s2->leftSampleNo-s2->rightSampleNo)){
            return false;
        }else{
            return s1->slop()>s2->slop();
        }
    }
    return (s1->intersactions.size() > s2->intersactions.size());
}
//比较两个地层匹配连线谁更浅，按照左侧井眼层位比较
bool shallow(const QSampleMatcher * s1,const QSampleMatcher *s2){
    return (s1->leftSampleNo<s2->leftSampleNo);
}
//比较两个地层匹配连线谁更浅，按照右侧井眼层位比较
bool shallowRight(const QSampleMatcher * s1,const QSampleMatcher *s2){
    return (s1->rightSampleNo<s2->rightSampleNo);
}
//比较两个地层深浅，按照地层的底部深度比较
bool shallowsample(const QGeoSample *  s1,const QGeoSample *s2){
    return (s1->bottom() <s2->bottom());
}
//地层连线比较
//bool linkerCompare(const QSampleLink *  s1,const QSampleLink *s2){
//    if (s2->leftSampleNo>s1->leftSampleNo ) return true;
//    if (s2->leftSampleNo<s1->leftSampleNo) return false;
//    if (s2->leftConnectPoint>s1->leftConnectPoint) return true;
//    if (s2->leftConnectPoint<s1->leftConnectPoint)return false;
//    if (s2->rightSampleNo>s1->rightSampleNo) return true;
//    if(s2->rightSampleNo<s1->rightSampleNo)return false;
//    if(s2->rightConnectPoint>s1->rightConnectPoint)return true;
//    if(s2->rightConnectPoint<s1->rightConnectPoint)return false;
//    return false;
//}
//bool linkerCompareRev(const QSampleLink *  s1,const QSampleLink *s2){
//    if (s2->rightSampleNo>s1->rightSampleNo) return true;
//    if(s2->rightSampleNo<s1->rightSampleNo)return false;
//    if(s2->rightConnectPoint>s1->rightConnectPoint)return true;
//    if(s2->rightConnectPoint<s1->rightConnectPoint)return false;
//    if (s2->leftSampleNo>s1->leftSampleNo ) return true;
//    if (s2->leftSampleNo<s1->leftSampleNo) return false;
//    if (s2->leftConnectPoint>s1->leftConnectPoint) return true;
//    if (s2->leftConnectPoint<s1->leftConnectPoint)return false;
//    return false;
//}

//角度转弧度
double rad(double d) {
    return d * M_PI / 180.0;
}
//根据经纬度，计算地表两点间直线距离
double getDistance(double lat1, double lng1, double lat2, double lng2) {
    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    double a = radLat1 - radLat2;
    double b = rad(lng1) - rad(lng2);
    double s = 2 * asin(sqrt(pow(sin(a / 2), 2)
                             + cos(radLat1) * cos(radLat2)
                             * pow(sin(b / 2), 2)));
    s = s * EARTH_RADIUS;
    s = round(s * 10000.0) / 10000.0;
    s = s*1000;
    return s;
}
//地层采样初始化
QGeoSample::QGeoSample(float top,float bot,QString des,int seal,QObject *parent):QObject(parent),_top(top),_bottom(bot),_seal(seal),_desc(des){

}
QGeoSample::QGeoSample(QGeoSample &another)
{
    this->_top=another.top();
    this->_bottom=another.bottom();
    this->_desc=another.desc();
    this->_vanish=another.vanish();
    this->_index=another.index();
}

QGeoSample::QGeoSample(QGeoSample *another)
{
    this->_top=another->top();
    this->_bottom=another->bottom();
    this->_desc=another->desc();
    this->_vanish=another->vanish();
    this->_index=another->index();
}

void QGeoSample::setIndex(int index){
    _index=index;
}


int QGeoSample::index()const{
    return _index;
}
float QGeoSample::Thickness(){
    return _bottom-_top;
}

bool QGeoSample::vanish() const
{
    return _vanish;
}

void QGeoSample::setVanish(bool value)
{
    _vanish = value;
}

QString QGeoSample::desc() const
{
    return _desc;
}

void QGeoSample::setDesc(const QString &desc)
{
    _desc = desc;
}

float QGeoSample::top() const
{
    return _top;
}

void QGeoSample::setTop(float top)
{
    _top = top;
}

float QGeoSample::bottom() const
{
    return _bottom;
}

void QGeoSample::setBottom(float bottom)
{
    _bottom = bottom;
}

QList<QGeoSample *> QGeoSample::paired() const
{
    return _paired;
}

void QGeoSample::setPaired(const QList<QGeoSample *> &paired)
{
    _paired = paired;
}

QList<QGeoSample *> QGeoSample::bottomLinked() const
{
    return _bottomLinked;
}

void QGeoSample::setBottomLinked(const QList<QGeoSample *> &bottomLinked)
{
    _bottomLinked = bottomLinked;
}

//地层匹配项初始化
QSampleMatcher::QSampleMatcher(int ln,int rn,QSection *parent):QObject(parent),leftSampleNo(ln),rightSampleNo(rn),section(parent){

}

QString QSampleMatcher::toStr()
{

    //    qDebug()<<"l="<<this->leftSampleNo;
    QString result= QString("%1-%2").arg(this->leftSampleNo).arg(this->rightSampleNo);
    //    qDebug()<<"<<<<"<<result;
    return result;

}

void QSampleMatcher::PrintIntersactions()
{
    QList<int> keys=intersactions.keys();
    qDebug()<<"|||||"<<this->toStr()<<"交点集:";
    for(int i=0;i<keys.size();i++){
        qDebug()<<"\t"<<i<<"|"<<intersactions[keys[i]]->toStr();
    }
}
//地层匹配项的斜率，按照底部连线的深度差计算，这里假设井都是竖直方向平行的，所以斜率只和深度差相关
float QSampleMatcher::slop() const
{
    float leftDepth=this->section->left()->samples[this->leftSampleNo]->bottom();
    float rightDepth=this->section->right()->samples[this->rightSampleNo]->bottom();
    return qAbs(leftDepth-rightDepth);
}
QWellbore::QWellbore(QString n,QObject *parent):QObject(parent),_name(n){

}

QWellbore::QWellbore(QWellbore & another)
{
    foreach(auto sample ,another.samples){
        this->AddSample(new QGeoSample(sample));
    }
    this->_name=another.name();
}


QGeoSample *QWellbore::
AddSample(float top,float bot, QString desc,int seal)
{
    QGeoSample * newsample=new QGeoSample(top*Y_SCALE, bot*Y_SCALE,desc,seal,this) ;
    return AddSample(newsample);
    //    return nullptr;
}
//向井眼QWellbore增加地层采样，每次都要按照深度排序
QGeoSample * QWellbore::AddSample(QGeoSample *sample)
{
    this->samples.append(sample);
    std::sort(samples.begin(),samples.end(),shallowsample);

    for(int i=0;i<this->samples.size();i++){
        this->samples[i]->setIndex(i);
    }
    return sample;
}

QString QWellbore::name() const
{
    return _name;
}

void QWellbore::setName(const QString &name)
{
    _name = name;
}
void QWellbore::print(){
    for(int i=0;i<this->samples.size();i++){
        qDebug()<<_name<<""<<i<<",["<<this->samples[i]->top()<<"="<<this->samples[i]->bottom()<<","<<this->samples[i]->desc()<<"]\n";
    }
}

void QWellbore::seal()
{
    // 步骤 1:对所有钻孔进行数据预处理，在各钻孔顶端和底端添加 0 厚度的虚拟连续地层。
    this->AddSample(this->top()/Y_SCALE,this->top()/Y_SCALE,"顶部虚拟层",1);
    this->AddSample(this->bottom()/Y_SCALE,this->bottom()/Y_SCALE,"底部虚拟层",2);
}

float QWellbore::top()
{
    if(samples.size()>0)
        return samples[0]->top();
    else return 0;
}

float QWellbore::bottom()
{
    if(samples.size()>0){
        return samples[samples.size()-1]->bottom();
    }else
        return 0;
}

QSection::QSection(QWellbore *left ,QWellbore * right,float distance,QObject *parent):QObject(parent),_distance(distance*X_SCALE){
    this->_left=new QWellbore(*left);
    this->_right=new QWellbore(*right);
}
// no 应为上一层
void QSection::ProcessMissingFormations(QWellbore * left ,QWellbore * right,int leftNo ,int leftSize,int rightNo,int rightSize,bool rev){
    if(rightSize==0 && leftSize>0){
        for(int i=1;i<=leftSize;i++){
            if(left->samples[leftNo+i]->formation()==nullptr){
                QVector<QPointF> verts;
                verts.append(QPointF(rev?this->_distance:0,left->samples[leftNo+i]->top()));
                verts.append(QPointF(rev?0:this->_distance,right->samples[rightNo]->bottom()));
                verts.append(QPointF(rev?this->_distance:0,left->samples[leftNo+i]->bottom()));
                QGeoFormation * formation=new QGeoFormation(verts,left->samples[leftNo+i]->desc(),this);
                this->AddFormation(formation);
                left->samples[leftNo+i]->setFormation(formation);
            }
        }
    }else if (leftSize>0 && rightSize>0 && !rev){
        QMap<QString,QSampleMatcher*>samplePairs;
        for(int i=0;i<=leftSize;i++){
            for (int j=0;j<=rightSize;j++){
                QSampleMatcher* con=new QSampleMatcher(i+leftNo,j+rightNo,this);

                samplePairs[QString("%1-%2").arg(con->leftSampleNo).arg(con->rightSampleNo)]=con;
            }
        }
        this->processConnections(samplePairs,more_intersactions_or_slop);
        QList<QSampleMatcher*> connects=samplePairs.values();
        std::sort(connects.begin(),connects.end(),shallow);
        QMap<int,QList<QSampleMatcher*>> leftMaps;
        for(int i=1;i<=leftSize;i++){
            QList<QSampleMatcher*> matchers=leftMaps[leftNo+i];
            for(int j=0;j<connects.size();j++){
                if(leftNo+i-1==connects[j]->leftSampleNo){
                    matchers.append(connects[j]);
                    leftMaps[leftNo+1]=matchers;
                }
            }
            std::sort(matchers.begin(),matchers.end(),shallowRight);
        }
        for(int i=0;i<leftMaps.size();i++){
            QVector<QPointF> verts;
            verts.append(QPointF(rev?this->_distance:0,left->samples[leftNo+i]->bottom()));
            QSampleMatcher * x=leftMaps[leftNo+1+i].last();

            verts.append(QPointF(rev?0:this->_distance,right->samples[leftMaps[leftNo+1+i].last()->rightSampleNo]->bottom()));
            verts.append(QPointF(rev?this->_distance:0,left->samples[leftNo+i+1]->bottom()));
            QGeoFormation * formation=new QGeoFormation(verts,left->samples[leftNo+i+1]->desc(),this);
            this->AddFormation(formation);
            left->samples[leftNo+i+1]->setFormation(formation);
        }

        QMap<int,QList<QSampleMatcher*>> rightMaps;
        for(int i=1;i<=rightSize;i++){
            QList<QSampleMatcher*> matchers=rightMaps[rightNo+i];
            for(int j=0;j<connects.size();j++){
                if(rightNo+i-1==connects[j]->rightSampleNo){
                    matchers.append(connects[j]);
                    rightMaps[rightNo+i]=matchers;
                }
            }
            std::sort(matchers.begin(),matchers.end(),shallow);
        }
        for(int i=0;i<rightMaps.size();i++){
            QVector<QPointF> verts;
            verts.append(QPointF(!rev?this->_distance:0,right->samples[rightNo+i]->bottom()));
            verts.append(QPointF(!rev?0:this->_distance,left->samples[rightMaps[rightNo+i+1].last()->leftSampleNo]->bottom()));
            verts.append(QPointF(!rev?this->_distance:0,right->samples[rightNo+i+1]->bottom()));
            QGeoFormation * formation=new QGeoFormation(verts,right->samples[rightNo+i+1]->desc(),this);
            this->AddFormation(formation);
            right->samples[rightNo+i+1]->setFormation(formation);
        }
    }
}
void QSection::ProcessSamples(QWellbore* left ,QWellbore *right, QMap<int ,QList<int>> & leftLinkedsampleMap,QMap<int ,QList<int>> & rightLinkedsampleMap,bool rev){
    int cursampleIndex=0;
    int prevLinkedsampleIndex=0;

    while(cursampleIndex<left->samples.size()){
        QGeoSample *cursample=left->samples[cursampleIndex];
        if (leftLinkedsampleMap.contains(cursampleIndex)){

            if (leftLinkedsampleMap[cursampleIndex].size()==1){
                //x-------x
                int rightsample=leftLinkedsampleMap[cursampleIndex].first();
                if(rightLinkedsampleMap[rightsample].size()==1){
                    //1:1
                    // x-------x
                    if (rev==false){
                        //                        sampleLinks->AddLink(new QSampleLink(cursampleIndex,Bottom, rightsample,Bottom));
                        if(cursampleIndex>0 && cursampleIndex<left->samples.size()-1){
                            QVector<QPointF> verts;
                            verts.append(QPointF(0,left->samples[cursampleIndex]->top()));
                            verts.append(QPointF(_distance,right->samples[rightsample]->top()));
                            verts.append(QPointF(_distance,right->samples[rightsample]->bottom()));
                            verts.append(QPointF(0,left->samples[cursampleIndex]->bottom()));

                            QGeoFormation * formation=new QGeoFormation(verts,left->samples[cursampleIndex]->desc(),this);
                            this->AddFormation(formation);
                            left->samples[cursampleIndex]->setFormation(formation);
                            right->samples[rightsample]->setFormation(formation);
                        }
                    }
                }else if(rightLinkedsampleMap[rightsample].size()>1){
                    //1:n,对方有 n 个连接
                    // x--------x
                    // y--------|
                    // x--------| *
                    int topLeft=rightLinkedsampleMap[rightsample].first();
                    int bottomLeft=rightLinkedsampleMap[rightsample].last();
                    if (topLeft==cursampleIndex){
                        if(rev==false){

                            QVector<QPointF> verts;
                            verts.append(QPointF(0,left->samples[topLeft]->top()));
                            verts.append(QPointF(_distance,right->samples[rightsample]->top()));
                            verts.append(QPointF(_distance,right->samples[rightsample]->bottom()));
                            verts.append(QPointF(0,left->samples[bottomLeft]->bottom()));

                            QGeoFormation * formation=new QGeoFormation(verts,left->samples[cursampleIndex]->desc(),this);
                            this->AddFormation(formation);
                            for(int j=topLeft;j<=bottomLeft;j++){
                                if(left->samples[j]->desc()==left->samples[topLeft]->desc()){
                                    left->samples[j]->setFormation(formation);
                                }
                            }
                            right->samples[rightsample]->setFormation(formation);
                            //                            sampleLinks->AddLink(new QSampleLink(rightLinkedsampleMap[rightsample].first(),Bottom, rightsample,Bottom));
                            //                            sampleLinks->AddLink(new QSampleLink(cursampleIndex,Bottom, rightsample,Bottom));
                        }
                    }
                }
            }else if(leftLinkedsampleMap[cursampleIndex].size()>1){
                //1:n
                // x-------x
                // |-------y
                // |-------x *
                int rightbottom=leftLinkedsampleMap[cursampleIndex].last();
                int righttop=leftLinkedsampleMap[cursampleIndex].first();
                if(rev==false){

                    QVector<QPointF> verts;
                    verts.append(QPointF(0,left->samples[cursampleIndex]->top()));
                    verts.append(QPointF(_distance,right->samples[righttop]->top()));
                    verts.append(QPointF(_distance,right->samples[rightbottom]->bottom()));
                    verts.append(QPointF(0,left->samples[cursampleIndex]->bottom()));

                    QGeoFormation * formation=new QGeoFormation(verts,left->samples[cursampleIndex]->desc(),this);
                    this->AddFormation(formation);

                    for(int j=righttop;j<=rightbottom;j++){
                        if(right->samples[j]->desc()==right->samples[righttop]->desc()){
                            right->samples[j]->setFormation(formation);
                        }
                    }
                    left->samples[cursampleIndex]->setFormation(formation);
                    //                    sampleLinks->AddLink(new QSampleLink(cursampleIndex,Bottom, leftLinkedsampleMap[cursampleIndex].first(),Bottom));
                    //                    sampleLinks->AddLink(new QSampleLink(cursampleIndex,Bottom, rightsample,Bottom));
                }
            }
            prevLinkedsampleIndex=cursampleIndex;
            cursampleIndex++;
        }else{
            //                非连续分布地层
            //查找下一连续地层
            int nextLinkedsampleIndex=cursampleIndex+1;
            while(nextLinkedsampleIndex<left->samples.size()){
                if(leftLinkedsampleMap.contains(nextLinkedsampleIndex)){//左侧链接信息中，标注改层链接信息不为空，则说明此层有对应的右侧地层，找到了下一连续地层
                    break;
                }
                nextLinkedsampleIndex++;
            }
            int diffOfRight =leftLinkedsampleMap[nextLinkedsampleIndex].first()-leftLinkedsampleMap[prevLinkedsampleIndex].last();//右侧的地层间隔
            int diffOfLeft=nextLinkedsampleIndex-prevLinkedsampleIndex;//左侧地层间隔
            if(prevLinkedsampleIndex>=cursampleIndex){
                cursampleIndex++;
                continue;
            }
            if (diffOfRight==0){
                //右侧尖灭地层

                QGeoFormation * prevFormation=left->samples[cursampleIndex-1]->formation();
                if(prevFormation!=nullptr){
                    QVector<QPointF> verts;
                    if(!rev){
                        verts.append(QPointF(0,left->samples[cursampleIndex]->top()));
                        verts.append(QPointF(_distance,right->samples[leftLinkedsampleMap[cursampleIndex-1].last()]->bottom()));
                        verts.append(QPointF(0,left->samples[cursampleIndex]->bottom()));
                    }else{
                        verts.append(QPointF(_distance,left->samples[cursampleIndex]->top()));
                        verts.append(QPointF(0,right->samples[leftLinkedsampleMap[cursampleIndex-1].last()]->bottom()));
                        verts.append(QPointF(_distance,left->samples[cursampleIndex]->bottom()));
                    }
                    prevFormation->merge(verts);
                }
                if(left->samples[cursampleIndex]->Thickness()<0*Y_SCALE){
                    //                    规则 1:相邻钻孔间，若存在厚度小于 2m 的夹层，则该层忽略不计，将该层与相邻地 层进行合并;
                    if(cursampleIndex>1){
                        left->samples[cursampleIndex]->setDesc(left->samples[cursampleIndex-1]->desc());
                        left->samples[cursampleIndex]->setFormation(left->samples[cursampleIndex-1]->formation());
                    }else{
                        left->samples[cursampleIndex]->setDesc(left->samples[cursampleIndex+1]->desc());
                        //                        left->samples[cursampleIndex]->setFormation(left->samples[cursampleIndex+1].formation());
                    }
                }else{
                    float xPosScale=0.5;
                    if(left->samples[cursampleIndex]->Thickness()<5*Y_SCALE){
                        xPosScale=0.33;
                    }else if(left->samples[cursampleIndex]->Thickness()<8*Y_SCALE){
                        xPosScale=0.5;
                    }else if(left->samples[cursampleIndex]->Thickness()>=8*Y_SCALE){
                        xPosScale=0.66;
                    }
                    float vanishPosX=xPosScale*_distance;
                    float curLeftSampleTop=left->samples[cursampleIndex]->top();
                    float curRightSampleTop=right->samples[leftLinkedsampleMap[cursampleIndex-1].last()]->bottom();
                    float curLeftSampleBottom=left->samples[cursampleIndex]->bottom();

                    float vanishPosYT=curLeftSampleTop-(curLeftSampleTop-curRightSampleTop)*xPosScale;
                    float vanishPosYB=curLeftSampleBottom-(curLeftSampleBottom-curRightSampleTop)*xPosScale;
                    float vanishPosY=(vanishPosYB+vanishPosYT)/2.0;

                    QVector<QPointF> verts;
                    if(!rev){
                        verts.append(QPointF(0,left->samples[cursampleIndex]->top()));
                        verts.append(QPointF(vanishPosX,vanishPosY));
                        verts.append(QPointF(0,left->samples[cursampleIndex]->bottom()));
                    }else{
                        verts.append(QPointF(_distance,left->samples[cursampleIndex]->top()));
                        verts.append(QPointF(_distance-vanishPosX,vanishPosY));
                        verts.append(QPointF(_distance,left->samples[cursampleIndex]->bottom()));
                    }
                    QGeoFormation * formation=new QGeoFormation(verts,left->samples[cursampleIndex]->desc(),this);
                    this->AddFormation(formation);
                    prevFormation->sub(verts);
                    left->samples[cursampleIndex]->setFormation(formation);
                }
                left->samples[cursampleIndex]->setVanish(true);
                cursampleIndex++;

                //            }else if (diff==1){
                //                //B[m]在上，B[n]在下，右侧缺失地层,连接地层 A[i]的下分界点和 B[m] 的下分界点，A[i]的上分界点和 B[n]的上分界点
                ////                  -A-    -B-
                ////                   x      x
                ////                  --------- --
                ////                     \ /    --1
                ////                   d /\  e
                ////                   /   \    --2
                ////                  --------- --3
                ////                   y     y
                ////                  --    --

                //                sampleLinks.append(new QSampleLink(cursampleIndex,Bottom, leftLinkedsampleMap[prevLinkedsampleIndex].last(),Bottom)); //1
                //                sampleLinks.append(new QSampleLink(cursampleIndex,Top, leftLinkedsampleMap[nextLinkedsampleIndex].first(),Top)); //2
                //                sampleLinks.append(new QSampleLink(cursampleIndex,Bottom, leftLinkedsampleMap[nextLinkedsampleIndex].first(),Top));//3
            }else if (diffOfRight>0 && !rev){
                this->ProcessMissingFormations(left,right,cursampleIndex-1,diffOfLeft-1,leftLinkedsampleMap[ prevLinkedsampleIndex].last(),diffOfRight-1,rev);
                cursampleIndex+=diffOfLeft-1;

            }else if (diffOfRight>0 && rev && left->samples[cursampleIndex]->formation()==nullptr){

                //若 diff>=2，则 A[i]与 B[n]和 B[m]之间的地层构成间断缺失模型，根据间断缺失 地层模型的尖灭规则选取尖灭点，依次连接地层 A[i]的上下分界点与其尖灭点;
                //                针对地层间断缺失的情况，考虑到地层连接线的平缓，根据地层连线斜率的大小确定其尖灭点。
                //                若地层与相邻钻孔中的地层构成间断缺失，则相邻钻孔中与其构成间断缺失的各地层的分界点都有可能是该地层的尖灭点。
                //                分别计算该地层的上下分界点与上述各点的连线的 斜率，取两连线的较大值作为与各点平缓的参考值，取参考值最小的点作为尖灭点。
                bool onLeftSide=true;//左侧
                int leftsampleIndex=cursampleIndex;
                int rightsampleIndex=leftLinkedsampleMap[prevLinkedsampleIndex].last();
                //方法：以左侧为准，每次取左侧一个地层底部连到右侧顶部做尖灭，再反过来取右侧一个地层底部连接到左侧地层底部做尖灭，交替进行
                while(true){
                    if(onLeftSide){//左侧

                        QVector<QPointF> verts;
                        verts.append(QPointF(_distance,left->samples[leftsampleIndex]->top()));
                        verts.append(QPointF(0,right->samples[rightsampleIndex]->bottom()));
                        verts.append(QPointF(_distance,left->samples[leftsampleIndex]->bottom()));

                        QGeoFormation * formation=new QGeoFormation(verts,left->samples[leftsampleIndex]->desc(),this);
                        this->AddFormation(formation);
                        left->samples[leftsampleIndex]->setFormation(formation);
                        //                        sampleLinks->AddLink(new QSampleLink(leftsampleIndex,Bottom,rightsampleIndex,Top));
                        //如果
                        if(rightsampleIndex<leftLinkedsampleMap[nextLinkedsampleIndex].first()-1){
                            onLeftSide=false;
                            rightsampleIndex++;
                        }else{
                            leftsampleIndex++;
                        }

                    }else{

                        QVector<QPointF> verts;
                        verts.append(QPointF(_distance,left->samples[leftsampleIndex]->bottom()));
                        verts.append(QPointF(0,right->samples[rightsampleIndex]->top()));
                        verts.append(QPointF(0,right->samples[rightsampleIndex]->bottom()));

                        QGeoFormation * formation=new QGeoFormation(verts,right->samples[rightsampleIndex]->desc(),this);
                        this->AddFormation(formation);

                        right->samples[rightsampleIndex]->setFormation(formation);
                        //                        sampleLinks->AddLink(new QSampleLink(leftsampleIndex,Bottom,rightsampleIndex,Bottom));
                        if(leftsampleIndex<nextLinkedsampleIndex-1){
                            onLeftSide=true;
                            leftsampleIndex++;
                        }else{
                            rightsampleIndex++;
                        }

                    }
                    if ((onLeftSide && leftsampleIndex>=nextLinkedsampleIndex) || (!onLeftSide && rightsampleIndex>=leftLinkedsampleMap[nextLinkedsampleIndex].first())){
                        break;
                    }
                }
                cursampleIndex=nextLinkedsampleIndex;
            }else{
                cursampleIndex++;
            }
            if(cursampleIndex>nextLinkedsampleIndex)
                prevLinkedsampleIndex=nextLinkedsampleIndex;
        }
    }
}

void QSection::AddFormation(QGeoFormation *formation)
{
    this->_formations.append(formation);
}
float QSection::depth(){
    return this->bottom()-this->top();
}

QGeoFormation * QSection::CurFormation(QPointF p)
{
    for(int i=0;i<_formations.size();i++){
        QGeoFormation * formation=_formations[i];
        if(formation->In(p)){
            return formation;
        }
    }
    return nullptr;
}
void QSection::drawGround(QPainter *painter)
{
    QPainterPath groundPath;

    QPolygonF ground;

    QTransform  transform2;

    groundPath.moveTo(QPointF(0,left()->top()-GROUND_THICKNESS));
    groundPath.lineTo(QPointF(distance(),right()->top()-GROUND_THICKNESS));
    groundPath.lineTo(QPointF(distance(),right()->top()));
    groundPath.lineTo(QPointF(0,left()->top()));
    groundPath.lineTo(QPointF(0,left()->top()-GROUND_THICKNESS));

    transform2.scale(2,2);
    QBrush brush=QBrush(Qt::BDiagPattern);
    brush.setTransform(transform2);
    painter->setBrush(brush);
    painter->setPen(QPen(QColor(79, 106, 25), 0.01, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));

    painter->drawPath(groundPath);
    painter->setPen(QPen(QBrush(Qt::black),2,Qt::SolidLine));
    painter->drawLine(QPointF(0,left()->samples[0]->top()),QPointF(distance(),right()->samples[0]->top()));

    groundPath.clear();
    groundPath.moveTo(QPointF(0,left()->bottom()+GROUND_THICKNESS));
    groundPath.lineTo(QPointF(distance(),right()->bottom()+GROUND_THICKNESS));
    groundPath.lineTo(QPointF(distance(),right()->bottom()));
    groundPath.lineTo(QPointF(0,left()->bottom()));
    groundPath.lineTo(QPointF(0,left()->bottom()+GROUND_THICKNESS));
    brush=QBrush(Qt::BDiagPattern);
    brush.setTransform(transform2);
    painter->setBrush(brush);
    //    groundPath.addPolygon(ground);
    painter->setPen(QPen(QColor(79, 106, 25), 0.01, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));

    painter->drawPath(groundPath);
    painter->setPen(QPen(QBrush(Qt::black),2,Qt::SolidLine));
    painter->drawLine(QPointF(0,left()->bottom()),QPointF(distance(),right()->bottom()));

}
void QSection::drawOneGround(QPainter *painter, QTransform& transform,QPolygonF & poly,QLineF  & line){
    painter->save();
    QPainterPath groundPath;
    groundPath.addPolygon(transform.map(poly));

    QBrush brush=QBrush(Qt::BDiagPattern);
    painter->setBrush(brush);
    painter->setPen(QPen(QColor(79, 106, 25),0, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));
    painter->fillPath(groundPath,brush);
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine));
    painter->drawLine(transform.map(line));
    painter->restore();
}
void QSection::drawGround(QPainter *painter, QTransform& transform2)
{

    QPolygonF ground;

    ground.append(QPointF(0,left()->top()-GROUND_THICKNESS));
    ground.append(QPointF(distance(),right()->top()-GROUND_THICKNESS));
    ground.append(QPointF(distance(),right()->top()));
    ground.append(QPointF(0,left()->top()));
    ground.append(QPointF(0,left()->top()-GROUND_THICKNESS));
    QLineF bottom=QLineF(
                QPointF(0,left()->top()),
                QPointF(distance(),right()->top())
                );
    this->drawOneGround(painter,transform2,ground,bottom);

    QPolygonF  ground2;
    ground2.append(QPointF(0,left()->bottom()+GROUND_THICKNESS));
    ground2.append(QPointF(distance(),right()->bottom()+GROUND_THICKNESS));
    ground2.append(QPointF(distance(),right()->bottom()));
    ground2.append(QPointF(0,left()->bottom()));
    ground2.append(QPointF(0,left()->bottom()+GROUND_THICKNESS));
    QLineF bottom2=QLineF(
                QPointF(0,left()->bottom()),
                QPointF(distance(),right()->bottom())
                );
    this->drawOneGround(painter,transform2,ground2,bottom2);

}
void calInteractions(QMap<QString,QSampleMatcher*> &  samplePairs){
    QList<QSampleMatcher*> connects=samplePairs.values();
    for(int i=0;i<connects.size();i++){
        connects[i]->intersactions.clear();
    }
    for(int i=0;i<connects.size();i++){
        QSampleMatcher* conA=connects[i];
        for(int j=i+1;j<connects.size();j++){
            QSampleMatcher *conB=connects[j];
            if(conA->leftSampleNo>conB->leftSampleNo && conA->rightSampleNo>conB->rightSampleNo){
                //1-1 2-2,1-2,2-3,3-1,4-2-->无交点
            } else if(conA->leftSampleNo>conB->leftSampleNo && conA->rightSampleNo<conB->rightSampleNo){
                //1-2 2-1
                conA->intersactions[j]=conB;
                conB->intersactions[i]=conA;
            }else if(conA->leftSampleNo<conB->leftSampleNo && conA->rightSampleNo<conB->rightSampleNo){
                //无交点
            }else if(conA->leftSampleNo<conB->leftSampleNo && conA->rightSampleNo>conB->rightSampleNo){
                conA->intersactions[j]=conB;
                conB->intersactions[i]=conA;
            }
        }
    }
}
void QSection::processConnections(QMap<QString,QSampleMatcher*> &  samplePairs,connector_comparor c){
    QList<QSampleMatcher*> connects=samplePairs.values();
    //    (2.1)统计各连接线与其余连线的交点数目
    calInteractions(samplePairs);
    //排序
    while(true){
        std::sort(connects.begin(), connects.end(), c);
        if(connects.size()>0 && connects[0]->intersactions.isEmpty()){
            //没有任何相交直线了
            break;
        }
        qDebug()<<"MostIntersactions:"<<connects[0]->intersactions.size()<<",is "<<connects[0]->leftSampleNo<<"-"<<connects[0]->rightSampleNo;
        connects[0]->PrintIntersactions();
        //移除交点最多的一个
        QString mostInterSecConKey=QString("%1-%2").arg(connects[0]->leftSampleNo).arg(connects[0]->rightSampleNo);
        qDebug()<<"mostInterSecConKey"<<":"<<mostInterSecConKey;
        samplePairs.remove(mostInterSecConKey);
        qDebug()<<"left:"<<samplePairs.size();
        calInteractions(samplePairs);
        connects=samplePairs.values();

    }
}
void QSection::process(){
// 地层连接字典，比如左1和右2层系相同，则对应 1-2: matcher
    QMap<QString,QSampleMatcher*>samplePairs;
//    第一步，增加封顶和封底虚拟层
    this->_left->seal();
    this->_right->seal();
//    第二步，将左右井眼地层按照地质属性相连，只要是同一地址属性，就建立连接
    for(int i=0;i<_left->samples.size();i++){
        for (int j=0;j<_right->samples.size();j++){
            if(_left->samples[i]->desc().compare(_right->samples[j]->desc(),Qt::CaseInsensitive)==0){
                QSampleMatcher* con=new QSampleMatcher(i,j,this);
                samplePairs[QString("%1-%2").arg(con->leftSampleNo).arg(con->rightSampleNo)]=con;
            }
        }
    }
//    统计各连接线与其余连线的交点数目,去掉相交的地层匹配，保证无交叉
    this->processConnections(samplePairs,more_intersactions);
    //将匹配的地层按照左侧地层深度进行排序，
    QList<QSampleMatcher*> connects=samplePairs.values();
    std::sort(connects.begin(),connects.end(),shallow);
    qDebug()<<"Section:"<<this->name();
    for(int i=0;i<connects.size();i++){
        qDebug()<<"\t |"<<i<<"|"<<connects[i]->leftSampleNo<<"-"<<connects[i]->rightSampleNo<<"|"<<_left->samples[connects[i]->leftSampleNo]->desc();
    }

    QStringList sampleConnectKeys=samplePairs.keys();
    //建立两个字典，
//    leftLinkedsampleMap,记录每一个左侧地层和哪几个右侧地层匹配,key是左侧地层序号，value是右侧匹配地层序号数组，可能是0-n项
//    rightLinkedsampleMap,记录每一个右侧地层和哪几个右侧地层匹配,key是左侧地层序号，value是右侧匹配地层序号数组，可能是0-n项
//    根据上面消掉交叉后的底层匹配建立这两个字典
    QMap<int ,QList<int>> leftLinkedsampleMap,rightLinkedsampleMap;
    foreach(auto sampleConnect ,sampleConnectKeys){
        QStringList parts=sampleConnect.split('-');
        assert(parts.length()==2);

        QList<int> linkedsamples=leftLinkedsampleMap[parts[0].toInt()];
        linkedsamples.append(parts[1].toInt());
        std::sort(linkedsamples.begin(),linkedsamples.end(),std::less<int>());
        leftLinkedsampleMap[parts[0].toInt()]=linkedsamples;

        linkedsamples=rightLinkedsampleMap[parts[1].toInt()];
        linkedsamples.append(parts[0].toInt());
        std::sort(linkedsamples.begin(),linkedsamples.end(),std::less<int>());
        rightLinkedsampleMap[parts[1].toInt()]=linkedsamples;
    }
    //处理尖灭、1对多，多对1的情况，分两趟，一趟以左侧视角进行，再以右侧视角进行
    this->ProcessSamples(_left,_right,leftLinkedsampleMap,rightLinkedsampleMap,false);
    this->ProcessSamples(_right,_left,rightLinkedsampleMap,leftLinkedsampleMap,true);
    return;
}

QString QSection::name()
{
    return QString("%1-%2").arg(_left->name()).arg(_right->name());
}
float QSection:: top(){
    return _left->top()<_right->top()?_left->top():_right->top();
}
float QSection::bottom()
{
    return _left->bottom()>_right->bottom()?_left->bottom():_right->bottom();
}
float QSection:: width(){
    return _distance;
}

QWellbore *QSection::left() const
{
    return _left;
}

void QSection::setLeft(QWellbore *left)
{
    _left = left;
}

QWellbore *QSection::right() const
{
    return _right;
}

void QSection::setRight(QWellbore *right)
{
    _right = right;
}

float QSection::distance() const
{
    return _distance;
}

void QSection::setDistance(float distance)
{
    _distance = distance;
}

QList<QGeoFormation *> QSection::formations() const
{
    return _formations;
}

void QSection::setFormations(const QList<QGeoFormation *> &formations)
{
    _formations = formations;
}
QSampleLink::QSampleLink(int ln,ConnectPoint lc, int rn,ConnectPoint rc, QObject *parent):QObject(parent),leftSampleNo(ln),rightSampleNo(rn),leftConnectPoint(lc),rightConnectPoint(rc)
{

}

ConnectPoint QSampleLink::getRightConnectPoint() const
{
    return rightConnectPoint;
}

void QSampleLink::setRightConnectPoint(const ConnectPoint &value)
{
    rightConnectPoint = value;
}

ConnectPoint QSampleLink::getLeftConnectPoint() const
{
    return leftConnectPoint;
}

void QSampleLink::setLeftConnectPoint(const ConnectPoint &value)
{
    leftConnectPoint = value;
}

QSampleLinks::QSampleLinks(QObject *parent):QObject(parent)
{

}

void QSampleLinks::AddLink(QSampleLink *samplelink)
{
    QMap<QString,QSampleLink*> linked;

    if(samplelink->leftSampleNo>0&&samplelink->leftConnectPoint==Top){
        samplelink->leftSampleNo--;
        samplelink->leftConnectPoint=Bottom;
    }
    if (samplelink->rightSampleNo>0&&samplelink->rightConnectPoint==Top){
        samplelink->rightSampleNo--;
        samplelink->rightConnectPoint=Bottom;
    }
    QString key=QString("%1-%2-%3-%4").arg(samplelink->leftSampleNo).arg(samplelink->leftConnectPoint).arg(samplelink->rightSampleNo).arg(samplelink->rightConnectPoint);
    if(!_links.contains(key)){
        _links[key]=samplelink;
    }
    return;
}

void QSampleLinks::Normalize()
{
//    QList<QSampleLink*> samplelinks = _links.values();
//    std::sort(samplelinks.begin(),samplelinks.end(),linkerCompare);
//    for(int i=0;i<samplelinks.size();i++){
//        _linksByLeft[samplelinks[i]->leftSampleNo].append(samplelinks[i]);
//    }
//    std::sort(samplelinks.begin(),samplelinks.end(),linkerCompareRev);
//    for(int i=0;i<samplelinks.size();i++){
//        _linksByRight[samplelinks[i]->rightSampleNo].append(samplelinks[i]);
//    }
}

QGeoFormation* QGeoSample::formation() const
{
    return _formation;
}

void QGeoSample::setFormation( QGeoFormation * formation)
{
    _formation = formation;
}

void QGeoSample::paint(QPainter *painter,QTransform &transform,int align, float width,bool last)
{
    painter->save();
    //    painter->fillRect(QRectF(QPointF(0,this->top()-10),QPointF(300,this->top()+10)),Qt::white);
    painter->setPen(QPen(QColor(DARK_RED), 1, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));
    if(align==0){
        QPointF pos=transform.map(QPointF(0,this->bottom()));
        QPointF pos2=pos;
        pos2+=QPointF(TICK_WIDTH,0);
        painter->drawLine(pos,pos2);
    }else{
        QPointF pos=transform.map(QPointF(width,this->bottom()));
        QPointF pos2=pos;
        pos2-=QPointF(TICK_WIDTH,0);
        painter->drawLine(pos,pos2);
    }


    QFont font("宋体", 8, QFont::ExtraBold, true);   //字体，大小，粗体，斜体
    font.setCapitalization(QFont::Capitalize);   //设置字母大小写
    //    font.setUnderline(true); //设置下划线
    //    font.setOverline(true); //设置上划线
    // font.setLetterSpacing(QFont::AbsoluteSpacing, 10); //设置字符间的间距
    painter->setFont(font);
    QRectF txtBounder=QRectF(transform.map(QPointF(0,this->bottom()))+=QPointF(TICK_WIDTH,-5),
                             transform.map(QPointF(width,this->bottom()))+=QPointF(-TICK_WIDTH,5));
    QRectF txtDescBounder=QRectF(transform.map(QPointF(0,this->bottom()))+=QPointF(TICK_WIDTH,0),
                             transform.map(QPointF(width,this->top()))+=QPointF(-TICK_WIDTH,0));
//    txtNextBounder.adjust(0,10,0,10);
    if(_seal==0 ){
        if(align==0){
            painter->drawText(txtBounder,Qt::AlignLeft|Qt::AlignTop,QString("%1").arg(QString::number(this->bottom()/Y_SCALE,'f',2)));
            painter->drawText(txtDescBounder,Qt::AlignLeft|Qt::AlignVCenter,QString("%1").arg(this->desc()));
        }else{
            painter->drawText(txtBounder,Qt::AlignRight|Qt::AlignTop,QString("%1").arg(QString::number(this->bottom()/Y_SCALE,'f',2)));
            painter->drawText(txtDescBounder,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(this->desc()));
        }

    }else if (_seal==2){
        if(align==0){
            painter->drawText(txtBounder,Qt::AlignLeft|Qt::AlignTop,QString("%1").arg(QString::number(this->bottom()/Y_SCALE,'f',2)));

        }else{
            painter->drawText(txtBounder,Qt::AlignRight|Qt::AlignTop,QString("%1").arg(QString::number(this->bottom()/Y_SCALE,'f',2)));
        }
    }
    painter->restore();
}

QGeoFormation::QGeoFormation(QPainterPath  contour, QString desc, QObject *parent):QObject(parent),contour(contour),_desc(desc)
{

}

QGeoFormation::QGeoFormation(QVector<QPointF> verts, QString desc, QObject *parent):QObject(parent)
{
    this->contour.addPolygon(QPolygonF(verts));
    this->_desc=desc;
}

QString QGeoFormation::desc() const
{
    return _desc;
}

bool QGeoFormation::In(QPointF p)
{
    return this->contour.contains(p);
}

void QGeoFormation::merge(QVector<QPointF> verts)
{
    QPainterPath addtionalPath;
    addtionalPath.addPolygon(QPolygonF(verts));
    this->contour+=addtionalPath;
}

void QGeoFormation::sub(QVector<QPointF> verts)
{
    QPainterPath addtionalPath;
    addtionalPath.addPolygon(QPolygonF(verts));
    this->contour-=addtionalPath;
}

void QGeoFormation::setDesc(const QString &desc)
{
    _desc = desc;
}

void QGeoFormation::paint(QPainter *painter,QTransform& transform)
{
    painter->save();
    //    QPainterPath formationPath;
    QTransform  brushTransform;
    //    QPolygonF formationBorder=QPolygonF(verts);
    //    formationPath.addPolygon(formationBorder);
    brushTransform.scale(BRUSH_X_SCALE,BRUSH_Y_SCALE);
    QBrush brush=QGeoSectionScene::getLegend(this->desc());
    brush.setTransform(brushTransform);
    painter->setBrush(brush);
    painter->setPen(QPen(QColor(79, 106, 25), 1, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));

    painter->drawPath(transform.map(this->contour));
    painter->restore();

}
