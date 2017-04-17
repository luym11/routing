#include "mainwindow.h"
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

    createLaneNum = 9;
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));

    otherVehicleNum = 23;
    connect(ui->spinBox_other, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox_other_valueChanged(int)));
    // connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox_valueChanged(int))); // ACTUALLY NO NEED

    setUpFlag = false;


}

void MainWindow::setUp(){ // do this after setting createLaneNum
    for(int i = 0; i <= createLaneNum; i++){
        vehicleNumOnEachLane.append(0);
    }
    setUpFlag = true;
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
    if(setUpFlag == true){
        if(createLaneNum >= 2){
            ui->lcdNumber->display(vehicleNumOnEachLane.at(1));
            ui->lcdNumber_2->display(vehicleNumOnEachLane.at(2));
        }
        if(createLaneNum >= 3)
            ui->lcdNumber_3->display(vehicleNumOnEachLane.at(3));
        if(createLaneNum >= 4)
            ui->lcdNumber_4->display(vehicleNumOnEachLane.at(4));
        if(createLaneNum >= 5)
            ui->lcdNumber_5->display(vehicleNumOnEachLane.at(5));
        if(createLaneNum >= 6)
            ui->lcdNumber_6->display(vehicleNumOnEachLane.at(6));
        if(createLaneNum >= 7)
            ui->lcdNumber_7->display(vehicleNumOnEachLane.at(7));
        if(createLaneNum >= 8)
            ui->lcdNumber_8->display(vehicleNumOnEachLane.at(8));
        if(createLaneNum >= 9)
            ui->lcdNumber_9->display(vehicleNumOnEachLane.at(9));
    }
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
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));
    int clickedLane = 1;
    myVehicleSetUp(clickedLane);
    playRound++;
}

void MainWindow::on_pushButton_2_clicked()
{
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));
    int clickedLane = 2;
    myVehicleSetUp(clickedLane);
    playRound++;
}

void MainWindow::on_pushButton_3_clicked()
{
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));
    int clickedLane = 3;
    myVehicleSetUp(clickedLane);
    playRound++;
}

void MainWindow::on_pushButton_4_clicked()
{
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));
    int clickedLane = 4;
    myVehicleSetUp(clickedLane);
    playRound++;
}

void MainWindow::on_pushButton_5_clicked()
{
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));
    int clickedLane = 5;
    myVehicleSetUp(clickedLane);
    playRound++;
}

void MainWindow::on_pushButton_6_clicked()
{
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));
    int clickedLane = 6;
    myVehicleSetUp(clickedLane);
    playRound++;
}

void MainWindow::on_pushButton_7_clicked()
{
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));
    int clickedLane = 7;
    myVehicleSetUp(clickedLane);
    playRound++;
}

void MainWindow::on_pushButton_8_clicked()
{

    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));
    int clickedLane = 8;
    myVehicleSetUp(clickedLane);
    playRound++;
}

void MainWindow::on_pushButton_9_clicked()
{
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));
    int clickedLane = 9;
    myVehicleSetUp(clickedLane);
    playRound++;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lcdNumber_lane->display(value);
    createLaneNum = value;
}

void MainWindow::otherVehicleSetUp(int vehicleNum, int laneNumForThisRound){
    if(playRound == 0){
        // Randomly assign "laneNumForThisRound" lanes for "vehicleNum" vehicles
        laneAssigned = randomlyAssignLaneFunc(vehicleNum, laneNumForThisRound);
    }else{
        // Each vehicle chooses a lane based on previous results, might be unable to get our from while() if laneNum on each playRound are different
        laneAssigned = lastResultBasedAssignLaneFunc(vehicleNum, laneNumForThisRound, vehicleNumOnEachLaneLastRound);
    }

    int lanelane;
    for(int i = 0; i < vehicleNum; i++){

        lanelane = laneAssigned.at(i);

        vehicleLanes.append(lanelane);
        vehicleNumOnEachLane.replace(lanelane, vehicleNumOnEachLane.at(lanelane) + 1);
        int t_move = 5000 + vehicleNumOnEachLane.at(vehicleLanes.at(i))*400;
        int t_life = t_move + 200;


        //create item and info for them
        QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
        eitem->setBrush(QBrush(QColor(0,255,0)));
        items.append(eitem);
        // Draw one other vehicle goes from its Lane
        if(playRound%mousePressedNum == (mousePressedNum-1)){
            dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(vehicleLanes.at(i)-1),vertices.at((mousePressedNum)), t_move/*+100*i*/,currentTime,t_life/*+100*i*/));

        }else{
            dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(vehicleLanes.at(i)-1),vertices.at((playRound+1)%mousePressedNum), t_move/*+100*i*/,currentTime,t_life/*+100*i*/));

        }
    }
    vehicleLanes.clear();
}

void MainWindow::myVehicleSetUp(int clickedLane){

    vehicleNumOnEachLane.replace(clickedLane, vehicleNumOnEachLane.at(clickedLane)+1);
    vehicleNumOnEachLaneLastRound = vehicleNumOnEachLane;
    int t_move = 5000+ vehicleNumOnEachLane.at(clickedLane)*400;
    int t_life = t_move + 200;

    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-2,-2,4,4);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    // Draw one vehicle goes from lane clickedLane
    if(playRound%mousePressedNum== (mousePressedNum-1)){
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(clickedLane-1),vertices.at((mousePressedNum)),t_move,currentTime,t_life)); // startTime, moveTime, endTime
    }else{
        dogleginformations.append(PointDoglegMoveInformation(vertices.at(playRound%mousePressedNum),allMidPoints.at(playRound%mousePressedNum).at(clickedLane-1),vertices.at((playRound+1)%mousePressedNum),t_move,currentTime,t_life)); // startTime, moveTime, endTime
    }
}

