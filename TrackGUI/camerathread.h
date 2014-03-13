#include <iostream>
#include <QApplication>
#include <QThread>
#include <QString>
#include "trackcore.h"
#include "itrdevice.h"
#include "asicameraapi.h"
#include "icamera.h"
#include "itrbase.h"
#include "processimage.h"
#include <QMutex>
using namespace itr_tracker;
using namespace itr_device;
using itr_math::RectangleF;
using itr_math::RectangleS;
using std::string;
class CameraThread : public QThread
{
public:
    CameraThread(QString name = "");
    void Init(TrackCore* core);
    ~CameraThread();
    void run();
    void stop();
    QMutex* mutex;
    RectangleF rect;
    string info;
    unsigned char *exinfo;
    RectangleS rectangle;
private:
    volatile bool stopped;
    QString name;
    TrackCore *core;
    AsiCamera camera;
    ProcessImage process;

};
