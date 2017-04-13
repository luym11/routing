#include "updater.h"
#include <QDebug>
#include <QTime>

Updater::Updater(QGraphicsScene *bindScene,QObject *parent):QObject(parent)
{
    scene=bindScene;
    currentTime=0;
}

void Updater::update(QList<QGraphicsEllipseItem *> &items, QList<PointDoglegMoveInformation> &informations, int interval, QList<int> &laneNums){
//    if(items.length()!= ( informations.length()%mousePressedNum ) ){
//        qDebug()<<"Error! Update Terminated!";
//        return;
//    }
//
    currentTime+=interval;
    for(int i=0;i<informations.length();++i){
        //here must be written as this cause lifetime may be INT_MAX and any add to is will cause overflow 
        if(informations[i].lifeTime-currentTime<-informations[i].startTime){
            //clean those expired points
            scene->removeItem(items[i]);
            delete items.takeAt(i);
            informations.removeAt(i);
            i-=1;
        }
        else{
            if(informations[i].startTime+informations[i].moveTime<=currentTime){
                items[i]->setPos(informations[i].endPos);
                if(!scene->items().contains(items[i]))scene->addItem(items[i]);
            }
            else{
                //items[i]->setPos(informations[i].startPos+((informations[i].endPos-informations[i].startPos)*(currentTime-informations[i].startTime))/informations[i].moveTime);
                if(informations[i].startTime + (informations[i].moveTime/2) >= currentTime){
                    //first half
                    items[i]->setPos(informations[i].startPos+((informations[i].midPos-informations[i].startPos)*(currentTime-informations[i].startTime))/ (informations[i].moveTime/2) );
                    //items[i]->setPos(informations[i].startPos);
                }else{
                    //second half
                    items[i]->setPos(informations[i].midPos+((informations[i].endPos-informations[i].midPos)*(currentTime-( informations[i].startTime + (informations[i].moveTime/2))))/ (informations[i].moveTime/2) );
                }
                if(!scene->items().contains(items[i]))scene->addItem(items[i]);
            }
        }
    }
}
