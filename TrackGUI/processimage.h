#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H
#include "itrbase.h"
#include <QtGui>
#include <string>
using std::string;
using itr_math::RectangleF;

class ProcessImage
{
    //Q_OBJECT
public:
    ProcessImage();
    ~ProcessImage();
    void Init(int Width,int Height);
    void Process(U8* inputimg,RectangleF rect,string Info,U8* outputimg);
private:
    int width;
    int height;
//private slots:
//    void paintEvent(QPaintEvent *);

};
#endif
