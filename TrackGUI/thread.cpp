#include"thread.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>    //串口用到的
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#define BAUDRATE B9600
#define RS_DEVICE "/dev/ttyS0"       //串口0
//#define RS_DEVICE "/dev/ttySAC1"       //串口1

Thread::Thread()
{}                                                 //析构

void Thread::run()          //这就是线程的具体工作了
{
 int fd,c=0,res;
struct termios oldtio,newtio;     //termios结构是用来保存波特率、字符大小等

printf("start.../n");

fd=open(RS_DEVICE,O_RDWR|O_NOCTTY);     //以读写方式打开串口。不控制TTY
if(fd<0)
{
perror("error");
exit(1);                             //失败退出
}

printf("Open.../n");

tcgetattr(fd,&oldtio);             //保存当前设置到oldtio
bzero(&newtio,sizeof(newtio));     //清除newtio结构，并重新对它的成员设置如下

newtio.c_cflag=BAUDRATE|CS8|CLOCAL|CREAD;  //9600、8位、忽略DCD信号、启用接收装置
newtio.c_iflag|=IGNPAR;                    //忽略奇偶
newtio.c_oflag=0;
newtio.c_lflag=0;

newtio.c_cc[VMIN]=0;
newtio.c_cc[VTIME]=100;                   //在规定时间(VTIME)内读取(VMIN)个字符;
tcflush(fd,TCIFLUSH);                    //清除所有队列在串口的输入与输出；
tcsetattr(fd,TCSANOW,&newtio);           //把我们的设置写入termios

while(stopped)                          //stopped为0时将退出线程
{
 if(write_rs)                           //write_rs为1时把字符串从串口中输出
{
write_rs=0;
write(fd,"wan shang song ta hui qu zai da",16);  //向串口中写入字符，通过串口调试助手可看到wan shang song ta hui qu zai da这个字符
}

 if(read_rs)                           //read_rs为1时读取，并存在buf
{
read_rs=0;
res=read(fd,buf,10);     //读取串口的数据到buf
buf[res]=0;
emit finished();                      //读完后发一个信号
}

}
printf("Close.../n");
tcsetattr(fd,TCSANOW,&oldtio);      //重新设置回原来的
close(fd);
quit();
}
