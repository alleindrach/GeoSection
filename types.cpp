#include "types.h"
#include <QObject>
#include <QtDebug>
#include  <QList>
#include <algorithm>
bool  more_intersactions(const QLayerMatcher * s1,const QLayerMatcher * s2)
{
    if(s1->intersactions.size()==s2->intersactions.size()){
        if(qAbs( s1->leftLayerNo-s1->rightLayerNo)>qAbs(s2->leftLayerNo-s2->rightLayerNo)){
            return true;
        }else{
            return false;
        }
    }
    return (s1->intersactions.size() > s2->intersactions.size());
}
bool shallow(const QLayerMatcher * s1,const QLayerMatcher *s2){
    return (s1->leftLayerNo<s2->leftLayerNo);
}
bool shallowLayer(const QGeoLayer *  s1,const QGeoLayer *s2){
    return (s1->bottom <s2->bottom);
}


double rad(double d) {
    return d * M_PI / 180.0;
}
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

QGeoLayer::QGeoLayer(float top,float bot,QString des,QObject *parent):QObject(parent),top(top),bottom(bot),desc(des){

}
void QGeoLayer::SetIndex(int index){
    this->index=index;
}


int QGeoLayer::Index(){
    return this->index;
}
float QGeoLayer::Thickness(){
    return this->bottom-this->top;
}

bool QGeoLayer::getVanish() const
{
    return vanish;
}

void QGeoLayer::setVanish(bool value)
{
    vanish = value;
}

QLayerMatcher::QLayerMatcher(int ln,int rn,QObject *parent):QObject(parent),leftLayerNo(ln),rightLayerNo(rn){

}

QString QLayerMatcher::toStr()
{

//    qDebug()<<"l="<<this->leftLayerNo;
    QString result= QString("%1-%2").arg(this->leftLayerNo).arg(this->rightLayerNo);
//    qDebug()<<"<<<<"<<result;
    return result;

}

void QLayerMatcher::PrintIntersactions()
{
    QList<int> keys=intersactions.keys();
    qDebug()<<"|||||"<<this->toStr()<<"交点集:";
    for(int i=0;i<keys.size();i++){
        qDebug()<<"\t"<<i<<"|"<<intersactions[keys[i]]->toStr();
    }
}
QWellbore::QWellbore(QString n,QObject *parent):QObject(parent),name(n){

}

QGeoLayer *QWellbore::AddLayer(float top,float bot, QString desc)
{
    QGeoLayer * newlayer=new QGeoLayer(top, bot,desc,this) ;
    this->layers.append(newlayer);
    std::sort(layers.begin(),layers.end(),shallowLayer);

    for(int i=0;i<this->layers.size();i++){
        this->layers[i]->SetIndex(i);
    }
    return newlayer;
    //    return nullptr;
}
void QWellbore::Print(){
    for(int i=0;i<this->layers.size();i++){
        qDebug()<<this->name<<""<<i<<",["<<this->layers[i]->top<<"="<<this->layers[i]->bottom<<","<<this->layers[i]->desc<<"]\n";
    }
}

void QWellbore::Seal()
{
    // 步骤 1:对所有钻孔进行数据预处理，在各钻孔顶端和底端添加 0 厚度的虚拟连续地层。
    this->AddLayer(this->top(),this->top(),"顶部虚拟层");
    this->AddLayer(this->bottom(),this->bottom(),"底部虚拟层");

}

float QWellbore::top()
{
    if(layers.size()>0)
    return layers[0]->top;
    else return 0;
}

float QWellbore::bottom()
{
    if(layers.size()>0){
        return layers[layers.size()-1]->bottom;
    }else
        return 0;
}

QSection::QSection(QWellbore *left ,QWellbore * right,float distance,QObject *parent):QObject(parent),left(left),right(right),distance(distance){

}

