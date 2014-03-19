#include <iostream>
#include <QApplication>
#include <QThread>
#include <QString>
#include "trackcore.h"
#include <QMutex>
using namespace itr_tracker;
class TrackThread : public QThread
{
public:
    TrackThread(QString name = "");
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
    /**
      * \brief 析构函数
      */
    ~TrackThread();
    QMutex* mutextrack;
    QMutex* mutexCurrent;
private:
    volatile bool stopped;
    QString name;
    TrackCore *core;
    lktracking *tracking;
    Matrix Hv,R;
    Vector z;
};
