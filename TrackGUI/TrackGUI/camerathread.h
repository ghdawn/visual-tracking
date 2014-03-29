#include <iostream>
#include <QApplication>
#include <QThread>
#include <QString>
#include "trackcore.h"
#include "itrdevice.h"
//#include "asicameraapi.h"
//#include "icamera.h"
#include "itrbase.h"
#include <vector>
#include "processimage.h"
#include <QMutex>
using namespace itr_tracker;
using itr_math::RectangleF;
using itr_math::RectangleS;
using std::string;
using std::vector;
class CameraThread : public QThread
{
public:
    CameraThread(QString name = "");
    /**
      * \brief 初始化camera线程
      * \param core Trackcore类
      */
    void Init(TrackCore* core);
    /**
      * \brief 析构函数
      */
    ~CameraThread();
    /**
      * \brief 运行线程
      */
    void run();
    /**
      * \brief 终止线程
      */
    void stop();

    QMutex* mutexCurrent;
    QMutex* mutexPost;

    unsigned char *exinfo;
    RectangleS rectangle;
private:
    volatile bool stopped;
    QString name;
    string info;
    vector<string> infolist;
    TrackCore *core;
    itr_device::AsiCamera camera;
    // itr_device::v4linux camera;
    ProcessImage process;
    int length;
    U8* inputimg;
    U8* rawImg;
    TimeClock clock;
};
