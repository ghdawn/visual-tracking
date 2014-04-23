#ifndef SERIALSEND_H
#define SERIALSEND_H
#include "serialport.h"
#include "itrbase.h"

void InitSerialPort();
S32 SerialSendForSSP(U8* Buffer,S32 Length);

#endif // SERIALSEND_H
