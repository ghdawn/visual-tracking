
#include "SerialSend.h"

S32 SerialSendForSSP(U8* Buffer,S32 Length)
{
    serialport.Init(115200);
    serialport.send(Buffer,Length);
    return Length;
}
