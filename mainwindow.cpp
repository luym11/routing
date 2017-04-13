#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#define MAX_LANE_NUM 9

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

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    createLaneNum = 2;
    laneNums.append(createLaneNum);
//    setSliderAndSpinBox();
//    QHBoxLayout *layout = new QHBoxLayout;
//    layout->addWidget(pSpinBox);
//    layout->addWidget(pSlider);
//    window.setLayout(layout);


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



    }

    mousePressedPosOld =e->pos();
    mousePressedNum ++;
    laneNums.append(createLaneNum);
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
    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 1
    if(playRound != 0 && playRound%(mousePressedNum)== (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints1.at(playRound%mousePressedNum),vertices.at((mousePressedNum)),2000,currentTime,2200)); // startTime, moveTime, endTime
    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints1.at(playRound%mousePressedNum),vertices.at((playRound+1)%mousePressedNum),2000,currentTime,2200)); // startTime, moveTime, endTime
    }
    //informations.append(PointDoglegMoveInformation(midPointsUp.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}

void MainWindow::on_pushButton_2_clicked()
{
    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 2
    if(playRound != 0 && playRound%(mousePressedNum) == (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints2.at(playRound%mousePressedNum),vertices.at((mousePressedNum)), 2000,currentTime,2200));

    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints2.at(playRound%mousePressedNum),vertices.at((playRound+1)%mousePressedNum), 2000,currentTime,2200));

    }
    //informations.append(PointDoglegMoveInformation(midPointsDown.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}

void MainWindow::on_pushButton_3_clicked()
{
    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from Lane 3 (a straight line)
    if(playRound != 0 && playRound%(mousePressedNum) == (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints3.at(playRound%mousePressedNum),vertices.at((mousePressedNum)), 2000,currentTime,2200));

    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),midPoints3.at(playRound%mousePressedNum),vertices.at((playRound+1)%mousePressedNum), 2000,currentTime,2200));

    }
    //informations.append(PointDoglegMoveInformation(midPointsDown.at(playRound),vertices.at(playRound+1), 2000,currentTime,2200));
    playRound++;
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lcdNumber->display(value);
    createLaneNum = value;
}


