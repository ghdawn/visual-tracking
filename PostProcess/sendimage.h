#ifndef SENDIMAGE_H
#define SENDIMAGE_H

#include <QtGui>

class SendImage : public QWidget
{
    Q_OBJECT
public:
    SendImage(QWidget *parent=0);
    ~SendImage();

private:

private slots:
    void paintEvent(QPaintEvent *);

};
#endif
