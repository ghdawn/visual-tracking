#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <cstdio>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int i;
    int width = 320;
    int height =240;
    ui->setupUi(this);
    unsigned char *pSrc=new unsigned char[width*height*4];
    for(i=0; i<width*height*3; i++)
    {
        pSrc[i] = i%255;
    }
    QPainter painter;
    QImage image = QImage(pSrc, width, height, QImage::Format_RGB32);
    painter.drawImage(QPoint(0, 0), image);
}

MainWindow::~MainWindow()
{
    delete ui;
}
