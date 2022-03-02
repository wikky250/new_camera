#include "QtCameraTest.h"


#include "logger.h"
using namespace cameramanager;
QtCameraTest::QtCameraTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	AppPath = qApp->applicationDirPath();//exe����Ŀ¼

	initProgram();
	initCameraParamList();
	QObject::connect(ui.pB_init, SIGNAL(clicked()), this, SLOT(onInitCamera()));
	QObject::connect(ui.pB_start, SIGNAL(clicked()), this, SLOT(onStartCamera()));
	QObject::connect(ui.pB_stop, SIGNAL(clicked()), this, SLOT(onStopCamera()));
}

void QtCameraTest::initProgram()
{
	QString erro;
	QDir finder(qApp->applicationDirPath() + "/");//finderΪAppPathλ��
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
	st << QString::fromLocal8Bit("����") << QString::fromLocal8Bit("ֵ");
	ui.tW_detail->setColumnCount(2);
	ui.tW_detail->setHorizontalHeaderLabels(st);
	ui.tW_detail->verticalHeader()->setHidden(true);
	ui.tW_detail->horizontalHeader()->setHidden(false);
	//ui.tW_detail->setColumnWidth(0, 300);
	ui.tW_detail->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	ui.tW_detail->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui.tW_detail->horizontalHeader()->setHighlightSections(false);
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
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("����Դ")));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		std::list<std::string> ls_str;
		m_camera->getTriggerList(ls_str);
		if (ls_str.size()!=0)
		{
			QComboBox* cb_triggerSource = new QComboBox;
			cb_triggerSource->setMaxCount(8);
			cb_triggerSource->addItem("NONE");
			for (std::list<std::string>::iterator it = ls_str.begin(); it != ls_str.end();++it)
			{
				cb_triggerSource->addItem(QString::fromStdString(*it));
			}
			connect(cb_triggerSource, QOverload<int>::of(&QComboBox::activated),
				[=](int index) {
				QString str = cb_triggerSource->currentText();
				m_camera->setCurrentTrigger(str.toStdString());
			});
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

		QString sts = QString::fromLocal8Bit("֡��");
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		m_camera->getCameraInt(FRAMES, value);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(value)));

		//value = -1;
		//QString localfile;
		//m_camera->GetTypeOfCamera(value, localfile);
		//currentcolumn = ui.tW_detail->rowCount();
		//ui.tW_detail->insertRow(currentcolumn);
		//QPushButton* btn = new QPushButton(ui.tW_detail);
		//btn->setText(localfile);
		//btn->setStyleSheet("text-align:right;");
		//btn->resize(ui.tW_detail->size().width(), ui.tW_detail->rowHeight(currentcolumn));
		//ui.tW_detail->setCellWidget(currentcolumn, 1, btn);
		//ui.tW_detail->setSpan(currentcolumn, 0, 1, 2);
		//QObject::connect(btn, &QPushButton::clicked, [=]() {
		//	QString dirpath = QFileDialog::getExistingDirectory(this, "ѡ��Ŀ¼", "./", QFileDialog::ShowDirsOnly);
		//	if (dirpath.length() > 0)
		//	{
		//		if (m_camera->SetLocalCameraPath(dirpath))
		//		{
		//			btn->setText(dirpath);
		//			btn->setWhatsThis(dirpath);
		//		}
		//		b_changeCamera = true;
		//	}
		//});

		if (0 != value)
		{
			ui.tW_detail->setRowHeight(currentcolumn, 0);
		}

		int w = 0, h = 0, x = 0, y = 0;
		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		m_camera->getCameraInt(WIDTHMAX, w);
		sts = QString::fromLocal8Bit("ROI���") + QString::number(w);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));

		m_camera->getCameraInt(WIDTH, w);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(w)));

		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		m_camera->getCameraInt(HEIGHTMAX, h);
		//sts = QString::fromLocal8Bit("ROI�߶�") + QString::number(h);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		m_camera->getCameraInt(HEIGHT, h);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(h)));

		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		m_camera->getCameraInt(OFFSETXMAX, x);
		sts = QString::fromLocal8Bit("Xƫ����") + QString::number(x);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		m_camera->getCameraInt(OFFSETX, x);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(x)));

		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		m_camera->getCameraInt(OFFSETYMAX, y);
		sts = QString::fromLocal8Bit("Yƫ����") + QString::number(y);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		m_camera->getCameraInt(OFFSETY, y);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(y)));

		//currentcolumn = ui.tW_detail->rowCount();
		//ui.tW_detail->insertRow(currentcolumn);
		//ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("������к�")));
		//ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		//ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		//ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString(cameraselectID)));
		//ui.tW_detail->item(currentcolumn, 1)->setFlags(ui.tW_detail->item(currentcolumn, 1)->flags() & (~Qt::ItemIsEditable));
		//ui.tW_detail->item(currentcolumn, 1)->setFlags(ui.tW_detail->item(currentcolumn, 1)->flags() & (~Qt::ItemIsSelectable));

		//currentcolumn = ui.tW_detail->rowCount();
		//ui.tW_detail->insertRow(currentcolumn);
		////sts = QString::fromLocal8Bit("��ƽ��");
		//ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(sts));
		//ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		//ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));

		//btn = new QPushButton;
		//btn->setText(QString::fromLocal8Bit("ִ��"));
		//if (!m_camera->Set_GetBalanceWhite(true))
		//{
		//	btn->setText(QString::fromLocal8Bit("�޷�ִ��"));
		//	btn->setEnabled(false);
		//	ui.tW_detail->setRowHeight(currentcolumn, 0);
		//}
		//ui.tW_detail->setCellWidget(currentcolumn, 1, btn);
		//btn->setCheckable(true);
		//QObject::connect(btn, &QPushButton::clicked, [=]() {
		//	btn->blockSignals(true);
		//	btn->setChecked(false);
		//	int ret = showMsgBox(QMessageBox::Question, "ִ��ȷ��", "<img src = './ico/question.png'/>\t�Ƿ�ȷ��ִ��һ�ΰ�ƽ�����?", "ȷ��", "ȡ��");
		//	if (ret == QMessageBox::Yes)
		//	{
		//		if (!m_camera->Set_GetBalanceWhite(true))
		//		{
		//			btn->setChecked(false);
		//			showMsgBox(QMessageBox::Question, "ִ��ȷ��", "ִ�а�ƽ��ʧ��", "ȷ��", "ȡ��");
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
		//});//����lambda���ʽ����
		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("�ع�ʱ��")));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));
		m_camera->getCameraInt(EXPOUSETIME, value);
		ui.tW_detail->setItem(currentcolumn, 1, new QTableWidgetItem(QString::number(value)));

		currentcolumn = ui.tW_detail->rowCount();
		ui.tW_detail->insertRow(currentcolumn);
		ui.tW_detail->setItem(currentcolumn, 0, new QTableWidgetItem(QString::fromLocal8Bit("�ع�ʱ��")));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsEditable));
		ui.tW_detail->item(currentcolumn, 0)->setFlags(ui.tW_detail->item(currentcolumn, 0)->flags() & (~Qt::ItemIsSelectable));

		QSlider* hsd = new QSlider(Qt::Horizontal);
		hsd->setMinimum(59);
		hsd->setMaximum(10000);
		hsd->setValue(value);
		ui.tW_detail->setCellWidget(currentcolumn, 1, hsd);
		connect(hsd, &QSlider::sliderReleased, [=]() {
			ui.tW_detail->item(currentcolumn - 1, 1)->setText(QString::number(hsd->value()));
		});//����lambda���ʽ����

	}

	ui.tW_detail->blockSignals(false);
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
	if (m_camera != nullptr)
		delete m_camera;
	m_camera = CreateLocalCamera();
	m_camera->initCamera(ui.textEdit_sn->toPlainText().toLocal8Bit());
	m_camera->SetCallback(CallbackGetImg, this);

	fillCamParamValue();



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
		ui.labelshow->setPixmap(QPixmap::fromImage(Qtemp));
		ui.labelshow->show();
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

void QtCameraTest::closeEvent(QCloseEvent * event)
{
	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}
}
