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

}

void MainWindow::paintEvent(QPaintEvent *)
{
    int i;
    int width = 320;
    int height =240;
    unsigned char *pSrc=new unsigned char[width*height*4];
    for(i=0; i<width*height; i++)
    {
        pSrc[i] = i;
    }
    QPainter painter(this);
    QImage image = QImage(pSrc, width, height, QImage::Format_RGB32);
    painter.drawImage(QPoint(0, 0), image);
    painter.end();
}

MainWindow::~MainWindow()
{
    delete ui;
}
