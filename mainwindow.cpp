﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

//remember to include <QDebug> in cpp files. printf() will not flush instantly in Qt GUI applications..
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //init a new scene and bind it to a view // ShenMeGui?
    scene=new QGraphicsScene(this);
    QRect rect=ui->graphicsView->rect(); //"created" a QRect?
    rect.setHeight(rect.height()-20);
    scene->setSceneRect(rect);
    ui->graphicsView->setScene(scene);
    //mouse events of the view are now handled by mainWindow
    ui->graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    tempLine =new QGraphicsLineItem();
    tempAdded =false;

    updater =new Updater(scene,this);
    currentTime=0;

    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUp()));
    connect(this,SIGNAL(triggerUpdate(QList<QGraphicsEllipseItem*>&,QList<PointDoglegMoveInformation>&,int, QList<int>&)),updater,SLOT(update(QList<QGraphicsEllipseItem*>&,QList<PointDoglegMoveInformation>&,int, QList<int>&)));
    timer->start(10);

    // init playRound
    playRound = 0;
    // init mousePressedNum
    mousePressedNum = -1;

    createLaneNum = 2;
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));

    //laneNums.append(createLaneNum);
//    setSliderAndSpinBox();
//    QHBoxLayout *layout = new QHBoxLayout;
//    layout->addWidget(pSpinBox);
//    layout->addWidget(pSlider);
//    window.setLayout(layout);
    otherVehicleNum = 4;
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox_valueChanged(int)));

    for(int i = 0; i <= MAX_LANE_NUM; i++){
        vehicleNumOnEachLane.append(0);
    }


}

MainWindow::~MainWindow()
{
    //remember those QObjects whose parent is this object have no need for deleting.
    //a QObject will automanticly delete all sons when it was destroyed.
    delete ui;
    if(tempLine)delete tempLine;
}

void MainWindow::timeUp(){
    currentTime+=timer->interval();
    ui->statusBar->showMessage(QTime::currentTime().toString());
    emit triggerUpdate(items,dogleginformations,timer->interval(), laneNums);

    ui->lcdNumber->display(vehicleNumOnEachLane.at(1));
    ui->lcdNumber_2->display(vehicleNumOnEachLane.at(2));
    ui->lcdNumber_3->display(vehicleNumOnEachLane.at(3));
    ui->lcdNumber_4->display(vehicleNumOnEachLane.at(4));
    ui->lcdNumber_5->display(vehicleNumOnEachLane.at(5));
    ui->lcdNumber_6->display(vehicleNumOnEachLane.at(6));
    ui->lcdNumber_7->display(vehicleNumOnEachLane.at(7));
    ui->lcdNumber_8->display(vehicleNumOnEachLane.at(8));
    ui->lcdNumber_9->display(vehicleNumOnEachLane.at(9));
}

//resize event
void MainWindow::resizeEvent(QResizeEvent *e){
    //resize the graphics view
    ui->graphicsView->resize(e->size().width(),e->size().height());
    QRect rect=ui->graphicsView->rect();
    rect.setHeight(rect.height()-20);
    scene->setSceneRect(rect);
}

