#include <QtGui/QApplication>
#include "trackthread.h"
#include "trackcore.h"
using namespace itr_tracker;

TrackThread::TrackThread(QString name)
{
    stopped = false;
    this->name = name;
    Hv.Init(2,4);
    R.Init(2,2);
    z.Init(2);
}
void TrackThread::Init(TrackCore *core)
{
    this->core=core;
    current.Init(core.Height,core.Width);
}
void TrackThread::run()
{
    int i;
    int length=current.GetCol()*current.GetRow();
    F32 *imgptr;
    while(!stop)
    {
        if(core->Tracking)
        {
            mutexPreImg.lock();
            imgptr=current.GetData();
            for(i=0;i<length;++i)
                imgptr[i]=core->preImg[i];
            mutexPreImg.unlock();
            if(core->TrackInit)
            {
                lktracking->Init(current,core->posTrack);
            }
            else
            {
                lktracking->Go(current,core->posTrack,)
            }
        }
    }
}
void TrackThread::stop()
{
    stop=true;
}
TrackThread::~TrackThread()
{}
