#include <QtGui>
#include "processImage.h"

ProcessImage::ProcessImage(QWidget *parent):QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(200);//定时器，每隔0.2秒刷新
    setWindowTitle(tr("Capture"));

}

ProcessImage::~ProcessImage()
{

}

void ProcessImage::paintEvent(QPaintEvent *)
{
    QPixmap pixImage;
    static int i=1;
    pixImage.load(tr("./img/%1.jpg").arg(i));//当前文件夹中有个img文件夹，期中有19张图片，名为i.jpg
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
    }
}
