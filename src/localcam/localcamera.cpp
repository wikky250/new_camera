#include "localcamera.h"
#include "logger.h"
#include <stdio.h>
#include <io.h>
#include <string>
#include <iostream>  
#include <fstream> 
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

namespace smartmore
{
	LocalCamera::LocalCamera(const cameramanager::DeviceInfo &device_info)
	{
		m_device_info = device_info;
	}

	const cameramanager::DeviceInfo &LocalCamera::getDeviceInfo()
	{
		return m_device_info;
	}


	LocalCamera::LocalCamera() {
	}

	LocalCamera::~LocalCamera()
	{
		stopGrabbing();
		LOGD("LocalCamera Destructor");
	}

	bool LocalCamera::initCamera(const char* cameranumber)
	{
		if (_access(cameranumber, 0) == 0)
		{
			_finddata_t FileInfo;
			m_sfolderPath = cameranumber;
			string strfind = m_sfolderPath + "\\*";
			intptr_t Handle = _findfirst(strfind.c_str(), &FileInfo);

			if (Handle == -1L)
			{
				cerr << "can not match the folder path" << endl;
				exit(-1);
			}
			do {
				//判断是否有子目录  
				if (FileInfo.attrib & _A_SUBDIR)
				{
					////这个语句很重要  
					if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
						continue;	
					
				//	char dirNew[200];
				//	strcpy(dirNew, cameranumber);
				//	strcat(dirNew, "\\");
				//	strcat(dirNew, FileInfo.name);

				//	initCamera(dirNew);
				}
				else
				{
					cout << m_sfolderPath.c_str() << "/" << FileInfo.name << endl;
					m_vFilePath.push_back(m_sfolderPath+"/"+FileInfo.name);
				}
			} while (_findnext(Handle, &FileInfo) == 0);
			_findclose(Handle);

			cv::Mat img = cv::imread(m_vFilePath[0]);
			if (BGRdata == nullptr)
			{
				BGRdata = new char[img.rows*img.cols * 3];
				w = img.cols;
				h = img.rows;
				c = 3;
			}
			m_trigger = 0;
			return true;

		}
		return false;
	}

	bool LocalCamera::isConnected()
	{
		return false;
	}

	bool LocalCamera::uninitCamera()
	{
		return true;
	}

	bool LocalCamera::openCamera()
	{
		bool ret = false;
		return ret;
	}

	bool LocalCamera::closeCamera()
	{
		bool ret = false;
		return ret;
	}

	void catchIncircle(LocalCamera * local)
	{
		cv::Mat img;
		for (std::vector<std::string>::iterator it = local->m_vFilePath.begin(); it != local->m_vFilePath.end(); ++it)
		{
			img = cv::imread(*it);
			if (img.channels()!=3)
			{
				cv::cvtColor(img.clone(), img, cv::COLOR_GRAY2RGB);
			}
			if (local->BGRdata == nullptr)
			{
				local->BGRdata = new char[img.rows*img.cols * 3];
				local->w = img.cols;
				local->h = img.rows;
				local->c = 3;
			}
			memcpy(local->BGRdata, img.data, img.rows*img.cols * 3);
			local->callbackfunc(local->pUser);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000/ local->m_fps));
			if (local->m_stop)
			{
				local->m_grabbing = false;
				break;
			}
		}
	}
	bool LocalCamera::startGrabbing()
	{
		m_stop = false;
		if (m_grabbing)
			return false;
		std::thread cam1(catchIncircle, this);
		cam1.detach();
		m_grabbing = true;
		return true;
	}

	bool LocalCamera::stopGrabbing()
	{
		m_stop = true;
		m_grabbing = false;
		return true;
	}

	bool LocalCamera::getCameraInt(cameramanager::CameraInt param, int & ret_value)
	{
		switch (param)
		{
		case cameramanager::WIDTH:
			ret_value = w;
			return true;
			break;
		case cameramanager::WIDTHMAX:
			break;
		case cameramanager::HEIGHT:
			ret_value = h;
			return true;
		case cameramanager::IMAGECHANNEL:
			ret_value = 3;
			return true;
			break;
		case cameramanager::FRAMES:
			ret_value = 5;
			return true;
			break;
		case cameramanager::FRAMESMAX:
			ret_value = 5;
			return true;
			break;
		case cameramanager::HEIGHTMAX:
		case cameramanager::OFFSETX:
		case cameramanager::OFFSETXMAX:
		case cameramanager::OFFSETY:
		case cameramanager::OFFSETYMAX:
		case cameramanager::EXPOUSETIME:
		case cameramanager::EXPOUSETIMEMAX:
		case cameramanager::EXPOUSETIMEMIN:
		case cameramanager::CAMIMGCOUNT:
		default:
			break;
		}
		return false;
	}

	bool LocalCamera::setCameraInt(cameramanager::CameraInt param, int val)
	{
		switch (param)
		{
		case cameramanager::FRAMES:
			m_fps = val;
			break;
		case cameramanager::WIDTH:
		case cameramanager::WIDTHMAX:
		case cameramanager::HEIGHT:
		case cameramanager::HEIGHTMAX:
		case cameramanager::IMAGECHANNEL:
		case cameramanager::OFFSETX:
		case cameramanager::OFFSETXMAX:
		case cameramanager::OFFSETY:
		case cameramanager::OFFSETYMAX:
		case cameramanager::EXPOUSETIME:
		case cameramanager::EXPOUSETIMEMAX:
		case cameramanager::EXPOUSETIMEMIN:
		case cameramanager::CAMIMGCOUNT:
		case cameramanager::FRAMESMAX:
		default:
			break;
		}
		return false;
	}

	bool LocalCamera::getCurrentTrigger(std::string& str)
	{
		if (0 == m_trigger)
			str = "NONE";
		if (1 == m_trigger)
			str = "BYUSER";
		return false;
	}

	bool LocalCamera::setCurrentTrigger(std::string str)
	{
		if (str == "NONE")
			m_trigger == 0;
		if (str == "BYUSER")
			m_trigger =1;
		return false;
	}

	bool LocalCamera::triggerOnce()
	{
		return false;
	}

	bool LocalCamera::getCurrentFormat(std::string & str)
	{
		return false;
	}

	bool LocalCamera::setCurrentFormat(std::string str)
	{
		return false;
	}

	bool LocalCamera::getFormatList(std::list<std::string>& list)
	{
		return false;
	}

	bool LocalCamera::getTriggerList(std::list<std::string>& list)
	{
		list.clear();
		list.push_back("BYUSER");
		return false;
	}


	bool LocalCamera::SetCallback(cameramanager::CallbackImage func, void * p)
	{
		pUser = p;
		callbackfunc = func;
		return true;
	}

	bool LocalCamera::getImage(UINT_PTR &data)
	{
		data = (UINT_PTR)(BGRdata);
		return true;
	}

}  // namespace smartmore
extern "C" __declspec(dllexport) cameramanager::ICameraDevice * __stdcall CreateExportCameraObj()
{
	return new smartmore::LocalCamera();
};