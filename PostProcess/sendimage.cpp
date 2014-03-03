#include <QtGui>
#include <cstdio>
#include <QPainter>
#include "sendimage.h"

SendImage::SendImage(QWidget *parent):QWidget(parent)
{
    setWindowTitle(tr("Capture"));

}

SendImage::~SendImage()
{

}

void SendImage::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    int i;
    int width;
    int height;
    int ViewX;
    int ViewY;
    width = 320;
    height =240;
    ViewX = 10;
    ViewY = 20;
    unsigned char *pSrc=new unsigned char[width*height*4];
    for(i=0; i<width*height*4; i++)
    {
        pSrc[i] = 255;
    }
    QImage image = QImage(pSrc, width, height, QImage::Format_RGB32);
    QPixmap pix(width,height);
    QPainter p(&pix);
    p.drawImage(QPoint(0, 0), image);
    p.drawText(QPoint(ViewX, ViewY), tr("ProcessImage Succeed!"));
    paint.end();
    pix.save("lm.png");
    QImage img = QImage("lm.png");
    unsigned char * p_bits=img.bits();
    for(i=0; i < width*height*3; i++)
    {
        pSrc[i] = p_bits[i];
    }

}
