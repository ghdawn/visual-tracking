#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H
#include <string>
#include <QWidget>
#include <QtGui>
#include "itrbase.h"
using itr_math::RectangleS;
using std::string;
class ProcessImage:public QWidget
{
public:
   ProcessImage(QWidget *parent=0);
~ProcessImage();

void Init(int Width,int Height);

void Process(U8* inputimg,RectangleS rect,string Info,U8* outputimg);
private:
int width,height;
};

#endif // PROCESSIMAGE_H
