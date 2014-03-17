#include <QtGui/QApplication>
#include "trackthread.h"
#include "trackcore.h"
#include <stdio.h>
using namespace itr_tracker;

TrackThread::TrackThread(QString name)
{
    stopped = false;
    this->name = name;
    F32 data[8]= {0,0,1,0,
                   0,0,0,1,
                   };
    Hv.Init(2,4);
    Hv.CopyFrom(data);
    R.Init(2,2);
    R.SetDiag(30.012306);
    z.Init(2);
    tracking=new lktracking();
}
void TrackThread::Init(TrackCore *core)
{
    this->core=core;
}
void TrackThread::run()
{
    int i;
    while(!stopped)
    {
        ///TODO: lock
        if(core->Tracking)
        {
            if(core->NewTrackImg)
            {
                //mutexCurrent->lock();
                if(core->TrackStatusChanged)
                {
                    tracking->Init(core->current,core->posTrack);
                    core->TrackStatusChanged=false;
                    printf("Init\n");
                }
                else
                {
                    tracking->Go(core->current,core->posTrack,z[0],z[1]);\
                    printf("Go:%d\n",core->missedImg);
                    //Hv(0,2)=Hv(1,3)=core->missedImg;
                    core->missedImg=0;
                    core->kf.UpdateModel();
                    core->kf.UpdateMeasure(Hv,R,z);
                }
                core->NewTrackImg=false;
                //mutexCurrent->unlock();
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
            usleep(200);
        }

    }
}
void TrackThread::stop()
{
    stopped=true;
}
TrackThread::~TrackThread()
{}
