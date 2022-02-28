#pragma once

#include <QtWidgets/QMainWindow>
#include <QLibrary>
#include "ui_QtCameraTest.h"

class QtCameraTest : public QMainWindow
{
    Q_OBJECT

public:
    QtCameraTest(QWidget *parent = Q_NULLPTR);

private:
    QString AppPath;
    Ui::QtCameraTestClass ui;
    void InitProgram();
};
