#ifndef SERIALSEND_H
#define SERIALSEND_H
#include "serialport.h"
#include "itrbase.h"
S32 SerialSendForSSP(U8* Buffer,S32 Length);
static SerialPort serialport;

#endif // SERIALSEND_H
