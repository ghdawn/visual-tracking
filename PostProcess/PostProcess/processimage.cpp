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
    this->width = Width;
    this->height = Height;
}
void ProcessImage::Process(U8* inputimg,RectangleS rect,string Info,U8* outputimg)
{
    int i;
    int ViewX = 10;
    int ViewY = 10;
    QImage image = QImage(inputimg, width, height, QImage::Format_RGB32);
    QPainter p;
    p.begin(&image);
    p.setBrush(Qt::NoBrush);
    p.setPen(Qt::red);
    p.drawRect(rect.X,rect.Y,rect.Width,rect.Height);
    p.drawText(QPoint(ViewX, ViewY), Info.c_str());
    p.end();
    image.save("lm2.png");
    unsigned char * p_bits=image.bits();
    for(i=0; i < width*height; i++)
    {
        outputimg[i] = p_bits[i];
    }
}
