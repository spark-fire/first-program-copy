#include "simpleorpewidget.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //add Lyj 20171030 read qss stylesheet
    QFile qssFile("./AuboStyleSheet/aubo-stylesheet.qss");//file
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        QString qss = QLatin1String(qssFile.readAll());
        a.setStyleSheet(qss);
        qssFile.close();
    }
    //---end---
    SimpleORPEWidget w;
    w.show();

    return a.exec();
}
