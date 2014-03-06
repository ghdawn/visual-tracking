#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H
#include "itrbase.h"
#include <QtGui>
#include <string>
using std::string;
using itr_math::RectangleF;

class ProcessImage : public QWidget
{
    Q_OBJECT
public:
    ProcessImage(QWidget *parent=0);
    ~ProcessImage();
    void Init(int Width,int Height);
    void process(U8* inputimg,RectangleF rect,string Info,U8* outputimg);
private:
    int width;
    int height;
//private slots:
//    void paintEvent(QPaintEvent *);

};
#endif
