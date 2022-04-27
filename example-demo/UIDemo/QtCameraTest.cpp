#include "QtCameraTest.h"


#include "logger.h"
#include <QDir>
#include <QDateTime>
#include <future> 
#include <QSettings>
using namespace std;
QtCameraTest::QtCameraTest(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	AppPath = qApp->applicationDirPath();//exe所在目录
	readParam();
	onCreateNewFolder();
	initProgram();
	initCameraParamList();
	QObject::connect(ui.pB_init, SIGNAL(clicked()), this, SLOT(onInitCamera()));
	QObject::connect(ui.pB_start, SIGNAL(clicked()), this, SLOT(onStartCamera()));
	QObject::connect(ui.pB_stop, SIGNAL(clicked()), this, SLOT(onStopCamera()));
	QObject::connect(ui.cB_recode, SIGNAL(toggled(bool)), this, SLOT(onCheckBoxtoSelectSavePath(bool)));
	QObject::connect(ui.pB_NF, SIGNAL(clicked()), this, SLOT(onCreateNewFolder()));
	QObject::connect(ui.tW_detail, SIGNAL(clicked()), this, SLOT(onCreateNewFolder()));
	QObject::connect(ui.pB_Save, SIGNAL(clicked()), this, SLOT(onSavePic()));
}

void QtCameraTest::initProgram()
{
	QString erro;
	QDir finder(qApp->applicationDirPath() + "/");//finder为AppPath位置
	if (!finder.exists())
	{
		return;
	}
	QFileInfoList list = finder.entryInfoList();
	QStringList filelist = finder.entryList();
	QStringList newnames = filelist.filter("camera.dll");
	if (newnames.size() > 0)
	{
		ui.comboBox_HaveDll->addItems(newnames);
	}
}

void QtCameraTest::initCameraParamList()
{
	QStringList st;
	st.clear();
	st << QString::fromLocal8Bit("属性") << QString::fromLocal8Bit("值");
	ui.tW_detail->setColumnCount(2);
	ui.tW_detail->setHorizontalHeaderLabels(st);
	ui.tW_detail->verticalHeader()->setHidden(true);
	ui.tW_detail->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tW_detail->horizontalHeader()->setHidden(false);
	ui.tW_detail->setColumnWidth(0, 200);
	ui.tW_detail->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui.tW_detail->horizontalHeader()->setHighlightSections(false);
	QObject::connect(ui.tW_detail, SIGNAL(cellChanged(int, int)), this, SLOT(onCameraCellChange(int, int)));

}