//mouse events
void MainWindow::mousePressEvent(QMouseEvent *e){

    mousePressedPosNew = e->pos();

    if(firstTimeFlag == true){
        //draw an ellipse at the beginning of the line
        QGraphicsEllipseItem *startItem = new QGraphicsEllipseItem(mousePressedPosNew.x()-5,mousePressedPosNew.y()-5, 10, 10);
        startItem->setBrush((QBrush(QColor(255, 0, 0))));
        scene->addItem(startItem);
        firstTimeFlag = false;

        // add in the QList of vertices
        vertices.append(e->pos());


    }else{
        //draw an ellipse at the beginning of the line &
        QGraphicsEllipseItem *startItem = new QGraphicsEllipseItem(mousePressedPosNew.x()-5,mousePressedPosNew.y()-5, 10, 10);
        startItem->setBrush((QBrush(QColor(255, 0, 0))));
        scene->addItem(startItem);


        // set midPointX and midPointXs
        midPoint = QPoint( (mousePressedPosOld.x() + mousePressedPosNew.x())/2,  (mousePressedPosOld.y() + mousePressedPosNew.y())/2);
        midPoint3.setX(midPoint.x());
        midPoint3.setY(midPoint.y());
        midPoint1 = QPoint( midPoint.x(),midPoint.y() - 20 );
        midPoint2 = QPoint( midPoint.x(),midPoint.y() + 20 );
        // add in the QList of midpoints
        midPoints1.append(midPoint1);
        midPoints2.append(midPoint2);
        midPoints3.append(midPoint3);
        //allMidPoints
        for(int i = 0; i < MAX_LANE_NUM; i++){
            /////////////////////////////////////////////////////////
            // vertical distance       i
            //        -20              0
            //         20              1
            //          0              2
            if(i == 2) {tempMidPointList.append(midPoint3); continue;}
            if(i == 0) {tempMidPointList.append(midPoint1); continue;}
            if(i == 1) {tempMidPointList.append(midPoint2);continue;}
            //        -30              3
            //         30              4
            //        -40              5
            //         40              6
            //        -50              7
            //         50              8
            if(i % 2 == 1){
                tempMidPointList.append(QPoint( midPoint.x(),midPoint.y() - (30 + 5 * (i-3))));
            }else{
                tempMidPointList.append(QPoint( midPoint.x(),midPoint.y() + (30 + 5 * (i-4))));
            }
        }

        // can reach them in the future
        allMidPoints.append(tempMidPointList);

        // add in the QList of vertices
        vertices.append(e->pos());


        // draw lanes
        if(createLaneNum >= 2){
            QGraphicsLineItem *path11 = new QGraphicsLineItem( mousePressedPosOld.x(),mousePressedPosOld.y(),midPoint.x(),midPoint.y() - 20 );
            scene->addItem(path11);
            QGraphicsLineItem *path12 = new QGraphicsLineItem( midPoint.x(),midPoint.y() - 20, mousePressedPosNew.x(), mousePressedPosNew.y() );
            scene->addItem(path12);

            QGraphicsLineItem *path21 = new QGraphicsLineItem( mousePressedPosOld.x(),mousePressedPosOld.y(),midPoint.x(),midPoint.y() + 20 );
            scene->addItem(path21);
            QGraphicsLineItem *path22 = new QGraphicsLineItem( midPoint.x(),midPoint.y() + 20, mousePressedPosNew.x(), mousePressedPosNew.y() );
            scene->addItem(path22);
        }
        if(createLaneNum >= 3){
            QGraphicsLineItem *path3 = new QGraphicsLineItem( mousePressedPosOld.x(), mousePressedPosOld.y(), mousePressedPosNew.x(), mousePressedPosNew.y() );
            scene->addItem(path3);
        }
        if(createLaneNum >= 4){
            // createLaneNum is not 2 or 3
            for(int i = 4; i <= createLaneNum; i++){
            QGraphicsLineItem *tpath11 = new QGraphicsLineItem( mousePressedPosOld.x(),mousePressedPosOld.y(),tempMidPointList.at(i-1).x(), tempMidPointList.at(i-1).y() );
            scene->addItem(tpath11);
            QGraphicsLineItem *tpath12 = new QGraphicsLineItem( tempMidPointList.at(i-1).x(), tempMidPointList.at(i-1).y(), mousePressedPosNew.x(), mousePressedPosNew.y() );
            scene->addItem(tpath12);
            }
        }


        laneNums.append(createLaneNum);

    }

    mousePressedPosOld =e->pos();
    mousePressedNum ++;
    // clear tempMids
    tempMidPointList.clear();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e){
    ////draw a line according to mouse pressed and released position
    //QGraphicsLineItem *item=new QGraphicsLineItem(mousePressedPosOld.x(),mousePressedPosOld.y(),e->pos().x(),e->pos().y());//old x, old y, new x, new y
    //scene->removeItem(tempLine);
    //tempAdded=false;
    //scene->addItem(item);

    ////here we construct a "point" and schedule its update
    //QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-5,-5,10,10);
    //eitem->setBrush(QBrush(QColor(0,0,0)));
    //items.append(eitem);
    //informations.append(PointDoglegMoveInformation(mousePressedPosOld,e->pos(),2000,currentTime,2200));

}

void MainWindow::mouseMoveEvent(QMouseEvent *e){
    //tempLine->setLine(mousePressedPosOld.x(),mousePressedPosOld.y(),e->pos().x(),e->pos().y());
    //if(!tempAdded){
    //    scene->addItem(tempLine);
    //    tempAdded=true;
    //}
}

