#include <QtGui/QApplication>
#include <QTextCodec>
#include <QDebug>

#include "widget.h"
#include "pindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    Widget w;
    w.show();

    return a.exec();
}
