#include <QtGui/QApplication>
#include "camerathread.h"
#include "trackcore.h"
#include "itrvision.h"
#include <QtGui>
#include <string>
CameraThread::CameraThread(QString name )
{
    stopped = false;
    this->name = name;
    camera.Open(0,320,240,2);
}
CameraThread::~CameraThread()
{

}
void CameraThread::Init(TrackCore *core)
{
    this->core = core;
    length=core->Width*core->Height;
    inputimg=new U8[length*4];
    rawImg=new U8[length];
    process.Init(core->Width,core->Height);
}
void CameraThread::run()
{
    while (!stopped)
    {
        camera.FetchFrame(rawImg,core->Width*core->Height,exinfo);
        if(mutexCurrent->tryLock())
        {
            for(int i=0; i < core->Width*core->Height; i++)
            {
                core->current[i] = rawImg[i];
            }
           // itr_vision::IOpnm::WritePGMFile("cur.pgm",core->current);
            core->NewTrackImg=true;
            mutexCurrent->unlock();
        }
        if(core->Tracking==false)
        {
            rectangle.X=core->posInit.X;
            rectangle.Y=core->posInit.Y;
            rectangle.Width=core->posInit.Width;
            rectangle.Height=core->posInit.Height;
            info="No tracker!!";
        }
        else
        {
            rectangle.X=core->posTrack.X;
            rectangle.Y=core->posTrack.Y;
            rectangle.Width=core->posTrack.Width;
            rectangle.Height=core->posTrack.Height;
            info="X:";
            info+=core->posTrack.X+core->posTrack.Width/2;
            info+="Y:";
            info+=core->posTrack.Y+core->posTrack.Height/2;
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
            process.Process(inputimg,rectangle,info,core->postImg);
            core->NewPostImg=true;
            mutexPost->unlock();
        }
    }
}
void CameraThread::stop()
{
    stopped = true;
}

