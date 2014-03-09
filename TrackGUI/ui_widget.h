#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTextEdit *textSend;
    QPushButton *send;
    QTextEdit *textReceive;
    QPushButton *receive;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(626, 286);
        textSend = new QTextEdit(Widget);
        textSend->setObjectName(QString::fromUtf8("textSend"));
        textSend->setGeometry(QRect(10, 10, 281, 191));
        send = new QPushButton(Widget);
        send->setObjectName(QString::fromUtf8("send"));
        send->setGeometry(QRect(10, 220, 281, 41));
        textReceive = new QTextEdit(Widget);
        textReceive->setObjectName(QString::fromUtf8("textReceive"));
        textReceive->setGeometry(QRect(320, 10, 281, 191));
        receive = new QPushButton(Widget);
        receive->setObjectName(QString::fromUtf8("receive"));
        receive->setGeometry(QRect(320, 220, 281, 41));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0, QApplication::UnicodeUTF8));
        send->setText(QApplication::translate("Widget", "send", 0, QApplication::UnicodeUTF8));
        receive->setText(QApplication::translate("Widget", "receive", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Widget);
    } // retranslateUi

};

namespace Ui
{
class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
