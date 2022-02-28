#include "QtCameraTest.h"

#include "logger.h"
using namespace cameramanager;
QtCameraTest::QtCameraTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	AppPath = qApp->applicationDirPath();//exe所在目录

	InitProgram();

}

void QtCameraTest::InitProgram()
{
	QString erro;
	//QLibrary* LocalFileDLL = new QLibrary(AppPath + "/QtCameraClass.dll");
	//pExportCamera CreateLocalCamera = (pExportCamera)(LocalFileDLL->resolve("CreateExportCameraObj"));//导出类
	//QString st = AppPath + "/balser_camera.dll";
	//QLibrary* BaslerCameraDLL = new QLibrary(st);
	//BaslerCameraDLL->load();
	//if (BaslerCameraDLL->isLoaded())
	//{
	//	pExportCamera CreateBaslerCamera = (pExportCamera)(BaslerCameraDLL->resolve("CreateExportCameraObj"));//导出类
	//	ui.comboBox_HaveDll->addItem("Basler");
	//}

	QString st = qApp->applicationDirPath() + "/";
	QDir finder(qApp->applicationDirPath() + "/");//finder为AppPath位置
	if (!finder.exists())
	{
		return;
	}
	QFileInfoList list = finder.entryInfoList(); //通过entryInfoList则可以获取过滤后所得到的文件夹下的文件信息列表，遍历文件通过操作QFileInfo可得到所需的文件详细信息（大小、类型、后缀等）
	QStringList filelist = finder.entryList();
	QStringList newnames = filelist.filter("camera.dll");
	if (newnames.size() > 0)
	{
		ui.comboBox_HaveDll->addItems(newnames);
	}
	//QLibrary* HikCameraDLL = new QLibrary(AppPath + "/QtHikCameraClass.dll");
	//pExportCamera CreateHikWebCamera = (pExportCamera)(HikCameraDLL->resolve("CreateExportCameraObj"));//导出类
}

void QtCameraTest::onInitCamera()
{
	QLibrary* LocalFileDLL = new QLibrary(qApp->applicationDirPath() + "/" + ui.comboBox_HaveDll->currentText());
	pExportCamera CreateLocalCamera = (pExportCamera)(LocalFileDLL->resolve("CreateExportCameraObj"));//导出类
	m_camera = CreateLocalCamera();
}