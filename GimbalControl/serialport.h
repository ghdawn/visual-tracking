#ifndef SERIALPORT_H
#define SERIALPORT_H


class SerialPort
{

public:
    SerialPort();
    int fd;
    void Init(char * name, int baudrate);
    void StopSSP();
    int send(unsigned char *Data, int length);
    int receive(unsigned char *Data, int length);
//protected:
    //virtual void run();
};
#endif
