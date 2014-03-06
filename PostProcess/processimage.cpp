#include <QtGui>
#include <cstdio>
#include <QPainter>
#include "processimage.h"

ProcessImage::ProcessImage(QWidget *parent):QWidget(parent)
{

}

ProcessImage::~ProcessImage()
{

}
void ProcessImage::Init(int Width,int Height)
{
    this->width = width;
    this->height = height;
}
void ProcessImage::process(U8* inputimg,RectangleF rect,string Info,U8* outputimg)
{
    QPainter paint(this);
    int i;
    int ViewX = 10;
    int ViewY = 10;
    unsigned char *pSrc=new unsigned char[width*height];
    for(i=0; i<width*height; i++)
    {
        pSrc[i] = 255;
    }
    QImage image = QImage(pSrc, width, height, QImage::Format_RGB32);
    QPixmap pix(width,height);
    QPainter p(&pix);
    p.drawImage(QPoint(0, 0), image);
    p.drawRect(rect.X,rect.Y,rect.Width,rect.Height);
    p.drawText(QPoint(ViewX, ViewY), tr("ProcessImage Succeed!"));
    paint.end();
    pix.save("lm.png");
    QImage img = QImage("lm.png");
    unsigned char * p_bits=img.bits();
    for(i=0; i < width*height; i++)
    {
        pSrc[i] = p_bits[i];
    }

}
