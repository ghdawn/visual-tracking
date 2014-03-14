#include <QtGui>
#include <cstdio>
#include <QPainter>
#include "processimage.h"
using namespace itr_tracker;
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
void ProcessImage::Process(U8* inputimg,RectangleS rect,string Info,U8* outputimg)
{
    int i;
    QPainter paint(this);
    int ViewX = 10;
    int ViewY = 10;
    QImage image = QImage(inputimg, width, height, QImage::Format_RGB32);
    QPixmap pix(width,height);
    QPainter p(&pix);
    p.drawImage(QPoint(0, 0), image);
    p.drawRect(rect.X,rect.Y,rect.Width,rect.Height);
    p.drawText(QPoint(ViewX, ViewY), tr("ProcessImage Succeed!"));
    paint.end();
    pix.save("lm.png");
    QImage img = QImage("lm.png");
    outputimg = img.bits();
     unsigned char * p_bits=img.bits();
     for(i=0; i < width*height; i++)
     {
         outputimg[i] = p_bits[i];
     }
}
