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
#include <QtCore>



#define MAX_LANE_NUM 9
using namespace std;

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

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_spinBox_other_valueChanged(int arg1);

    void on_pushButton_10_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);

    void on_spinBox_4_valueChanged(int arg1);

    void on_spinBox_5_valueChanged(int arg1);

    void on_spinBox_6_valueChanged(int arg1);

    void on_spinBox_7_valueChanged(int arg1);

    void on_spinBox_8_valueChanged(int arg1);

    void on_spinBox_9_valueChanged(int arg1);

    void on_pushButton_run_clicked();

    void on_pushButton_11_clicked();

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

    int otherVehicleNum;
    void otherVehicleSetUp(int vehicleNum, int thislaneNum);
    QList<int> vehicleLanes;
    QList<int> vehicleNumOnEachLane;
    QList<int> vehicleNumOnEachLaneLastRound;

    void myVehicleSetUp(int clickedLane);
    QList<int> randomlyAssignLaneFunc(int vehicleNum, int laneNumOfThisRound);
    QList<int> findMinValueIndices(QList<int> vehicleNumOnEachLane);

    QList<int> laneAssigned;//should declare here, not in the function. That's what caused out of index
    QList<int> minValueIndices;
    QList<int> lastResultBasedAssignLaneFunc(int vehicleNum, int laneNumOfThisRound, QList<int> myVehicleNumOnEachLaneLastRound);

    int myVehicleNumOnLane1;
    int myVehicleNumOnLane2;
    int myVehicleNumOnLane3;
    int myVehicleNumOnLane4;
    int myVehicleNumOnLane5;
    int myVehicleNumOnLane6;
    int myVehicleNumOnLane7;
    int myVehicleNumOnLane8;
    int myVehicleNumOnLane9;

    void setUp();
    bool setUpFlag;
};

#endif // MAINWINDOW_H
