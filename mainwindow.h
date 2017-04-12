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
    void triggerUpdate(QList<QGraphicsEllipseItem*> &items,QList<PointDoglegMoveInformation> &informations,int interval, int mousePressedNum);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QPoint mousePressedPosOld;
    QPoint mousePressedPosNew;
    QGraphicsLineItem *tempLine;
    bool tempAdded;
    QPoint midPoint;
    QPoint midPointUp;
    QPoint midPointDown;
    QList<QPoint> vertices;
    QList<QPoint> midPointsUp;
    QList<QPoint> midPointsDown;

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
};

#endif // MAINWINDOW_H