void QtCameraTest::fillCamParamValue()
{

	ui.tW_detail->blockSignals(true);

	while (ui.tW_detail->rowCount())
	{
		ui.tW_detail->removeRow(0);
	}
	{
		int currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->setFocusPolicy(Qt::NoFocus);
		ui.tW_detail->insertRow(currentcolumn);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("触发源")));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		std::list<std::string> ls_str;
		m_camera->getTriggerList(ls_str);
		if (ls_str.size() != 0)
		{
			cb_triggerSource = new QComboBox(this);
			cb_triggerSource->setMaxCount(8);
			cb_triggerSource->addItem("NONE");
			for (std::list<std::string>::iterator it = ls_str.begin(); it != ls_str.end(); ++it)
			{
				cb_triggerSource->addItem(QString::fromStdString(*it));
			}
			connect(cb_triggerSource, QOverload<int>::of(&QComboBox::activated),
				[=](int index) {
				QString str = cb_triggerSource->currentText();
				m_camera->setCurrentTrigger(str.toStdString());
			});
			bool b = connect(cb_triggerSource, &QComboBox::textActivated, this, &QtCameraTest::onslotChangeTriggerSource);
			std::string cur;
			m_camera->getCurrentTrigger(cur);
			cb_triggerSource->setCurrentText(QString::fromStdString(cur));
			ui.tW_detail->setCellWidget(currentcolumn, 1, cb_triggerSource);

		}
		else
		{
			ui.tW_detail->setRowHeight(currentcolumn, 0);
		}

		int value = 0;

		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		QString sts = QString::fromLocal8Bit("帧率_");
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		m_camera->getCameraInt(cameramanager::FRAMES, value);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(value)));

		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->setFocusPolicy(Qt::NoFocus);
		ui.tW_detail->insertRow(currentcolumn);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem("Format"));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		ls_str.clear();
		m_camera->getFormatList(ls_str);
		if (ls_str.size() != 0)
		{
			QComboBox* cb_Formate = new QComboBox(this);
			for (std::list<std::string>::iterator it = ls_str.begin(); it != ls_str.end(); ++it)
			{
				cb_Formate->addItem(QString::fromStdString(*it));
			}
			connect(cb_Formate, QOverload<int>::of(&QComboBox::activated),
				[=](int index) {
				QString str = cb_Formate->currentText();
				m_camera->setCurrentFormat(str.toStdString());
			});
			std::string cur;
			m_camera->getCurrentFormat(cur);
			cb_Formate->setCurrentText(QString::fromStdString(cur));
			ui.tW_detail->setCellWidget(currentcolumn, 1, cb_Formate);

		}



		//button
		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);

		sts = QString::fromLocal8Bit("手动触发");
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));

		QPushButton* cb_triggerOnce = new QPushButton;
		cb_triggerOnce->setText("Trigger");
		connect(cb_triggerOnce, &QPushButton::clicked,
			[=]() {
			m_camera->triggerOnce();
		});

		ui.tW_detail->setCellWidget(currentcolumn, 1, cb_triggerOnce);



		int w = 0, h = 0, x = 0, y = 0;
		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		m_camera->getCameraInt(cameramanager::WIDTHMAX, w);
		sts = QString::fromLocal8Bit("宽度_") + QString::number(w);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));

		m_camera->getCameraInt(cameramanager::WIDTH, w);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(w)));

		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		m_camera->getCameraInt(cameramanager::HEIGHTMAX, h);
		sts = QString::fromLocal8Bit("高度_") + QString::number(h);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		m_camera->getCameraInt(cameramanager::HEIGHT, h);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(h)));

		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		m_camera->getCameraInt(cameramanager::OFFSETXMAX, x);
		sts = QString::fromLocal8Bit("X偏移量_") + QString::number(x);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		m_camera->getCameraInt(cameramanager::OFFSETX, x);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(x)));

		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		m_camera->getCameraInt(cameramanager::OFFSETYMAX, y);
		sts = QString::fromLocal8Bit("Y偏移量_") + QString::number(y);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		m_camera->getCameraInt(cameramanager::OFFSETY, y);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(y)));

		//currentcolumn = ui.tW_detail->rowCount();
		//ui.tW_detail->insertRow(currentcolumn);
		//ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("相机序列号")));
		//ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		//ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		//ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString(cameraselectID)));
		//ui.tW_detail->item(currentcolumn, 1)->setFlags(ui.tW_detail->item(currentcolumn, 1)->flags() & (~Qt::ItemIsEditable));
		//ui.tW_detail->item(currentcolumn, 1)->setFlags(ui.tW_detail->item(currentcolumn, 1)->flags() & (~Qt::ItemIsSelectable));

		//currentcolumn = ui.tW_detail->rowCount();
		//ui.tW_detail->insertRow(currentcolumn);
		////sts = QString::fromLocal8Bit("白平衡");
		//ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		//ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		//ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));

		//btn = new QPushButton;
		//btn->setText(QString::fromLocal8Bit("执行"));
		//if (!m_camera->Set_GetBalanceWhite(true))
		//{
		//	btn->setText(QString::fromLocal8Bit("无法执行"));
		//	btn->setEnabled(false);
		//	ui.tW_detail->setRowHeight(currentcolumn, 0);
		//}
		//ui.tW_detail->setCellWidget(currentcolumn, 1, btn);
		//btn->setCheckable(true);
		//QObject::connect(btn, &QPushButton::clicked, [=]() {
		//	btn->blockSignals(true);
		//	btn->setChecked(false);
		//	int ret = showMsgBox(QMessageBox::Question, "执行确认", "<img src = './ico/question.png'/>\t是否确认执行一次白平衡操作?", "确认", "取消");
		//	if (ret == QMessageBox::Yes)
		//	{
		//		if (!m_camera->Set_GetBalanceWhite(true))
		//		{
		//			btn->setChecked(false);
		//			showMsgBox(QMessageBox::Question, "执行确认", "执行白平衡失败", "确认", "取消");
		//			return;
		//		}

		//		btn->setChecked(true);
		//		ti_balance = new QTimer;
		//		ti_balance->setSingleShot(true);
		//		QObject::connect(ti_balance, &QTimer::timeout, [=]()
		//		{
		//			if (m_camera->Set_GetBalanceWhite(false))
		//			{
		//				delete ti_balance;
		//				ti_balance = nullptr;
		//				btn->setChecked(false);
		//			}
		//			else
		//			{
		//				ti_balance->start(100);
		//			}
		//		});
		//		ti_balance->start(100);
		//		b_changeCamera = true;
		//	}
		//	else
		//	{
		//		if (!m_camera->Set_GetBalanceWhite(false))
		//			btn->setChecked(false);
		//	}
		//	btn->blockSignals(false);
		//});//利用lambda表达式可用
		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("曝光时长")));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		m_camera->getCameraInt(cameramanager::EXPOUSETIME, value);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(value)));

		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("曝光时长")));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));

		QSlider* hsd = new QSlider(Qt::Horizontal);
		hsd->setMinimum(59);
		hsd->setMaximum(10000);
		hsd->setValue(value);
		ui.tW_detail->setCellWidget(currentcolumn, 1, hsd);
		connect(hsd, &QSlider::sliderReleased, [=]() {
			ui.tW_detail->item(currentcolumn - 1, 1)->setText(QString::number(hsd->value()));
		});//利用lambda表达式可用

	}

	ui.tW_detail->verticalHeader()->setSectionResizeMode(QHeaderView::Custom);
	std::string cur;
	m_camera->getCurrentTrigger(cur);
	if ("NONE" != cur)
	{
		ui.tW_detail->setRowHeight(1, 0);
	}

	ui.tW_detail->blockSignals(false);
}

