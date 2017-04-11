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
    connect(this,SIGNAL(triggerUpdate(QList<QGraphicsEllipseItem*>&,QList<PointMoveInformation>&,int)),updater,SLOT(update(QList<QGraphicsEllipseItem*>&,QList<PointMoveInformation>&,int)));
    timer->start(10);

//    Round = 0;
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
    emit triggerUpdate(items,informations,timer->interval());
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

//        // add in the QList of vertices
//        vertices.append(e->pos());
    }else{
        //draw an ellipse at the beginning of the line &
        QGraphicsEllipseItem *startItem = new QGraphicsEllipseItem(mousePressedPosNew.x()-5,mousePressedPosNew.y()-5, 10, 10);
        startItem->setBrush((QBrush(QColor(255, 0, 0))));
        scene->addItem(startItem);
        //draw two lanes from mousePressedPosOld to mousePressedPosNew
        midPoint = QPoint( (mousePressedPosOld.x() + mousePressedPosNew.x())/2,  (mousePressedPosOld.y() + mousePressedPosNew.y())/2);
//        midPointUp = QPoint( midPoint.x(),midPoint.y() + 20 );
//        midPointDown = QPoint( midPoint.x(),midPoint.y() - 20 );
        QGraphicsLineItem *path11 = new QGraphicsLineItem( mousePressedPosOld.x(),mousePressedPosOld.y(),midPoint.x(),midPoint.y() + 20 );
        scene->addItem(path11);
        QGraphicsLineItem *path12 = new QGraphicsLineItem( mousePressedPosOld.x(),mousePressedPosOld.y(),midPoint.x(),midPoint.y() - 20 );
        scene->addItem(path12);
        QGraphicsLineItem *path21 = new QGraphicsLineItem( midPoint.x(),midPoint.y() + 20, mousePressedPosNew.x(), mousePressedPosNew.y() );
        scene->addItem(path21);
        QGraphicsLineItem *path22 = new QGraphicsLineItem( midPoint.x(),midPoint.y() - 20, mousePressedPosNew.x(), mousePressedPosNew.y() );
        scene->addItem(path22);

//        // add in the QList of vertices
//        vertices.append(e->pos());
//        // add in the QList of midpoints
//        midPointsUp.append(midPointUp);
//        midPointsDown.append(midPointDown);
    }

    mousePressedPosOld =e->pos();
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
    //informations.append(PointMoveInformation(mousePressedPosOld,e->pos(),2000,currentTime,2200));

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
    // Draw one vehicle goes from Lane 1
//    informations.append(PointMoveInformation(vertices.at(Round),midPointsUp.at(Round),2000,currentTime,2200));
//    informations.append(PointMoveInformation(midPointsUp.at(Round),vertices.at(Round+1), 2000,currentTime,2200));
//    Round++;
}

void MainWindow::on_pushButton_2_clicked()
{
    // Draw one vehicle goes from Lane 2
//    informations.append(PointMoveInformation(vertices.at(Round),midPointsDown.at(Round),2000,currentTime,2200));
//    informations.append(PointMoveInformation(midPointsDown.at(Round),vertices.at(Round+1), 2000,currentTime,2200));
//    Round++;
}
