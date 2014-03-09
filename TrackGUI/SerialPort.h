#ifndef SERIALPORT_H
#define SERIALPORT_H
#include<QThread>

class SerialPort:public QThread
{
    Q_OBJECT
public:
    SerialPort();
    char buf[128];
    volatile bool stopped;
    volatile bool write_rs;
    volatile bool read_rs;

protected:
    virtual void run();
};
#endif
