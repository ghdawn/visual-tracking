#include <QApplication>
#include <QFont>
#include <QPushButton>

QPushButton *p;

void a()
{
    p->setText("AA");
}
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QPushButton quitb("Quit");
    p=&quitb;
    quitb.resize(75, 30);
    quitb.setFont(QFont("Times", 18, QFont::Bold));

    QObject::connect(&quitb, SIGNAL(clicked()), &app, SLOT(quit()));

    quitb.show();

    return app.exec();
}