// stupid function! just assign vehicles to lanes using rand()%Num works
QList<int> MainWindow::randomlyAssignLaneFunc(int vehicleNum, int laneNumOfThisRound){
    int summ = 0;
    int summ2 = 0;
    QList<int> randomVehicleNumDistribution;
    QList<int> randomLaneAssignment;

    for(int i = 0; i < laneNumOfThisRound; i++){
        randomVehicleNumDistribution.append(rand() % vehicleNum + 1);
        summ = summ + randomVehicleNumDistribution.at(i);
    }
    for(int i = 0; i < (laneNumOfThisRound - 1); i++){
        randomVehicleNumDistribution.replace(i, qFloor( vehicleNum * randomVehicleNumDistribution.at(i)/summ + 1) - 1  );
        summ2 = summ2 + randomVehicleNumDistribution.at(i);
    }
    randomVehicleNumDistribution.replace(laneNumOfThisRound - 1, vehicleNum - summ2);

    for(int i = 0; i<laneNumOfThisRound; i++){
            for(int j = 0; j < randomVehicleNumDistribution.at(i); j++){
                 randomLaneAssignment.append(i+1);
            }
    }
    return randomLaneAssignment;
}

QList<int> MainWindow::findMinValueIndices(QList<int> myVehicleNumOnEachLane){
    int minNum;
    QList<int> indicesOfMinValues;
    minNum = myVehicleNumOnEachLane.at(1);
    for(int i = 1; i < myVehicleNumOnEachLane.length(); i++){
        if(minNum >= myVehicleNumOnEachLane.at(i)){
            minNum = myVehicleNumOnEachLane.at(i);
        }
    }
    for(int i = 1; i < myVehicleNumOnEachLane.length(); i++){
        if(minNum == myVehicleNumOnEachLane.at(i)){
            indicesOfMinValues.append(i);
        }
    }
    return indicesOfMinValues;
}

QList<int> MainWindow::lastResultBasedAssignLaneFunc(int vehicleNum, int laneNumForThisRound, QList<int> myVehicleNumOnEachLaneLastRound){ //last 2 params should be consistant
    QList<int> lastResultBasedAssignedLane;
    minValueIndices = findMinValueIndices(myVehicleNumOnEachLaneLastRound);
    int r;
    int ii;
    int compareRound;
    for(int i = 0; i < vehicleNum; i++){
        r = rand()%10;
        if( r == 0 || r == 1 || r == 2 || r == 3){ // 40% for others, 60% for lanes previously have min #vehicles
            ii = rand()%laneNumForThisRound + 1;
            compareRound = 0;
            while(compareRound < minValueIndices.length()){
                if(ii == minValueIndices.at(compareRound)){
                    compareRound = 0;
                    ii = rand()%laneNumForThisRound + 1;
                }else{
                    compareRound++;
                }
            }
            lastResultBasedAssignedLane.append(ii);
        }else{
            ii = rand()%laneNumForThisRound + 1;
            compareRound = 0;
            while(1){
                if(ii == minValueIndices.at(compareRound)){
                    break;
                }
                compareRound =( compareRound+1 )% minValueIndices.length();
                if(compareRound == 0 ){
                    ii = rand()%laneNumForThisRound + 1;
                }
                cout << "createLaneNum = " << createLaneNum << endl;
                cout << "ii = " << ii << ", minValueIndices.at(compareRound) = " << minValueIndices.at(compareRound) << endl;
            }
            lastResultBasedAssignedLane.append(ii);
        }
    }

    return lastResultBasedAssignedLane;
}



void MainWindow::on_spinBox_other_valueChanged(int arg1)
{
    otherVehicleNum = arg1;
}

void MainWindow::on_pushButton_10_clicked()
{
    for(int i = 1; i <= createLaneNum; i++){
        vehicleNumOnEachLane.replace(i, 0);
        //cout << "vehicleNumOnEachLane.length() = " << vehicleNumOnEachLane.length() << endl;
    }
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    myVehicleNumOnLane1 = arg1;
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    myVehicleNumOnLane2 = arg1;
}

void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    myVehicleNumOnLane3 = arg1;
}

void MainWindow::on_spinBox_4_valueChanged(int arg1)
{
    myVehicleNumOnLane4 = arg1;
}

void MainWindow::on_spinBox_5_valueChanged(int arg1)
{
    myVehicleNumOnLane5 = arg1;
}

void MainWindow::on_spinBox_6_valueChanged(int arg1)
{
    myVehicleNumOnLane6 = arg1;
}

void MainWindow::on_spinBox_7_valueChanged(int arg1)
{
    myVehicleNumOnLane7 = arg1;
}

void MainWindow::on_spinBox_8_valueChanged(int arg1)
{
    myVehicleNumOnLane8 = arg1;
}

void MainWindow::on_spinBox_9_valueChanged(int arg1)
{
    myVehicleNumOnLane9 = arg1;
}

void MainWindow::on_pushButton_run_clicked()
{
    otherVehicleSetUp(otherVehicleNum, laneNums.at(playRound%mousePressedNum));
    //QList<int> myVehicleNumOnEachLane;
    //myVehicleNumOnEachLane
    int clickedLane = 9;
    myVehicleSetUp(clickedLane);
    playRound++;
}

void MainWindow::on_pushButton_11_clicked()
{
    setUp();
}
