#include <QtGui/QApplication>
#include "trackthread.h"
#include "trackcore.h"
#include <stdio.h>
#include <iostream>
using std::cout;
using namespace itr_tracker;

TrackThread::TrackThread(QString name)
{
    stopped = false;
    this->name = name;
    F32 data[8]= {0,0,1,0,
                  0,0,0,1
                 };
    Hv.Init(2,4);
    Hv.CopyFrom(data);
    R.Init(2,2);
    R.SetDiag(5.012306);
    z.Init(2);
    tracking=new lktracking();
}
void TrackThread::Init(TrackCore *core)
{
    this->core=core;
}
void TrackThread::run()
{
    FILE* fkf=fopen("kf.txt","w");
    while(!stopped)
    {
        ///TODO: lock

        if(core->Tracking)
        {
            if(core->NewTrackImg)
            {
                mutexCurrent->lock();
                printf("Begin Track!\n");
                if(core->TrackStatusChanged)
                {
                    tracking->Init(core->current,core->posTrack);
                    core->TrackStatusChanged=false;
                    printf("Init\n");
                }
                else
                {
                    tracking->Go(core->current,core->posTrack,z[0],z[1]);
                    Hv(0,2)=Hv(1,3)=core->deltaT;
                    core->kf.UpdateMeasure(Hv,R,z);
                    if(true)
                    {
                        printf("Delta T:%d\n",core->deltaT);
                        printf("X:%f %f\n",core->kf.x[0],core->kf.x[1]);
                        printf("P:%f %f\n",core->posTrack.X,core->posTrack.Y);
                    }
                    if(false)
                    {
                        fprintf(fkf,"%f %f %f %f\n",core->kf.x[0],core->kf.x[1],core->posTrack.X,core->posTrack.Y);
                    }
                    core->deltaT=0;
                    core->posTrack.X=core->kf.x[0];
                    core->posTrack.Y=core->kf.x[1];
                }
                core->NewTrackImg=false;
                mutexCurrent->unlock();
            }

        }
        else
        {
            if(core->TrackStatusChanged)
            {
                //fclose(fkf);
                delete tracking;
                tracking=NULL;
                tracking=new lktracking();
                core->TrackStatusChanged=false;
            }
            msleep(500);
        }
    }

}
void TrackThread::stop()
{
    stopped=true;
    cout<<name.toStdString()<<" Stopped!\n";
}
TrackThread::~TrackThread()
{
    if(tracking!=NULL)
        delete tracking;
}