void MainWindow::on_pushButton_clicked()
{
    // other vehicles choosing lanes
    //int vehicleNum = otherVehicleNum;
    //int thislaneNum = laneNums.at(playRound%mousePressedNum);
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));

    int i = 1; i--;
    int t_start = 2000+ vehicleNumOnEachLane.at(i)*400;
    int t_stop = t_start + 200;

    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 1
    if(playRound != 0 && playRound%(mousePressedNum)== (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints1.at(playRound%mousePressedNum),vertices.at((mousePressedNum)),t_start,currentTime,t_stop)); // startTime, moveTime, endTime
    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints1.at(playRound%mousePressedNum),vertices.at((playRound+1)%mousePressedNum),t_start,currentTime,t_stop)); // startTime, moveTime, endTime
    }
    //informations.append(PointDoglegMoveInformation(midPointsUp.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}

void MainWindow::on_pushButton_2_clicked()
{
    // other vehicles choosing lanes
    //int vehicleNum = otherVehicleNum;
    //int thislaneNum = laneNums.at(playRound%mousePressedNum);
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));

    int i = 2; i--;
    int t_start = 2000+ vehicleNumOnEachLane.at(i)*400;
    int t_stop = t_start + 200;


    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 2
    if(playRound != 0 && playRound%(mousePressedNum) == (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints2.at(playRound%mousePressedNum),vertices.at((mousePressedNum)), t_start,currentTime,t_stop));

    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints2.at(playRound%mousePressedNum),vertices.at((playRound+1)%mousePressedNum), t_start,currentTime,t_stop));

    }
    //informations.append(PointDoglegMoveInformation(midPointsDown.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}

void MainWindow::on_pushButton_3_clicked()
{
    // other vehicles choosing lanes
    //int vehicleNum = otherVehicleNum;
    //int thislaneNum = laneNums.at(playRound%mousePressedNum);
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));

    int i = 3; i--;
    int t_start = 2000+ vehicleNumOnEachLane.at(i)*400;
    int t_stop = t_start + 200;


    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 3 (a straight line)
    if(playRound != 0 && playRound%(mousePressedNum) == (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints3.at(playRound%mousePressedNum),vertices.at((mousePressedNum)), t_start,currentTime,t_stop));

    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints3.at(playRound%mousePressedNum),vertices.at((playRound+1)%mousePressedNum), t_start,currentTime,t_stop));

    }
    //informations.append(PointDoglegMoveInformation(midPointsDown.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}

void MainWindow::on_pushButton_4_clicked()
{
    // other vehicles choosing lanes
    //int vehicleNum = otherVehicleNum;
    //int thislaneNum = laneNums.at(playRound%mousePressedNum);
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));

    int i = 4; i--;
    int t_start = 2000+ vehicleNumOnEachLane.at(i)*400;
    int t_stop = t_start + 200;

    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 4
    if(playRound != 0 && playRound%(mousePressedNum) == (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(4-1),vertices.at((mousePressedNum)), t_start,currentTime,t_stop));

    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(4-1),vertices.at((playRound+1)%mousePressedNum), t_start,currentTime,t_stop));

    }
    //informations.append(PointDoglegMoveInformation(midPointsDown.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}

void MainWindow::on_pushButton_5_clicked()
{
    // other vehicles choosing lanes
    //int vehicleNum = otherVehicleNum;
    //int thislaneNum = laneNums.at(playRound%mousePressedNum);
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));

    int i = 5; i--;
    int t_start = 2000+ vehicleNumOnEachLane.at(i)*400;
    int t_stop = t_start + 200;

    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 5
    if(playRound != 0 && playRound%(mousePressedNum) == (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(5-1),vertices.at((mousePressedNum)), t_start,currentTime,t_stop));

    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(5-1),vertices.at((playRound+1)%mousePressedNum), t_start,currentTime,t_stop));

    }
    //informations.append(PointDoglegMoveInformation(midPointsDown.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}

