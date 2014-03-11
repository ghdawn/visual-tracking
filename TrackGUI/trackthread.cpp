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
    tracking=new lktracking();
}
void TrackThread::Init(TrackCore *core)
{
    this->core=core;
    current.Init(core->Height,core->Width);
}
void TrackThread::run()
{
    int i;
    int length=current.GetCol()*current.GetRow();
    while(!stopped)
    {
    ///TODO: lock
        if(core->Tracking)
        {
            if(core->TrackStatusChanged)
            {
                tracking->Init(core->current,core->posTrack);
                core->TrackStatusChanged=false;
            }
            else
            {
                tracking->Go(core->current,core->posTrack,z[0],z[1]);
            }
        }
        else
        {
            if(core->TrackStatusChanged)
            {
                delete tracking;
                tracking=new lktracking();
                core->TrackStatusChanged=false;
            }
        }
    }
}
void TrackThread::stop()
{
    stopped=true;
}
TrackThread::~TrackThread()
{}
