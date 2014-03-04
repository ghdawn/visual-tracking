#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QtGui>
#include"ui_mainwindow.h"    //奇怪?这个头文件从哪里来的？呵呵,刚才用designer做的mainwindow.ui文件，经过make后会生成这个头文件，
#include"thread.h"           //把我们前面定义的线程包含进来

class MainWindow:public QMainWindow,public Ui::MainWindow  //多继承
{
  Q_OBJECT
 public:
  MainWindow(QWidget *parent=0);
 public slots:
  void writeThread();
  void readThread();
  void closeThread();
  void display();

 private:
  Thread *yy;
};
#endif
