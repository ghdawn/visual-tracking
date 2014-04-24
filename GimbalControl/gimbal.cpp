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
    float k1 = 1;
    float k2 = 1;
    float k3 = 1;
    float k4 = 1;
    float U0 = 1;
    float V0 = 1;
    float omegax;
    float omegay;
    omegax = (X[0]-U0)*k1 + X[2]*k2;
    omegay = (X[1]-V0)*k3 + X[3]*k4;

    ASS32(&buffer[0]) = 0x11;
    ASU8(&buffer[4]) = 1;
    ASF32(&buffer[5]) = omegay;
    ASF32(&buffer[9]) = omegax;
    protocol.SSPSendPackage(0,buffer,13);

}


void Gimbal::Control(F32 omegax,F32 omegay)
{
    ASS32(&buffer[0]) = 0x11;
    ASU8(&buffer[4]) = 1;
    ASF32(&buffer[5]) = omegay;
    ASF32(&buffer[9]) = omegax;
    protocol.SSPSendPackage(0,buffer,13);
}

}
