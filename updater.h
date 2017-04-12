#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QGraphicsScene>
#include <QList>
#include <QGraphicsItem>
#include "pointmoveinformation.h"
#include "pointdoglegmoveinformation.h"

class Updater:public QObject
{
    Q_OBJECT
public:
    Updater(QGraphicsScene *bindScene,QObject *parent=0);// constructor
    int currentTime;


public slots:
    void update(QList<QGraphicsEllipseItem*> &items,QList<PointDoglegMoveInformation> &informations,int interval);


private:
    QGraphicsScene *scene;
};

#endif // UPDATER_H 
