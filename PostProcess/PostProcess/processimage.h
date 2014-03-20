#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H
#include <string>
#include <vector>
#include <QWidget>
#include <QtGui>
#include "itrbase.h"
using itr_math::RectangleS;
using std::vector;
using std::string;
class ProcessImage:public QWidget
{
public:
   ProcessImage(QWidget *parent=0);
    /**
     * \brief 析构函数
     */
    ~ProcessImage();
    /**
     * \brief 初始化图片
     * \param Width 图片宽度
     * \param Height 图片长度
     */
    void Init(int Width,int Height);
    /**
     * \brief 在图片中添加矩形及文本
     * \param inputimg 图片指针
     * \param rect 矩形框
     * \param Info 输入的文本信息
     * \param outputimg 输出图片的指针
     */
    void Process(U8* inputimg,RectangleS rect,std::vector<string> InfoList,U8* outputimg);
private:
    int width,height;
};

#endif // PROCESSIMAGE_H
