#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPoint>
#include <QList>
#include <QTimer>
#include <QTime>
#include "pointmoveinformation.h"
#include "updater.h"
#include "pointdoglegmoveinformation.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //events 
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);



public slots:
    void timeUp();
signals:
    void triggerUpdate(QList<QGraphicsEllipseItem*> &items,QList<PointDoglegMoveInformation> &informations,int interval, QList<int> &laneNums);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QPoint mousePressedPosOld;
    QPoint mousePressedPosNew;
    QGraphicsLineItem *tempLine;
    bool tempAdded;
    QPoint midPoint;
    QPoint midPoint1;
    QPoint midPoint2;
    QPoint midPoint3;
    QPoint midPoint4;
    QPoint midPoint5;
    QPoint midPoint6;
    QPoint midPoint7;
    QPoint midPoint8;
    QPoint midPoint9;
    QList<QPoint> vertices;
    QList<QPoint> midPoints1;
    QList<QPoint> midPoints2;
    QList<QPoint> midPoints3;
    QList<QPoint> midPoints4;
    QList<QPoint> midPoints5;
    QList<QPoint> midPoints6;
    QList<QPoint> midPoints7;
    QList<QPoint> midPoints8;
    QList<QPoint> midPoints9;

    QList<QPoint> tempMidPointList;
    QList<QList<QPoint> > allMidPoints;

//    QWidget window;
//    QSpinBox *pSpinBox;
//    QSlider *pSlider;
//    void setSliderAndSpinBox();
//    int nMin = 2;
//    int nMax = 5;
//    int nSingleStep = 1;

    Updater *updater;
    QList<QGraphicsEllipseItem *> items;
    QList<PointMoveInformation> informations;
    QList<PointDoglegMoveInformation> dogleginformations;
    int currentTime;

    QTimer *timer;

    bool firstTimeFlag = true;
    int playRound;
    int mousePressedNum;
    int createLaneNum;
    QList<int> laneNums;
};

#endif // MAINWINDOW_H
