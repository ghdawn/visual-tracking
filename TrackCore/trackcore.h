#ifndef TRACKCORE_H
#define TRACKCORE_H

#include "itrbase.h"
#include "itrvision.h"
#include "lktracking.h"
#include "gimbal.h"
#include "asyncbuffer.h"

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
    /** \brief
     *
     * \param Width int
     * \param Height int
     * \return void
     *
     */
    void Init(int Width,int Height);
    /** \brief
     *
     * \return void
     *
     */
    void BeginTrack();
    /** \brief
     *
     * \return void
     *
     */
    void Manual();
    KalmanFilter kf;
    Matrix current;
    U8* postImg;
    int Width,Height;
//            CycleQueue<U8*> preImgQueue;
//            CycleQueue<U8*> postImageQueue;
    Gimbal gimbalControl;
    bool Tracking;
    bool TrackStatusChanged;
    bool NewTrackImg;
    bool NewPostImg;
    RectangleS posInit;
    RectangleF posTrack;
    int missedImg;
    int deltaT;
protected:
private:
};
}
#endif // TRACKCORE_H
