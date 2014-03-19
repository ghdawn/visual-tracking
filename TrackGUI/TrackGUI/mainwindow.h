#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "trackcore.h"
#include <QMainWindow>
#include <QMutex>
using namespace itr_tracker;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    /**
      * \brief 析构函数
      */
    ~MainWindow();
    TrackCore *core;
    QMutex* mutexPost;
private:
    Ui::MainWindow *ui;
private slots:
    /**
      * \brief 调用绘图事件
      * \param 绘图事件指针
      */
    void paintEvent(QPaintEvent *);
};

#endif // MAINWINDOW_H
