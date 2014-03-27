#include <iostream>
#include <QApplication>
#include <QThread>
#include <QString>
#include "trackcore.h"
#include "itrdevice.h"
#include "gimbal.h"
#include <QMutex>
using namespace itr_tracker;
using namespace itr_device;
class JoyStickThread : public QThread
{
public:
    JoyStickThread(QString name = "");
    /**
      * \brief 析构函数
      */
    ~JoyStickThread();
    /**
      * \brief 初始化摇杆线程
      * \param core Trackcore类
      */
    void Init(TrackCore* core);
    /**
      * \brief 运行线程
      */
    void run();
    /**
      * \brief 终止线程
      */
    void stop();
    QMutex* mutex;
    TrackCore *core;
    volatile bool stopped;
private:
    QString name;
    StdJoyStick joystick;
    bool isopen;
    Gimbal gimbal;
    AsiCamera camera;
};
