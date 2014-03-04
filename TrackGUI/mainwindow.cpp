#include"mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  :QMainWindow(parent)
{
  setupUi(this);
  yy = new Thread;
  yy->start();          //启动线程
  yy->stopped=1;        //初始化变量
  yy->write_rs=0;
  yy->read_rs=0;

  connect(writeButton,SIGNAL(clicked()),this,SLOT(writeThread()));      //信号与槽
  connect(readButton,SIGNAL(clicked()),this,SLOT(readThread()));
  connect(closeButton,SIGNAL(clicked()),this,SLOT(closeThread()));
  connect(yy,SIGNAL(finished()),this,SLOT(display()));      //前面线程读完了不是发一个信号么，这个信号就是发到这个槽
}

void MainWindow::display()
{
dis_label->setText(yy->buf);     //读到的在dis_label显示，dis_label就是我们前面designer放的标签，显示buf中的内容
}

void MainWindow::writeThread()  //前面线程都是根据stopped、write_rs、read_rs的状态来工作的
{
  yy->write_rs=1;
}

void MainWindow::readThread()
{
  yy->read_rs=1;
}

void MainWindow::closeThread()
{
yy->stopped=0;
}
