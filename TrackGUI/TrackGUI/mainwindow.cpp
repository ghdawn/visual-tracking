#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <cstdio>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);
    setWindowTitle(tr("Capture"));
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    QImage Mimg;
   // while(1)
   {
       if(core->NewPostImg==true)
       {
           printf("Begin Display!\n");
           mutexPost->lock();
           Mimg=QImage(core->postImg, 320, 240, QImage::Format_RGB32);
           paint.drawImage(QPoint(0, 0), Mimg);
           paint.end();
           mutexPost->unlock();
           core->NewPostImg=false;
           printf("End Display!\n");
       }
   }
}

MainWindow::~MainWindow()
{
    delete ui;
}
