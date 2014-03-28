#include <QtGui/QApplication>
#include "camerathread.h"
#include "trackcore.h"
#include "itrvision.h"
#include <QtGui>
#include <string>
#include <string.h>
#include <sstream>
#include <iostream>
using std::cout;
using std::stringstream;
CameraThread::CameraThread(QString name )
{
    stopped = false;
    this->name = name;

}
CameraThread::~CameraThread()
{
    delete[] inputimg;
    delete[] rawImg;
}
void CameraThread::Init(TrackCore *core)
{
    this->core = core;
    length=core->Width*core->Height;
    inputimg=new U8[length*4];
    rawImg=new U8[length];
    process.Init(core->Width,core->Height);
    camera.Open(0,core->Width,core->Height,0);
    clock.Tick();
}
void CameraThread::run()
{
    int delta;
    int img=0,dir=0;
    char filename[20];
    while (!stopped)
    {
        infolist.clear();
        memset(rawImg,0,sizeof(rawImg));
        //for(int j=0;j<3;++j)
            camera.FetchFrame(rawImg,core->Width*core->Height,exinfo);

        delta=clock.Tick();

        core->deltaT+=delta;

        if(mutexCurrent->tryLock())
        {
            for(int i=0; i < core->Width*core->Height; i++)
            {
                core->current[i] = rawImg[i];
            }
            core->NewTrackImg=true;
            mutexCurrent->unlock();
        }


        if(!core->Tracking)
        {
            rectangle.X=core->posInit.X;
            rectangle.Y=core->posInit.Y;
            rectangle.Width=core->posInit.Width;
            rectangle.Height=core->posInit.Height;
            info="No tracker!!";
            infolist.push_back(info);
        }
        else
        {                       
            rectangle.X=core->posTrack.X;
            rectangle.Y=core->posTrack.Y;
            rectangle.Width=core->posTrack.Width;
            rectangle.Height=core->posTrack.Height;
            stringstream ss;
            ss<<"X:"<<core->posTrack.X<<"\nY:"<<core->posTrack.Y;
            ss<<"\nCamera;"<<1000/delta<<"Hz";
            for(int i=0;i<3;i++)
            {
                ss>>info;
                infolist.push_back(info);
            }
        }

        if(mutexPost->tryLock())
        {
            for(int i=0;i<length;++i)
            {
                inputimg[4*i]=rawImg[i];
                inputimg[4*i+1]=rawImg[i];
                inputimg[4*i+2]=rawImg[i];
                inputimg[4*i+3]=rawImg[i];
            }
            process.Process(inputimg,rectangle,infolist,core->postImg);
            core->NewPostImg=true;
            mutexPost->unlock();
        }
        //printf("End Draw Post!\n");
    }
}
void CameraThread::stop()
{
    stopped = true;
    cout<<name.toStdString()<<" Stopped!\n";
}

