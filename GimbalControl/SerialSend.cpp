#include "SerialSend.h"

SerialPort serialport;
void InitSerialPort()
{
    serialport.Init("/dev/ttyACM0",115200);
}

S32 SerialSendForSSP(U8* Buffer,S32 Length)
{
    serialport.Send(Buffer,Length);
    return Length;
}
