#include "trackcore.h"

namespace itr_tracker
{
    TrackCore::TrackCore()
    {
        kf.Init(4);
        F32 data[24]= {1,0,1,0,
                       0,1,0,1,
                       0,0,1,0,
                       0,0,0,1,
                      };
        kf.F_x.CopyFrom(data);
        preImg=NULL;
        postImg=NULL;
        posInit.Init(144,108,32,24);
        int missedImg=0;
    }

    TrackCore::~TrackCore()
    {
        if(preImg!=NULL)
            delete []preImg;
        if(postImg!=NULL)
            delete []postImg;
    }
    void TrackCore::Init(int ImageSize)
    {
        preImg=new U8[ImageSize];
        postImg=new U8[ImageSize];
    }

    void TrackCore::BeginTrack()
    {
        Tracking=true;
        TrackInit=true;
        kf.x[0]=posInit.X;
        kf.x[1]=posInit.Y;
        kf.x[2]=kf.x[3]=0;

        posTrack.X=posInit.X;
        posTrack.Y=posInit.Y;
        posTrack.Width=posInit.Width;
        posTrack.Height=posInit.Height;
    }

    void TrackCore::Manual()
    {
        Tracking=false;
        TrackInit=false;
    }
}