void QtCameraTest::readParam()
{
	QSettings setting(AppPath + "/setting.ini", QSettings::IniFormat);
	setting.setIniCodec("utf-8");

	QString st = setting.value("CAMERASET/SERIALNUMBER", "-1").toString();
	if ("-1" != st)
	{
		ui.textEdit_sn->setText(st);
	}
	di.width = setting.value("CAMERASET/WIDTH", 1280).toInt();
	di.height = setting.value("CAMERASET/HEIGHT", 1024).toInt();
	di.offsetx = setting.value("CAMERASET/OFFSETX", 0).toInt();
	di.offsety = setting.value("CAMERASET/OFFSETY", 0).toInt();
	di.frames = setting.value("CAMERASET/FRAMES", 5).toInt();
	di.expousetime = setting.value("CAMERASET/EXPOUSETIME", 5000).toInt();
}

void QtCameraTest::saveParam()
{
	QSettings setting(AppPath + "/setting.ini", QSettings::IniFormat);
	setting.setIniCodec("utf-8");

	setting.setValue("CAMERASET/SERIALNUMBER", ui.textEdit_sn->toPlainText());

	setting.setValue("CAMERASET/WIDTH", di.width);
	setting.setValue("CAMERASET/HEIGHT", di.height);
	setting.setValue("CAMERASET/OFFSETX", di.offsetx);
	setting.setValue("CAMERASET/OFFSETY", di.offsety);
	setting.setValue("CAMERASET/FRAMES", di.frames);
	setting.setValue("CAMERASET/EXPOUSETIME", di.expousetime);
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
	cameramanager::pExportCamera CreateLocalCamera = (cameramanager::pExportCamera)(LocalFileDLL->resolve("CreateExportCameraObj"));//导出类
	if (m_camera != nullptr)
		delete m_camera;
	m_camera = CreateLocalCamera();
	if (m_camera->initCamera(ui.textEdit_sn->toPlainText().toLocal8Bit()))
	{
		LOGD("initCamera setCameraInt");
		m_camera->SetCallback(CallbackGetImg, this);
		m_camera->setCameraInt(cameramanager::WIDTH, di.width);
		m_camera->setCameraInt(cameramanager::HEIGHT, di.height);
		m_camera->setCameraInt(cameramanager::OFFSETX, di.offsetx);
		m_camera->setCameraInt(cameramanager::OFFSETY, di.offsety);
		m_camera->setCameraInt(cameramanager::FRAMES, di.frames);
		m_camera->setCameraInt(cameramanager::EXPOUSETIME, di.expousetime);
		fillCamParamValue();
	}



}
bool QtCameraTest::GetImageFromCam()
{
	int w, h, c;
	m_camera->getCameraInt(cameramanager::WIDTH, w);
	m_camera->getCameraInt(cameramanager::HEIGHT, h);
	m_camera->getCameraInt(cameramanager::IMAGECHANNEL, c);
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
		m.copyTo(m_Live);
		int zz = ui.labelshow->frameWidth();
		if (m.channels() == 3)
		{
			cv::cvtColor(m, temp, cv::COLOR_BGR2RGB);//BGR convert to RGB
			cv::resize(m, temp_resize, cv::Size(ui.labelshow->size().width() - zz * 2, ui.labelshow->size().height() - zz * 2));
			Qtemp = QImage((const unsigned char*)(temp_resize.data), temp_resize.cols, temp_resize.rows, temp_resize.step, QImage::Format_RGB888);
		}
		else
		{
			cv::cvtColor(m, temp, cv::COLOR_GRAY2RGB);//GRAY convert to RGB
			cv::resize(temp, temp_resize, cv::Size(ui.labelshow->size().width() - zz * 2, ui.labelshow->size().height() - zz * 2));
			Qtemp = QImage((const unsigned char*)(temp_resize.data), temp_resize.cols, temp_resize.rows, temp_resize.step, QImage::Format_RGB888);
		}
		if (m_bSaving)
		{
			QDateTime ti = QDateTime::currentDateTime();
			QString path = m_sSavePath + "/" + ti.toString("HH_mm_ss_zzz") + ".bmp";
			async(launch::async, [](cv::Mat img, std::string path) {
				cv::imwrite(path, img);
			}, m, path.toStdString());
		}
		ui.labelshow->setPixmap(QPixmap::fromImage(Qtemp));
		ui.labelshow->show();
	}
	return false;
}

