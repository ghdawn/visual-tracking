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
    }

    TrackCore()::~TrackCore()
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
}
