#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtCameraTest.h"

class QtCameraTest : public QMainWindow
{
    Q_OBJECT

public:
    QtCameraTest(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtCameraTestClass ui;
};
