#ifndef THREAD_H
#define THREAD_H
#include<QThread>

class Thread:public QThread
{
  Q_OBJECT
 public:
  Thread();
  char buf[128];
  volatile bool stopped;
  volatile bool write_rs;
  volatile bool read_rs;

 protected:
  virtual void run();
};
#endif
