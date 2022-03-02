#pragma once

#include <QtWidgets/QMainWindow>
#include <QLibrary>
#include <QDir>
#include <QThread>
#include <QImage>
#include "ui_QtCameraTest.h"
#include "icameradevice.h"
#include <intsafe.h>
#include "opencv2/opencv.hpp"

class QtCameraTest : public QMainWindow
{
    Q_OBJECT

public:
    QtCameraTest(QWidget *parent = Q_NULLPTR);


	bool GetImageFromCam();
private:
    //camera test
    cameramanager::ICameraDevice* m_camera;
	cv::Mat temp, temp_resize;
	QImage Qtemp;
    //////////////////////////////////////////////////////////////////////////



    QString AppPath;
    Ui::QtCameraTestClass ui;
    void InitProgram();

public slots:
    void onInitCamera();
	void onStartCamera();
	void onStopCamera();
};
