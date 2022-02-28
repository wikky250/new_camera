#include "QtCameraTest.h"

#include "logger.h"
using namespace cameramanager;
QtCameraTest::QtCameraTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	AppPath = qApp->applicationDirPath();//exe����Ŀ¼

	InitProgram();

}

void QtCameraTest::InitProgram()
{
	QString erro;
	//QLibrary* LocalFileDLL = new QLibrary(AppPath + "/QtCameraClass.dll");
	//pExportCamera CreateLocalCamera = (pExportCamera)(LocalFileDLL->resolve("CreateExportCameraObj"));//������
	//QString st = AppPath + "/balser_camera.dll";
	//QLibrary* BaslerCameraDLL = new QLibrary(st);
	//BaslerCameraDLL->load();
	//if (BaslerCameraDLL->isLoaded())
	//{
	//	pExportCamera CreateBaslerCamera = (pExportCamera)(BaslerCameraDLL->resolve("CreateExportCameraObj"));//������
	//	ui.comboBox_HaveDll->addItem("Basler");
	//}

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

void QtCameraTest::onInitCamera()
{
	QLibrary* LocalFileDLL = new QLibrary(qApp->applicationDirPath() + "/" + ui.comboBox_HaveDll->currentText());
	pExportCamera CreateLocalCamera = (pExportCamera)(LocalFileDLL->resolve("CreateExportCameraObj"));//������
	m_camera = CreateLocalCamera();
}