void MainWindow::on_pushButton_6_clicked()
{
    // other vehicles choosing lanes
    //int vehicleNum = otherVehicleNum;
    //int thislaneNum = laneNums.at(playRound%mousePressedNum);
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));

    int i = 6; i--;
    int t_start = 2000+ vehicleNumOnEachLane.at(i)*400;
    int t_stop = t_start + 200;

    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 6
    if(playRound != 0 && playRound%(mousePressedNum) == (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(6-1),vertices.at((mousePressedNum)), t_start,currentTime,t_stop));

    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(6-1),vertices.at((playRound+1)%mousePressedNum), t_start,currentTime,t_stop));

    }
    //informations.append(PointDoglegMoveInformation(midPointsDown.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}

void MainWindow::on_pushButton_7_clicked()
{
    // other vehicles choosing lanes
    //int vehicleNum = otherVehicleNum;
    //int thislaneNum = laneNums.at(playRound%mousePressedNum);
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));

    int i = 7; i--;
    int t_start = 2000+ vehicleNumOnEachLane.at(i)*400;
    int t_stop = t_start + 200;

    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 7
    if(playRound != 0 && playRound%(mousePressedNum) == (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(7-1),vertices.at((mousePressedNum)), t_start,currentTime,t_stop));

    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(7-1),vertices.at((playRound+1)%mousePressedNum), t_start,currentTime,t_stop));

    }
    //informations.append(PointDoglegMoveInformation(midPointsDown.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}

void MainWindow::on_pushButton_8_clicked()
{
    // other vehicles choosing lanes
    //int vehicleNum = otherVehicleNum;
    //int thislaneNum = laneNums.at(playRound%mousePressedNum);
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));

    int i = 8; i--;
    int t_start = 2000+ vehicleNumOnEachLane.at(i)*400;
    int t_stop = t_start + 200;

    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 8
    if(playRound != 0 && playRound%(mousePressedNum) == (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(8-1),vertices.at((mousePressedNum)), t_start,currentTime,t_stop));

    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(8-1),vertices.at((playRound+1)%mousePressedNum), t_start,currentTime,t_stop));

    }
    //informations.append(PointDoglegMoveInformation(midPointsDown.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}

void MainWindow::on_pushButton_9_clicked()
{
    // other vehicles choosing lanes
    //int vehicleNum = otherVehicleNum;
    //int thislaneNum = laneNums.at(playRound%mousePressedNum);
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));

    int i = 9; i--;
    int t_start = 2000+ vehicleNumOnEachLane.at(i)*400;
    int t_stop = t_start + 200;

    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 9
    if(playRound != 0 && playRound%(mousePressedNum) == (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(9-1),vertices.at((mousePressedNum)), t_start,currentTime,t_stop));

    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(9-1),vertices.at((playRound+1)%mousePressedNum), t_start,currentTime,t_stop));

    }

    //informations.append(PointDoglegMoveInformation(midPointsDown.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lcdNumber_lane->display(value);
    createLaneNum = value;
}

void MainWindow::otherVehicleSetUp(int vehicleNum, int thislaneNum){

    int ttemp;
    for(int i = 0; i < vehicleNum; i++){

        int lanelane = (thislaneNum+i)%thislaneNum;// can be adjusted, basically from 1-MAX_LANE_NUM
        if(lanelane == 0) lanelane = 3;

        vehicleLanes.append(lanelane);
        ttemp = vehicleNumOnEachLane.at(lanelane);
        vehicleNumOnEachLane.replace(lanelane, ttemp+1);
        int t_start = 2000 + vehicleNumOnEachLane.at(vehicleLanes.at(i))*200;
        int t_stop = t_start + 200;
        //create item and info for them
        QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
        eitem->setBrush(QBrush(QColor(0,255,0)));
        items.append(eitem);
        // Draw one other vehicle goes from its Lane
        if(playRound != 0 && playRound%(mousePressedNum) == (mousePressedNum-1)){
            dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(vehicleLanes.at(i)-1),vertices.at((mousePressedNum)), t_start+100*i,currentTime,t_stop+100*i));

        }else{
            dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(vehicleLanes.at(i)-1),vertices.at((playRound+1)%mousePressedNum), t_start+100*i,currentTime,t_stop+100*i));

        }
    }
    vehicleLanes.clear();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    otherVehicleNum = arg1;
}

void MainWindow::on_pushButton_10_clicked()
{
    for(int i = 0; i < MAX_LANE_NUM; i++)
    vehicleNumOnEachLane.replace(i, 0);
}
