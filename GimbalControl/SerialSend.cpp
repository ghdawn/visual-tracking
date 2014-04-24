#include "SerialSend.h"

SerialPort serialport;
void InitSerialPort()
{
    serialport.Init("/dev/ttyUSB0",115200);
}

S32 SerialSendForSSP(U8* Buffer,S32 Length)
{
    serialport.Send(Buffer,Length);
    serialport.Receive(Buffer,Length);
    for(int i=0;i<Length;i++)
        printf("%x ",Buffer[i]);
    return Length;
}
