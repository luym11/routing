#ifndef POINTDOGLEGMOVEINFORMATION_H
#define POINTDOGLEGMOVEINFORMATION_H
#define INT_MAX 2147483647


#include <QPoint>

class PointDoglegMoveInformation
{
public:
    //all times are in msec.
    PointDoglegMoveInformation(QPoint startPos,QPoint midPos, QPoint endPos,int moveTime,int startTime,int lifeTime =INT_MAX);
    QPoint startPos;
    QPoint midPos;
    QPoint endPos;
    int moveTime;
    int startTime;
    int lifeTime;
};

#endif // POINTDOGLEGMOVEINFORMATION_H



