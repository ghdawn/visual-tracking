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
    ~MainWindow();
    TrackCore *core;
    QMutex* mutexPost;
private:
    Ui::MainWindow *ui;
private slots:
    void paintEvent(QPaintEvent *);
};

#endif // MAINWINDOW_H
