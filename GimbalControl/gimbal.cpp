#include "gimbal.h"
#include "SerialSend.h"

namespace itr_tracker
{

/** \brief
 *
 * \param X Vector
 * \return void
 *
 */
void Gimbal::Init(F32 degX,F32 degY,F32 u0,F32 v0)
{
    InitSerialPort();
    StandDataSendFun fun = SerialSendForSSP;
    protocol.Init(0xA5,0x5A, fun);
    this->degX = degX;
    this->degY = degY;
    this->u0 = u0;
    this->v0 = v0;

}

void Gimbal::Control(Vector X)
{
    float kpx = 0.5;
    float kdx = 0;
    float kpy = 0.5;
    float kdy = 0;
    float U0 = 160;
    float V0 = 120;
    float omegax;
    float omegay;
    omegax = (X[0]-U0)*kpx + X[2]*kdx;
    omegay = (X[1]-V0)*kpy + X[3]*kdy;

    ASU8(&buffer[0]) = 0x30;
    ASU8(&buffer[1]) = 1;
    ASS16(&buffer[2]) = S16(omegax*10);
    ASS16(&buffer[4]) = S16(omegay*10);
    protocol.SSPSendPackage(0,buffer,6);

}


void Gimbal::Control(F32 omegax,F32 omegay)
{
    ASU8(&buffer[0]) = 0x30;
    ASU8(&buffer[1]) = 1;
    ASS16(&buffer[2]) = S16(omegax*5);
    ASS16(&buffer[4]) = S16(omegay*5);
    protocol.SSPSendPackage(0,buffer,6);
}

}
