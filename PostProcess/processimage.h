#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H
#include "itrbase.h"
#include <QtGui>
#include <string>
#include "trackcore.h"
using std::string;
using namespace itr_tracker;
using itr_math::RectangleF;

class ProcessImage : public QWidget
{
    Q_OBJECT
public:
    ProcessImage(QWidget *parent=0);
    ~ProcessImage();
    /** \brief
     *
     * \param Width int
     * \param Height int
     * \return void
     *
     */
    void Init(int Width,int Height);
    /** \brief
     *
     * \param inputimg U8*
     * \param rect RectangleS
     * \param Info string
     * \param outputimg U8*
     * \return void
     *
     */
    void Process(U8* inputimg,RectangleS rect,string Info,U8* outputimg);
private:
    int width;
    int height;
    TrackCore *core;
//private slots:
//    void paintEvent(QPaintEvent *);

};
#endif
