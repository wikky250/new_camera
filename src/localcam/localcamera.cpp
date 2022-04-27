#include "localcamera.h"
#include "logger.h"
#include <stdio.h>
#include <io.h>
#include <string>
#include <iostream>  
#include <fstream> 

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
					//这个语句很重要  
					if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
					{
						//string newPath = folderPath + "\\" + FileInfo.name;
						//dfsFolder(newPath, fout);
					}
				}
				else
				{
					cout << m_sfolderPath.c_str() << "/" << FileInfo.name << endl;
					m_vFilePath.push_back(FileInfo.name);
				}
			} while (_findnext(Handle, &FileInfo) == 0);
			_findclose(Handle);
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

	void catchIncircle() {


	}
	bool LocalCamera::startGrabbing()
	{
		bool ret = false;
		return ret;
	}

	bool LocalCamera::stopGrabbing()
	{
		bool ret = false;
		return ret;
	}

	bool LocalCamera::getCameraInt(cameramanager::CameraInt param, int & ret_value)
	{
		return false;
	}

	bool LocalCamera::setCameraInt(cameramanager::CameraInt, int)
	{
		return false;
	}

	bool LocalCamera::getCurrentTrigger(std::string& str)
	{

		return false;
	}

	bool LocalCamera::setCurrentTrigger(std::string str)
	{
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
		list.push_back("AUTO");
		list.push_back("BYUSER");
		//LOGD("device_vendor_name:{} Get Trigger List", m_camera->GetDeviceInfo().GetSerialNumber());
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
		//data = (UINT_PTR)(p);
		return true;
	}

}  // namespace smartmore
extern "C" __declspec(dllexport) cameramanager::ICameraDevice * __stdcall CreateExportCameraObj()
{
	return new smartmore::LocalCamera();
};