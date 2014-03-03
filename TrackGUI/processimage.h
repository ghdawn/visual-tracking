#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H

#include <QtGui>

class ProcessImage : public QWidget
{
    Q_OBJECT
public:
    ProcessImage(QWidget *parent=0);
    ~ProcessImage();

private:

private slots:
    void paintEvent(QPaintEvent *);

};
#endif
