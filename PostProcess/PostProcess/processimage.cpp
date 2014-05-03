#include <QtGui>
#include <stdio.h>
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
void ProcessImage::Process(U8* inputimg,RectangleS rect,std::vector<string> InfoList,U8* outputimg)
{
    int i;
    static int pic=0;
    int ViewX = 10;
    int ViewY = 10;
    char filename[50];
    QImage imagex = QImage(inputimg, width, height, QImage::Format_RGB32);

    QPainter p;
    p.begin(&imagex);
    p.setBrush(Qt::NoBrush);
    p.setPen(Qt::green);
    p.drawRect(rect.X,rect.Y,rect.Width,rect.Height);
    p.end();
    QImage image = imagex.mirrored(false, false);
    p.begin(&image);
    p.setBrush(Qt::NoBrush);
    p.setPen(Qt::green);
    for(i=0;i<InfoList.size();i++)
    {         
         if(i==2)
         {
             ViewY=10;
             ViewX=width-InfoList[i].length()*9;
             p.drawText(QPoint(ViewX, ViewY), InfoList[i].c_str());
         }
         else
         {
             p.drawText(QPoint(ViewX, ViewY), InfoList[i].c_str());
         }
         ViewY = ViewY+20;
    }
  // p.drawText(QPoint(ViewX, ViewY), Info.c_str());
    p.end();
    sprintf(filename,"bin/Debug/output/%05d.png",pic++);
    image.save(filename);
    unsigned char * p_bits=image.bits();
    for(i=0; i < width*height*4; i++)
    {
        outputimg[i] = p_bits[i];
    }
}
