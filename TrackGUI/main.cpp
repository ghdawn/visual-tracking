#include <QtGui>
#include "processImage.h"

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    ProcessImage process;
    process.resize(380,250);
    process.show();

    return app.exec();
}