void QtCameraTest::onStartCamera()
{
	if (m_camera)
	{
		m_camera->startGrabbing(); 
		ui.tW_detail->setEnabled(false);
	}
}

void QtCameraTest::onStopCamera()
{
	if (m_camera)
	{
		m_camera->stopGrabbing();
		ui.tW_detail->setEnabled(true);
	}
}

void QtCameraTest::closeEvent(QCloseEvent * event)
{
	saveParam();
	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}
}
QString createMultipleFolders(const QString path)
{
	QDir dir(path);
	if (dir.exists(path)) {
		return path;
	}

	QString parentDir = createMultipleFolders(path.mid(0, path.lastIndexOf('/')));
	QString dirName = path.mid(path.lastIndexOf('/') + 1);
	QDir parentPath(parentDir);
	if (!dirName.isEmpty())
	{
		parentPath.mkpath(dirName);
	}
	return parentDir + "/" + dirName;
}
void QtCameraTest::onCheckBoxtoSelectSavePath(bool b)
{
	if (b)
	{
		onCreateNewFolder();
		m_bSaving = true;
	}
	else
	{
		m_bSaving = false;
	}
}

void QtCameraTest::onCreateNewFolder()
{
	QString path = qApp->applicationDirPath() + "/savepath";
	m_sSavePath = path + QDateTime::currentDateTime().toString("/yyyy_MM_dd_HH_mm_ss") + "/";
	QDir dir(m_sSavePath);
	if (!dir.exists())
	{
		createMultipleFolders(m_sSavePath);
	}
}

