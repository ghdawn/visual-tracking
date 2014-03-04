#include <QtGui>
#include "sendimage.h"

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    SendImage process;
    int width;
    int height;
    width = 320;
    height =240;
    process.resize(width,height);
    process.show();

    return app.exec();
}
