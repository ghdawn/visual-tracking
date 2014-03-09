#include <iostream>
#include <QApplication>
#include <QThread>
#include <QString>
#include <QMutex>

class Thread : public QThread
{
public:
    Thread(QString name = "")
    {
        stopped = false;
        this->name = name;
    }

    void run()
    {
        while (!stopped)
        {
            mutex->lock();
            if (stopped)
            {
                stopped = false;
                mutex->unlock();
                break;
            }

            std::cout << "In " << name.toStdString() << "'s run()." << std::endl;
            mutex->unlock();
            QThread::msleep(4000);
            qDebug("Threadlock Succeed!!");
        }
        qDebug("end!");
    }

    void stop()
    {
        mutex->lock();
        stopped = true;
        mutex->unlock();
    }
    QMutex* mutex;
private:
    volatile bool stopped;
    QString name;

};
//用来测试多线程的窗口
/*#ifndef MINWINDOW_H_
#define MINWINDOW_H_

#include <QtGui/QWidget>
#include "mythread.h"
#include <QtGui/QPushButton>

class MainWindow : public QWidget
{
public:
      MainWindow(QWidget *parent = 0);
      ~MainWindow();
      MyThread *thread;
      QPushButton *pb;
};
#endif;*/
//用来测试串口的窗口界面
/*#ifndef MAINWINDOW_H
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
#endif*/
