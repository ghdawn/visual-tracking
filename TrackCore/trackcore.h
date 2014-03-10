#ifndef TRACKCORE_H
#define TRACKCORE_H

#include "itrbase.h"
#include "itrvision.h"
#include "lktracking.h"
#include "gimbal.h"
using itr_math::RectangleF;
using itr_math::RectangleS;
namespace itr_tracker
{
    class TrackCore
    {
        public:
            /** Default constructor */
            TrackCore();
            ~TrackCore();
            void Init(int ImageSize);
            void BeginTrack();
            void Manual();
            KalmanFilter kf;
            U8* preImg;
            U8* postImg;
            int Width,Height;
//            CycleQueue<U8*> preImgQueue;
//            CycleQueue<U8*> postImageQueue;
            Gimbal gimbalControl;
            bool Tracking;
            bool TrackInit;
            RectangleS posInit;
            RectangleF posTrack;
            int missedImg;
        protected:
        private:
    };
}
#endif // TRACKCORE_H
