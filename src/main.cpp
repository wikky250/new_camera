#include "UIDemo/QtCameraTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtCameraTest w;
    w.show();
    return a.exec();
}
