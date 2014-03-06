#ifndef SENDIMAGE_H
#define SENDIMAGE_H

#include <QtGui>
#include "itrbase.h"

class SendImage : public QWidget
{
    Q_OBJECT
public:
    SendImage(QWidget *parent=0);
    ~SendImage();
    void Init(int width,int height);
    void process(U8* inputimg,RectangleF rect,string Info,U8* outputimg);
private:

private slots:
    void paintEvent(QPaintEvent *);

};
#endif
