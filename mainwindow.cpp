#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


//remember to include <QDebug> in cpp files. printf() will not flush instantly in Qt GUI applications..
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //init a new scene and bind it to a view
    scene=new QGraphicsScene(this);
    QRect rect=ui->graphicsView->rect();
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
    mousePressedPos =e->pos();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e){
    //draw a line according to mouse pressed and released position
    QGraphicsLineItem *item=new QGraphicsLineItem(mousePressedPos.x(),mousePressedPos.y(),e->pos().x(),e->pos().y());
    scene->removeItem(tempLine);
    tempAdded=false;
    scene->addItem(item);

    //here we construct a "point" and schedule its update
    QGraphicsEllipseItem *eitem=new QGraphicsEllipseItem(-5,-5,10,10);
    eitem->setBrush(QBrush(QColor(0,0,0)));
    items.append(eitem);
    informations.append(PointMoveInformation(mousePressedPos,e->pos(),2000,currentTime,2200));
}

void MainWindow::mouseMoveEvent(QMouseEvent *e){
    tempLine->setLine(mousePressedPos.x(),mousePressedPos.y(),e->pos().x(),e->pos().y());
    if(!tempAdded){
        scene->addItem(tempLine);
        tempAdded=true;
    }
}