void ProcessLayers(QWellbore* left ,QWellbore *right, QMap<int ,QList<int>> & leftLinkedLayerMap,QMap<int ,QList<int>> & rightLinkedLayerMap,QList<QLayerLink *> & layerLinks,bool rev=false){
    int curlayerIndex=0;
    int prevLinkedLayerIndex=0;
    while(curlayerIndex<left->layers.size()){
        QGeoLayer *curlayer=left->layers[curlayerIndex];
        if (leftLinkedLayerMap.contains(curlayerIndex)){

            if (leftLinkedLayerMap[curlayerIndex].size()==1){
                //x-------x
                int rightLayer=leftLinkedLayerMap[curlayerIndex].first();
                if(rightLinkedLayerMap[rightLayer].size()==1){
                    //1:1
                    // x-------x
                    if (rev==false){
                        layerLinks.append(new QLayerLink(curlayerIndex,Bottom, rightLayer,Bottom));
                    }else{
                        layerLinks.append(new QLayerLink(rightLayer,Bottom,curlayerIndex,Bottom));
                    }
                }else if(rightLinkedLayerMap[rightLayer].size()>1){
                    //1:n,对方有 n 个连接
                    // x--------x
                    // y--------|
                    // x--------| *
                    int deepestLeft=rightLinkedLayerMap[rightLayer].last();
                    if (deepestLeft==curlayerIndex){
                        if(rev==false){
                            layerLinks.append(new QLayerLink(curlayerIndex,Bottom, rightLayer,Bottom));
                        }else{
                            layerLinks.append(new QLayerLink( rightLayer,Bottom,curlayerIndex,Bottom));
                        }

                    }
                }
            }else if(leftLinkedLayerMap[curlayerIndex].size()>1){
                //1:n
                // x-------x
                // |-------y
                // |-------x *
                int rightLayer=leftLinkedLayerMap[curlayerIndex].last();
                if(rev==false){
                    layerLinks.append(new QLayerLink(curlayerIndex,Bottom, rightLayer,Bottom));
                }else{
                    layerLinks.append(new QLayerLink( rightLayer,Bottom,curlayerIndex,Bottom));
                }
            }
            prevLinkedLayerIndex=curlayerIndex;
            curlayerIndex++;
        }else{
            //                非连续分布地层
            //查找下一连续地层
            int nextLinkedLayerIndex=curlayerIndex+1;
            while(nextLinkedLayerIndex<left->layers.size()){
                if(leftLinkedLayerMap.contains(nextLinkedLayerIndex)){//左侧链接信息中，标注改层链接信息不为空，则说明此层有对应的右侧地层，找到了下一连续地层
                    break;
                }
            }
            int diffOfRight =leftLinkedLayerMap[nextLinkedLayerIndex].first()-leftLinkedLayerMap[prevLinkedLayerIndex].last();//右侧的地层间隔
            int diffOfLeft=nextLinkedLayerIndex-prevLinkedLayerIndex;//左侧地层间隔
            if (diffOfRight==0){
                //右侧尖灭地层
                if(left->layers[curlayerIndex]->Thickness()<2){
//                    规则 1:相邻钻孔间，若存在厚度小于 2m 的夹层，则该层忽略不计，将该层与相邻地 层进行合并;
                    if(curlayerIndex>1){
                        left->layers[curlayerIndex]->desc=left->layers[curlayerIndex-1]->desc;
                    }else{
                        left->layers[curlayerIndex]->desc=left->layers[curlayerIndex+1]->desc;
                    }

                }
                left->layers[curlayerIndex]->setVanish(true);
                curlayerIndex++;

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

//                layerLinks.append(new QLayerLink(curlayerIndex,Bottom, leftLinkedLayerMap[prevLinkedLayerIndex].last(),Bottom)); //1
//                layerLinks.append(new QLayerLink(curlayerIndex,Top, leftLinkedLayerMap[nextLinkedLayerIndex].first(),Top)); //2
//                layerLinks.append(new QLayerLink(curlayerIndex,Bottom, leftLinkedLayerMap[nextLinkedLayerIndex].first(),Top));//3
            }else if (diffOfRight>0 && !rev){

//若 diff>=2，则 A[i]与 B[n]和 B[m]之间的地层构成间断缺失模型，根据间断缺失 地层模型的尖灭规则选取尖灭点，依次连接地层 A[i]的上下分界点与其尖灭点;
//                针对地层间断缺失的情况，考虑到地层连接线的平缓，根据地层连线斜率的大小确定其尖灭点。
//                若地层与相邻钻孔中的地层构成间断缺失，则相邻钻孔中与其构成间断缺失的各地层的分界点都有可能是该地层的尖灭点。
//                分别计算该地层的上下分界点与上述各点的连线的 斜率，取两连线的较大值作为与各点平缓的参考值，取参考值最小的点作为尖灭点。
                bool onLeftSide=true;//左侧
                int leftlayerIndex=curlayerIndex;
                int rightlayerIndex=leftLinkedLayerMap[prevLinkedLayerIndex].last()+1;
                //方法：以左侧为准，每次取左侧一个地层底部连到右侧顶部做尖灭，再反过来取右侧一个地层底部连接到左侧地层底部做尖灭，交替进行
                while(true){
                    if(onLeftSide){//左侧
                        layerLinks.append(new QLayerLink(leftlayerIndex,Bottom,rightlayerIndex,Top));
                        //如果
                        if(rightlayerIndex<leftLinkedLayerMap[nextLinkedLayerIndex].first()){
                            onLeftSide=false;
                        }
                        leftlayerIndex++;
                    }else{
                        layerLinks.append(new QLayerLink(leftlayerIndex,Bottom,rightlayerIndex,Bottom));
                        if(leftlayerIndex<nextLinkedLayerIndex-1){
                            onLeftSide=true;
                        }
                        rightlayerIndex++;
                    }
                    if ((onLeftSide && leftlayerIndex>=nextLinkedLayerIndex) || (!onLeftSide && rightlayerIndex>=leftLinkedLayerMap[nextLinkedLayerIndex].first())){
                        break;
                    }
                }
                curlayerIndex=nextLinkedLayerIndex;
            }
        }
    }
}

