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
    void triggerUpdate(QList<QGraphicsEllipseItem*> &items,QList<PointMoveInformation> &informations,int interval);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QPoint mousePressedPos;
    QGraphicsLineItem *tempLine;
    bool tempAdded;

    Updater *updater;
    QList<QGraphicsEllipseItem *> items;
    QList<PointMoveInformation> informations;
    int currentTime;

    QTimer *timer;
};

#endif // MAINWINDOW_H
