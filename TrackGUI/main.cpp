#include<QApplication>
#include"mainwindow.h"

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    MainWindow mw;
    mw.show();
    return app.exec();
}
/*#include <QtGui>
#include "processimage.h"

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    ProcessImage process;
    int width;
    int height;
    width = 320;
    height =240;
    process.resize(width,height);
    process.show();

    return app.exec();
}*/
