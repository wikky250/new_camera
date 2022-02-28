#include "QtCameraTest.h"

#include "icameradevice.h"
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
	QString st = AppPath + "/balser_camera.dll";
	QLibrary* BaslerCameraDLL = new QLibrary(st);
	BaslerCameraDLL->load();
	if (BaslerCameraDLL->isLoaded())
	{
		pExportCamera CreateBaslerCamera = (pExportCamera)(BaslerCameraDLL->resolve("CreateExportCameraObj"));//������
	}
	//QLibrary* HikCameraDLL = new QLibrary(AppPath + "/QtHikCameraClass.dll");
	//pExportCamera CreateHikWebCamera = (pExportCamera)(HikCameraDLL->resolve("CreateExportCameraObj"));//������
}
