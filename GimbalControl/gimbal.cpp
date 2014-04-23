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
    }

    void Gimbal::Control(F32 omegax,F32 omegay)
    {
        U8 *buffer;
        ASF32(&buffer[1]) = omegax;
        ASF32(&buffer[2]) = omegay;
        //StandDataSendFun fun=SerialSendForSSP;
//        protocol.Init(0xA5,0x5A, SerialSendForSSP);
//        protocol.SSPSendPackage(0,buffer,8);

    }

    void Gimbal::Control()
    {
        S32 axiscount;
        F32 axisvalue[30]={0};
        while(1)
        {
            joystick.Update();
            axiscount=joystick.GetAxisCount();
            for(S32 i=0; i<axiscount; i++)
            {
                axisvalue[i]=joystick.GetAxisValue(i);
            }
            if(axisvalue[0]>0.5||axisvalue[0]<-0.5)
            {
                ASF32(&buffer[1]) = axisvalue[0];
            }
            if(axisvalue[1]>0.5||axisvalue[1]<-0.5)
            {
                ASF32(&buffer[2]) = axisvalue[1];
            }
        }
    }
}
