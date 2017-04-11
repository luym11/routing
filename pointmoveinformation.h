#ifndef POINTMOVEINFORMATION_H
#define POINTMOVEINFORMATION_H
#define INT_MAX 2147483647

#include <QPoint>

class PointMoveInformation
{
public:
    //all times are in msec.
    PointMoveInformation(QPoint startPos,QPoint endPos,int moveTime,int startTime,int lifeTime =INT_MAX);
    QPoint startPos;
    QPoint endPos;
    int moveTime;
    int startTime;
    int lifeTime;
};

#endif // POINTMOVEINFORMATION_H