void QSection::Process(){
    //    (1)将相邻两钻孔中属性相同的地层层底进行连接。
    this->left->Seal();
    this->right->Seal();
    for(int i=0;i<left->layers.size();i++){
        for (int j=0;j<right->layers.size();j++){
            if(left->layers[i]->desc.compare(right->layers[j]->desc,Qt::CaseInsensitive)==0){
                QLayerMatcher* con=new QLayerMatcher(i,j,this);
                this->layerPairs[QString("%1-%2").arg(con->leftLayerNo).arg(con->rightLayerNo)]=con;

            }

        }
    }
    QList<QLayerMatcher*> connects=this->layerPairs.values();
    //    (2.1)统计各连接线与其余连线的交点数目
    for(int i=0;i<connects.size();i++){
        QLayerMatcher* conA=connects[i];
        for(int j=i+1;j<connects.size();j++){
            QLayerMatcher *conB=connects[j];
            if(conA->leftLayerNo>conB->leftLayerNo && conA->rightLayerNo>conB->rightLayerNo){
                //1-1 2-2,1-2,2-3,3-1,4-2-->无交点
            } else if(conA->leftLayerNo>conB->leftLayerNo && conA->rightLayerNo<conB->rightLayerNo){
                //1-2 2-1
                conA->intersactions[j]=conB;
                conB->intersactions[i]=conA;
            }else if(conA->leftLayerNo<conB->leftLayerNo && conA->rightLayerNo<conB->rightLayerNo){
                //无交点
            }else if(conA->leftLayerNo<conB->leftLayerNo && conA->rightLayerNo>conB->rightLayerNo){
                conA->intersactions[j]=conB;
                conB->intersactions[i]=conA;
            }
        }
    }
    //排序
    while(true){
        std::sort(connects.begin(), connects.end(), more_intersactions);
        if(connects.size()>0 && connects[0]->intersactions.isEmpty()){
            //没有任何相交直线了
            break;
        }
        qDebug()<<"MostIntersactions:"<<connects[0]->intersactions.size()<<",is "<<connects[0]->leftLayerNo<<"-"<<connects[0]->rightLayerNo;
        connects[0]->PrintIntersactions();
        //移除交点最多的一个
        QString mostInterSecConKey=QString("%1-%2").arg(connects[0]->leftLayerNo).arg(connects[0]->rightLayerNo);
        qDebug()<<"mostInterSecConKey"<<":"<<mostInterSecConKey;
        this->layerPairs.remove(mostInterSecConKey);
        qDebug()<<"left:"<<this->layerPairs.size();
        connects=this->layerPairs.values();

    }
    connects=this->layerPairs.values();
    std::sort(connects.begin(),connects.end(),shallow);
    qDebug()<<"Section:"<<this->name();
    for(int i=0;i<connects.size();i++){
        qDebug()<<"\t |"<<i<<"|"<<connects[i]->leftLayerNo<<"-"<<connects[i]->rightLayerNo<<"|"<<this->left->layers[connects[i]->leftLayerNo]->desc;
    }

    QStringList layerConnectKeys=layerPairs.keys();
    QMap<int ,QList<int>> leftLinkedLayerMap,rightLinkedLayerMap;
    foreach(auto layerConnect ,layerConnectKeys){
        QStringList parts=layerConnect.split('-');
        assert(parts.length()==2);

        QList<int> linkedLayers=leftLinkedLayerMap[parts[0].toInt()];
        linkedLayers.append(parts[1].toInt());
        std::sort(linkedLayers.begin(),linkedLayers.end(),std::greater<int>());
        leftLinkedLayerMap[parts[0].toInt()]=linkedLayers;

        linkedLayers=rightLinkedLayerMap[parts[1].toInt()];
        linkedLayers.append(parts[0].toInt());
        std::sort(linkedLayers.begin(),linkedLayers.end(),std::greater<int>());
        rightLinkedLayerMap[parts[1].toInt()]=linkedLayers;
    }

    ProcessLayers(this->left,this->right,leftLinkedLayerMap,rightLinkedLayerMap,this->layerLinks,false);
    ProcessLayers(this->right,this->left,rightLinkedLayerMap,leftLinkedLayerMap,this->layerLinks,false);
    return;
}

QString QSection::name()
{
    return QString("%1-%2").arg(this->left->name).arg(this->right->name);
}
float QSection:: Top(){
    return left->top()<right->top()?left->top():right->top();
}
float QSection::Bottom()
{
    return left->bottom()>right->bottom()?left->bottom():right->bottom();
}
float QSection:: Width(){
    return distance;
}
QLayerLink::QLayerLink(int ln,ConnectPoint lc, int rn,ConnectPoint rc, QObject *parent):QObject(parent),leftLayerNo(ln),rightlayerNo(rn),leftConnectPoint(lc),rightConnectPoint(rc)
{

}

ConnectPoint QLayerLink::getRightConnectPoint() const
{
    return rightConnectPoint;
}

void QLayerLink::setRightConnectPoint(const ConnectPoint &value)
{
    rightConnectPoint = value;
}

ConnectPoint QLayerLink::getLeftConnectPoint() const
{
    return leftConnectPoint;
}

void QLayerLink::setLeftConnectPoint(const ConnectPoint &value)
{
    leftConnectPoint = value;
}
