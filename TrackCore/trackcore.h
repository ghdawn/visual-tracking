#ifndef TRACKCORE_H
#define TRACKCORE_H

#include "itrbase.h"
#include "itrvision.h"
#include "lktracking.h"
using namespace itr_math;
using itr_container::CycleQueue;
namespace itr_tracker
{
    class TrackCore
    {
        public:
            /** Default constructor */
            TrackCore();
            ~TrackCore();
            void Init(int ImageSize);
            KalmanFilter kf;
            U8* preImg;
            U8* postImg;
//            CycleQueue<U8*> preImgQueue;
//            CycleQueue<U8*> postImageQueue;
            Gimbal gimbalControl;
        protected:
        private:

        Matrix Hx,Hv,R;
        Vector z,X,v;
    };
}
#endif // TRACKCORE_H
