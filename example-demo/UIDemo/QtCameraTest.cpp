#include "QtCameraTest.h"


#include "logger.h"
using namespace cameramanager;
QtCameraTest::QtCameraTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	AppPath = qApp->applicationDirPath();//exe����Ŀ¼

	InitProgram();
	QObject::connect(ui.pB_init, SIGNAL(clicked()), this, SLOT(onInitCamera()));
	QObject::connect(ui.pB_start, SIGNAL(clicked()), this, SLOT(onStartCamera()));
	QObject::connect(ui.pB_stop, SIGNAL(clicked()), this, SLOT(onStopCamera()));
}

void QtCameraTest::InitProgram()
{
	QString erro;
	QString st = qApp->applicationDirPath() + "/";
	QDir finder(qApp->applicationDirPath() + "/");//finderΪAppPathλ��
	if (!finder.exists())
	{
		return;
	}
	QFileInfoList list = finder.entryInfoList(); //ͨ��entryInfoList����Ի�ȡ���˺����õ����ļ����µ��ļ���Ϣ�б������ļ�ͨ������QFileInfo�ɵõ�������ļ���ϸ��Ϣ����С�����͡���׺�ȣ�
	QStringList filelist = finder.entryList();
	QStringList newnames = filelist.filter("camera.dll");
	if (newnames.size() > 0)
	{
		ui.comboBox_HaveDll->addItems(newnames);
	}
	//QLibrary* HikCameraDLL = new QLibrary(AppPath + "/QtHikCameraClass.dll");
	//pExportCamera CreateHikWebCamera = (pExportCamera)(HikCameraDLL->resolve("CreateExportCameraObj"));//������
}

void CallbackGetImg(void* puser)
{
	((QtCameraTest*)puser)->GetImageFromCam();
	return;
};
void QtCameraTest::onInitCamera()
{
	//should add 
	QLibrary* LocalFileDLL = new QLibrary(qApp->applicationDirPath() + "/" + ui.comboBox_HaveDll->currentText());
	pExportCamera CreateLocalCamera = (pExportCamera)(LocalFileDLL->resolve("CreateExportCameraObj"));//������
	m_camera = CreateLocalCamera();
	m_camera->initCamera(ui.textEdit_sn->toPlainText().toLocal8Bit());
	m_camera->SetCallback(CallbackGetImg, this);
}
bool QtCameraTest::GetImageFromCam()
{
	int w, h, c;
	m_camera->getCameraInt(WIDTH, w);
	m_camera->getCameraInt(HEIGHT, h);
	m_camera->getCameraInt(IMAGECHANNEL, c);
	if (-1 == w || -1 == h || c == -1)
	{
		return false;
	}
	cv::Mat m(h, w, c == 1 ? CV_8UC1 : CV_8UC3);
	UINT_PTR p = 0;
	bool b = m_camera->getImage(p);

	if (b)
	{
		memcpy(m.data, (char*)p, w * h * c);
		cv::imshow("", m);
		cv::waitKey(1);
	//	int zz = ui.label->frameWidth();
	//	if (m.channels() == 3)
	//	{
	//		cvtColor(m, temp, CV_BGR2RGB);//BGR convert to RGB
	//		cv::resize(temp, temp_resize, cv::Size(ui.label->size().width() - zz * 2, ui.label->size().height() - zz * 2));
	//		Qtemp = QImage((const unsigned char*)(temp_resize.data), temp_resize.cols, temp_resize.rows, temp_resize.step, QImage::Format_RGB888);
	//	}
	//	else
	//	{
	//		cvtColor(m, temp, CV_GRAY2RGB);//GRAY convert to RGB
	//		cv::resize(temp, temp_resize, cv::Size(ui.label->size().width() - zz * 2, ui.label->size().height() - zz * 2));
	//		Qtemp = QImage((const unsigned char*)(temp_resize.data), temp_resize.cols, temp_resize.rows, temp_resize.step, QImage::Format_RGB888);
	//	}
	//	ui.label->setPixmap(QPixmap::fromImage(Qtemp));
	//	ui.label->show();
	}
	return false;
}

void QtCameraTest::onStartCamera()
{
	if (m_camera)
		m_camera->startGrabbing();
}

void QtCameraTest::onStopCamera()
{
	if (m_camera)
		m_camera->stopGrabbing();
}
