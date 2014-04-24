#ifndef SERIALPORT_H
#define SERIALPORT_H


class SerialPort
{

public:
    SerialPort();
    int fd;
    volatile bool stopped;
    volatile bool write_rs;
    volatile bool read_rs;
    void Init(char * name, int baudrate);
    void Close();
    int Send(unsigned char *Data, int length);
    int Receive(unsigned char *Data, int length);
//protected:
    //virtual void run();
};
#endif
