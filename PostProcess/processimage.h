#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H
#include "itrbase.h"
#include <QtGui>
#include <string>
#include "trackcore.h"
using std::string;
using namespace itr_tracker;
using itr_math::RectangleF;

class ProcessImage : public QWidget
{
    Q_OBJECT
public:
    ProcessImage(QWidget *parent=0);
    ~ProcessImage();
    void Init(int Width,int Height);
    void Process(U8* inputimg,RectangleS rect,string Info,U8* outputimg);
private:
    int width;
    int height;
    TrackCore *core;
//private slots:
//    void paintEvent(QPaintEvent *);

};
#endif
