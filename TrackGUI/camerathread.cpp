#include <QtGui/QApplication>
#include "camerathread.h"
#include "trackcore.h"
#include <string>
CameraThread::CameraThread(QString name )
{
    stopped = false;
    this->name = name;
}
CameraThread::~CameraThread()
{

}
void CameraThread::Init(TrackCore* core)
{

    this->core = core;
}
void CameraThread::run()
{
    while (!stopped)
    {
        camera.FetchFrame(inputimg,core->Width*core->Height,exinfo);
        int i;
        for(i=0; i < core->Width*core->Height; i++)
        {
            core->current[i] = inputimg[i];
        }
        process.Init(core->Width,core->Height);
        process.ProcessImage(inputimg,rect,info,core->postImg);
        core->NewPostImg=true;
    }
}
void CameraThread::stop()
{
    stopped = true;
}

