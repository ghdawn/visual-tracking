#ifndef SERIALPORT_H
#define SERIALPORT_H
#include<QThread>

class SerialPort:public QThread
{
    Q_OBJECT
public:
    SerialPort();
    int fd;
    volatile bool stopped;
    volatile bool write_rs;
    volatile bool read_rs;
    void Init(/*char * name,*/ int baudrate);
    void send(unsigned char *Data, int length);
    void receive(unsigned char *Data, int length);
//protected:
    //virtual void run();
};
#endif
