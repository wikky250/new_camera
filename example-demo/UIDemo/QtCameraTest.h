#pragma once

#include <QtWidgets/QMainWindow>
#include <QLibrary>
#include <QDir>
#include <QThread>
#include "ui_QtCameraTest.h"
#include "icameradevice.h"

class QtCameraTest : public QMainWindow
{
    Q_OBJECT

public:
    QtCameraTest(QWidget *parent = Q_NULLPTR);

private:
    //camera test
    cameramanager::ICameraDevice* m_camera;
    //////////////////////////////////////////////////////////////////////////



    QString AppPath;
    Ui::QtCameraTestClass ui;
    void InitProgram();
public slots:
    void onInitCamera();
};
