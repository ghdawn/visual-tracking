#include <QtGui>
#include <cstdio>
#include <QPainter>
#include "processimage.h"

ProcessImage::ProcessImage(QWidget *parent):QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(200000);//定时器，每隔0.2秒刷新
    setWindowTitle(tr("Capture"));

}

ProcessImage::~ProcessImage()
{

}

void ProcessImage::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    int i;
    int width;
    int height;
    int ViewX;
    int ViewY;
    width = 320;
    height =240;
    ViewX = 100;
    ViewY = 20;
    unsigned char *pSrc=new unsigned char[width*height*4];
    for(i=0; i<width*height*4; i++)
    {
        pSrc[i] = 128;
    }
    /* pixImage.load(tr("./img/%1.jpg").arg(i));//当前文件夹中有个img文件夹，期中有19张图片，名为i.jpg
     //pixImage.loadFromData((uchar *)buf,sizeof(buf),"JPEG",QPixmap::Color);
     QPainter painter(this);
     painter.drawPixmap(0,0,QWidget::width(),QWidget::height(),pixImage);
     if(i==3)
     {
         i=1;
     }
     else
     {
         i++;
     }*/

    QImage image = QImage(pSrc, width, height, QImage::Format_RGB32);
    QPixmap pix(width,height);
    QPainter p(&pix);
    p.drawImage(QPoint(0, 0), image);
    p.drawText(QPoint(ViewX, ViewY), tr("ProcessImage Succeed!"));
    p.end();
    pix.save("a.png");
    QImage img = QImage("a.png");
    unsigned char * p_bits=img.bits();
    for(i=0; i < width*height*3; i++)
    {
        pSrc[i] = p_bits[i];
    }
    int j;
}
