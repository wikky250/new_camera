#pragma once

#include <QtWidgets/QMainWindow>
#include <QLibrary>
#include <QDir>
#include <QThread>
#include <QImage>
#include <QHeaderView>
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
    cameramanager::ICameraDevice* m_camera= nullptr;
	QComboBox* cb_triggerSource = nullptr;
	cv::Mat temp, temp_resize;
	QImage Qtemp;
    //////////////////////////////////////////////////////////////////////////
	QString m_sSavePath;
	bool m_bSaving = false;

    QString AppPath;
    Ui::QtCameraTestClass ui;
    void initProgram();
	void initCameraParamList();
	void fillCamParamValue();
	

	//table button and slider
	QPushButton *btn;
	///////////
public slots:
    void onInitCamera();
	void onStartCamera();
	void onStopCamera();
	void closeEvent(QCloseEvent *event);
	void onCheckBoxtoSelectSavePath(bool);
	void onCreateNewFolder();
	void onCameraCellChange(int, int);
	void onslotChangeTriggerSource(QString trigger) {
		if ("NONE" != trigger)
			ui.tW_detail->setRowHeight(1, 0);
		else
		{
			ui.tW_detail->setRowHeight(1, ui.tW_detail->rowHeight(2));
		} };
};