void QtCameraTest::onCameraCellChange(int r, int c)
{
	((QTableWidget*)sender())->blockSignals(true);
	switch (r)
	{
	case 3:
	{
		int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
		if (m_camera)
			m_camera->stopGrabbing();
		m_camera->setCameraInt(cameramanager::WIDTH, values);
		m_camera->getCameraInt(cameramanager::WIDTH, values);
		di.width = values;
		((QTableWidget*)sender())->item(r, c)->setText(QString::number(values));
		m_camera->getCameraInt(cameramanager::WIDTHMAX, values);
		QString sts = QString::fromLocal8Bit("宽度_") + QString::number(values);
		((QTableWidget*)sender())->item(r, 0)->setText(sts);
		m_camera->getCameraInt(cameramanager::OFFSETXMAX, values);
		sts = QString::fromLocal8Bit("X偏移量_") + QString::number(values);
		((QTableWidget*)sender())->item(r + 2, 0)->setText(sts);

		break;
	}
	case 4:
	{
		int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
		if (m_camera)
			m_camera->stopGrabbing();
		m_camera->setCameraInt(cameramanager::HEIGHT, values);
		m_camera->getCameraInt(cameramanager::HEIGHT, values);
		di.height = values;
		((QTableWidget*)sender())->item(r, c)->setText(QString::number(values));
		m_camera->getCameraInt(cameramanager::HEIGHTMAX, values);
		QString sts = QString::fromLocal8Bit("高度_") + QString::number(values);
		((QTableWidget*)sender())->item(r, 0)->setText(sts);
		m_camera->getCameraInt(cameramanager::OFFSETYMAX, values);
		sts = QString::fromLocal8Bit("Y偏移量_") + QString::number(values);
		((QTableWidget*)sender())->item(r + 2, 0)->setText(sts);
		break;
	}
	case 5:
	{
		int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
		if (m_camera)
			m_camera->stopGrabbing();
		m_camera->setCameraInt(cameramanager::OFFSETX, values);
		m_camera->getCameraInt(cameramanager::OFFSETX, values);
		di.offsetx = values;
		((QTableWidget*)sender())->item(r, c)->setText(QString::number(values));
		m_camera->getCameraInt(cameramanager::OFFSETXMAX, values);
		QString sts = QString::fromLocal8Bit("X偏移量_") + QString::number(values);
		((QTableWidget*)sender())->item(r, 0)->setText(sts);
		m_camera->getCameraInt(cameramanager::WIDTHMAX, values);
		sts = QString::fromLocal8Bit("宽度_") + QString::number(values);
		((QTableWidget*)sender())->item(r - 2, 0)->setText(sts);
		break;
	}
	case 6:
	{
		int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
		int valueBefore = values;
		if (m_camera)
			m_camera->stopGrabbing();
		m_camera->setCameraInt(cameramanager::OFFSETY, values);
		m_camera->getCameraInt(cameramanager::OFFSETY, values);
		di.offsety = values;
		((QTableWidget*)sender())->item(r, c)->setText(QString::number(values));
		m_camera->getCameraInt(cameramanager::OFFSETYMAX, values);
		QString sts = QString::fromLocal8Bit("Y偏移量_") + QString::number(values);
		((QTableWidget*)sender())->item(r, 0)->setText(sts);
		m_camera->getCameraInt(cameramanager::HEIGHTMAX, values);
		sts = QString::fromLocal8Bit("高度_") + QString::number(values);
		((QTableWidget*)sender())->item(r - 2, 0)->setText(sts);

		break;
	}
	case 7:
	{
		int values = ((QTableWidget*)sender())->item(r, c)->text().toInt();
		int valueBefore = values;
		if (m_camera)
			m_camera->stopGrabbing();
		m_camera->setCameraInt(cameramanager::EXPOUSETIME, values);
		m_camera->getCameraInt(cameramanager::EXPOUSETIME, values);
		di.expousetime = values;
		((QTableWidget*)sender())->item(r, c)->setText(QString::number(values));
		((QSlider*)ui.tW_detail->cellWidget(r+1, 1))->setValue(values);
		break;
	}
	default:
		break;
	}
	((QTableWidget*)sender())->blockSignals(false);
}

void QtCameraTest::onslotChangeTriggerSource(QString trigger) {

	LOGD("onslotChangeTriggerSource,tigger:{}", trigger.toStdString());
	if ("NONE" != trigger)
		ui.tW_detail->setRowHeight(1, 0);
	else
	{
		ui.tW_detail->setRowHeight(1, ui.tW_detail->rowHeight(2));
	}
}
void QtCameraTest::onSavePic()
{
	QDateTime ti = QDateTime::currentDateTime();
	QString path = m_sSavePath + "/" + ti.toString("HH_mm_ss_zzz") + ".bmp";
	async(launch::async, [](cv::Mat img, std::string path) {
		cv::imwrite(path, img);
	}, m_Live, path.toStdString());
}
;
