#include "baslercamera.h"
#include "logger.h"
#include "pylon/PylonVersion.h"
#include "pylon/PylonVersionInfo.h"
#include <iostream>

namespace smartmore
{

BaslerCamera::BaslerCamera(const cameramanager::DeviceInfo &device_info)
{
    m_device_info = device_info;
    // Before using any pylon methods, the pylon runtime must be initialized.
    PylonInitialize();
}

const cameramanager::DeviceInfo &BaslerCamera::getDeviceInfo()
{
    return m_device_info;
}


BaslerCamera::BaslerCamera() {
	PylonInitialize();
}

BaslerCamera::~BaslerCamera()
{
	stopGrabbing();
	LOGD("BaslerCamera Destructor");
    m_camera.reset();
	PylonTerminate();
}

bool BaslerCamera::initCamera(const char* cameranumber)
{
    bool ret = false;

	if (m_camera)
	{
		LOGE("device_vendor_name:{} has already attached", cameranumber);
		return false;
	}

	int exitCode = 0;
	try
	{
		DeviceInfoList_t info_list;
		CTlFactory::GetInstance().EnumerateDevices(info_list);
		for (auto& it = info_list.begin(); it != info_list.end(); ++it)
		{
			const std::string device_vendor_name = it->GetVendorName();
			LOGD("device_vendor_name:{}", device_vendor_name);
			if (device_vendor_name != "Basler")
			{
				continue;
			}
            if (it->GetSerialNumber()!= cameranumber)
			{
				continue;
            }
            m_camera = std::make_shared<CBaslerUniversalInstantCamera>(CTlFactory::GetInstance().CreateDevice(*it));
			ret = true;
			break;
		}
	}
	catch (const GenericException& e)
	{
		// Error handling.
		std::cerr << "An exception occurred." << std::endl << e.GetDescription() << std::endl;
	}
	
    return ret;
}

bool BaslerCamera::isConnected()
{
    if (m_camera)
    {
        return m_camera->IsOpen();
    }
    return false;
}

bool BaslerCamera::uninitCamera()
{
    if (m_camera && m_camera->IsGrabbing())
    {
        m_camera->StopGrabbing();
    }
    if (m_camera && m_camera->IsOpen())
    {
        m_camera->Close();
    }

    return true;
}

bool BaslerCamera::openCamera()
{
    bool ret = false;
    if (m_camera)
    {
        try
        {
            m_camera->Open();
            ret = true;
        }
        catch (const std::exception &e)
        {
            ret = false;
            std::cerr << e.what() << '\n';
        }
    }
    return ret;
}

bool BaslerCamera::closeCamera()
{
    bool ret = false;
    if (m_camera && m_camera->IsOpen())
    {
        try
        {
            m_camera->Close();
            ret = true;
        }
        catch (const std::exception &e)
        {
            ret = false;
            std::cerr << e.what() << '\n';
        }
    }
    return ret;
}

void catchIncircle(std::shared_ptr<CBaslerUniversalInstantCamera> cam,BaslerCamera * basler) {

	CGrabResultPtr ptrGrabResult;
	CImageFormatConverter converter;
	converter.OutputPixelFormat = PixelType_RGB8packed;
	while (cam->IsGrabbing())
	{
		// Retrieve grab results and notify the camera event and image event handlers.
		if (cam->RetrieveResult(5000, ptrGrabResult, TimeoutHandling_Return))
		{
			converter.OutputBitAlignment = OutputBitAlignment_MsbAligned;
			converter.Convert(basler->pylonBGRdata, ptrGrabResult);
			basler->callbackfunc(basler->pUser);
		}
		//std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		// Nothing to do here with the grab result The grab results are handled by the registered event handlers.
	}
}
bool BaslerCamera::startGrabbing()
{
    //LOGD("m_extern_trigger:{}", m_extern_trigger);
    //m_camera->TriggerMode.TrySetValue(TriggerModeEnums::TriggerMode_On);
    //m_camera->TriggerSource.TrySetValue(TriggerSourceEnums::TriggerSource_Software);

    bool ret = false;
    if (m_camera)
    {
        try
        {
			if (m_camera->IsGrabbing())
			{
				LOGE("device_vendor_name:{} is grabbing", m_camera->GetDeviceInfo().GetSerialNumber());
				return false;
			}
			m_camera->Open();
			pylonBGRdata = CPylonImage::Create(PixelType_RGB8packed, m_camera->Width.GetValue(), m_camera->Height.GetValue());
            m_camera->StartGrabbing();
			LOGD("device_vendor_name:{} Start to Grab", m_camera->GetDeviceInfo().GetSerialNumber());

			std::thread cam1(catchIncircle, m_camera,this);
			cam1.detach();
        }
        catch (const std::exception &e)
        {
            ret = false;
            std::cerr << e.what() << '\n';
        }
		catch (const GenericException& e)
		{
			// Error handling.
			LOGE("Pylon ERROR:{} ", e.GetDescription());
			return false;
		}
    }
    return ret;
}

bool BaslerCamera::stopGrabbing()
{
    bool ret = false;
    if (m_camera && m_camera->IsOpen() && m_camera->IsGrabbing())
    {
        try
        {
            m_camera->StopGrabbing();
			pylonBGRdata.Release();
			m_camera->Close();
			LOGD("device_vendor_name:{} Stop to Grab", m_camera->GetDeviceInfo().GetSerialNumber());
            ret = true;
        }
        catch (const std::exception &e)
        {
            ret = false;
            std::cerr << e.what() << '\n';
        }
		catch (const GenericException& e)
		{
			// Error handling.
			LOGE("Pylon ERROR:{} ", e.GetDescription());
			return false;
		}
    }
    return ret;
}

bool BaslerCamera::getCameraInt(cameramanager::CameraInt param, int & ret_value)
{
	bool ret = false;
	try
	{
		if (!m_camera)
		{
			return false;
		}
		switch (param)
		{
		case cameramanager::WIDTH:
			ret_value = m_camera->Width.GetValue();
			ret = true;
			break;
		case cameramanager::WIDTHMAX:
			ret_value = m_camera->Width.GetMax();
			ret = true;
			break;
		case cameramanager::HEIGHT:
			ret_value = m_camera->Height.GetValue();
			ret = true;
			break;
		case cameramanager::HEIGHTMAX:
			ret_value = m_camera->Height.GetMax();
			ret = true;
			break;
		case cameramanager::IMAGECHANNEL:
			ret_value = 3;
			ret = true;
			break;
		case cameramanager::OFFSETX:
			ret_value = m_camera->OffsetX.GetValue();
			ret = true;
			break;
		case cameramanager::OFFSETXMAX:
			ret_value = m_camera->OffsetX.GetMax();
			ret = true;
			break;
		case cameramanager::OFFSETY:
			ret_value = m_camera->OffsetY.GetValue();
			ret = true;
			break;
		case cameramanager::OFFSETYMAX:
			ret_value = m_camera->OffsetY.GetMax();
			ret = true;
			break;
		case cameramanager::FRAMES:
			if (m_camera->IsUsb())
			{
				ret_value = m_camera->AcquisitionFrameRate.GetValue();
			}
			if (m_camera->IsGigE())
			{
				ret_value = m_camera->AcquisitionFrameRateAbs.GetValue();
			}
			ret = true;
			break;
		case cameramanager::FRAMESMAX:
			if (m_camera->IsUsb())
			{
				ret_value = m_camera->AcquisitionFrameRate.GetMax();
			}
			if (m_camera->IsGigE())
			{
				ret_value = m_camera->AcquisitionFrameRateAbs.GetMax();
			}
			ret = true;
			break;
		case cameramanager::EXPOUSETIME:
			if (m_camera->IsGigE())
			{
				ret_value = m_camera->ExposureTimeAbs.GetValue();
			}
			if (m_camera->IsUsb())
			{
				ret_value = m_camera->ExposureTime.GetValue();
			}
			ret = true;
			break;
		case cameramanager::EXPOUSETIMEMAX:
			if (m_camera->IsGigE())
			{
				ret_value = m_camera->ExposureTimeAbs.GetMax();
			}
			if (m_camera->IsUsb())
			{
				ret_value = m_camera->ExposureTime.GetMax();
			}
			ret = true;
			break;
		case cameramanager::EXPOUSETIMEMIN:
			if (m_camera->IsGigE())
			{
				ret_value = m_camera->ExposureTimeAbs.GetMax();
			}
			if (m_camera->IsUsb())
			{
				ret_value = m_camera->ExposureTime.GetMax();
			}
			ret = true;
			break;
		case cameramanager::CAMIMGCOUNT:
			//ret_value = m_camera->totalcount;
			ret = true;
			break;
		default:
			break;
		}
	}
	catch (const GenericException& e)
	{
		// Error handling.
		LOGE("Pylon ERROR:{} ", e.GetDescription());
		return false;
	}
	return false;
}

bool BaslerCamera::setCameraInt(cameramanager::CameraInt param, int value)
{
	try
	{
		switch (param)
		{
		case cameramanager::WIDTH:
			m_camera->Width.SetValue(value);
			break;
		case cameramanager::HEIGHT:
			m_camera->Height.SetValue(value);
			break;
		case cameramanager::OFFSETX:
			m_camera->OffsetX.SetValue(value);
			break;
		case cameramanager::OFFSETY:
			m_camera->OffsetY.SetValue(value);
			break;
		case cameramanager::FRAMES:
		{
			if (m_camera->IsUsb())
			{
				m_camera->AcquisitionFrameRate.SetValue(value);
			}
			if (m_camera->IsGigE())
			{
				m_camera->AcquisitionFrameRateAbs.SetValue(value);
			}
			break;
		}
		case cameramanager::EXPOUSETIME:
		{
			if (m_camera->IsGigE())
			{
				m_camera->ExposureTimeAbs.SetValue(value);
			}
			if (m_camera->IsUsb())
			{
				m_camera->ExposureTime.SetValue(value);
			}
			break;
		}
		case cameramanager::CAMIMGCOUNT:
			break;
		default:
			break;
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool BaslerCamera::getCurrentTrigger(std::string &str)
{
	try
	{
		if (!m_camera)
		{
			return false;
		}
		if (!m_camera->IsOpen())
		{
			m_camera->Open();
		}
		m_camera->TriggerSelector.SetValue(TriggerSelector_AcquisitionStart);
		TriggerModeEnums te = m_camera->TriggerMode.GetValue();
		if (m_camera->TriggerMode.GetValue() == TriggerMode_Off)
		{
			str = "NONE";
			return true;
		}
		else
		{
			str = m_camera->TriggerSource.ToString();
		}
		m_camera->Close();
	}
	catch (const GenericException& e)
	{
		// Error handling.
		LOGE("Pylon ERROR:{} ", e.GetDescription());
		return false;
	}
	return false;
}

bool BaslerCamera::setCurrentTrigger(std::string str)
{
	try
	{
		if (!m_camera)
		{
			return false;
		}
		if (!m_camera->IsOpen())
		{
			m_camera->Open();
		}
		if (str == "NONE")
		{
			m_camera->TriggerSelector.SetValue(TriggerSelector_AcquisitionStart);
			m_camera->TriggerMode.SetValue(TriggerMode_Off);
			m_camera->AcquisitionFrameRateEnable.SetValue(true);
			m_camera->AcquisitionFrameRateAbs.SetValue(5);
			return true;
		}
		else
		{
			m_camera->AcquisitionFrameRateEnable.SetValue(false);
			m_camera->TriggerSelector.SetValue(TriggerSelector_AcquisitionStart);
			m_camera->TriggerMode.SetValue(TriggerMode_On);
			m_camera->AcquisitionFrameCount.SetValue(1);
			m_camera->TriggerSource.FromString(str.c_str());
			return true;
		}
	}
	catch (const GenericException& e)
	{
		// Error handling.
		LOGE("Pylon ERROR:{} ", e.GetDescription());
		return false;
	}
	return false;
}

bool BaslerCamera::getTriggerList(std::list<std::string>& list)
{
	StringList_t settableValues;
	try
	{ 
		if (!m_camera)
		{
			return false;
		}
		if (!m_camera->IsOpen())
		{
			m_camera->Open();
		}
		list.clear();
		m_camera->TriggerSource.GetSettableValues(settableValues);
		for (int i = 0; i < settableValues.size(); ++i)
		{
			list.push_back(settableValues[i].c_str());
		}
		m_camera->Close();
	}
	catch (const GenericException& e)
	{
		// Error handling.
		LOGE("Pylon ERROR:{} ", e.GetDescription());
		return false;
	}

	LOGD("device_vendor_name:{} Get Trigger List", m_camera->GetDeviceInfo().GetSerialNumber());
	return false;
}


bool BaslerCamera::SetCallback(cameramanager::CallbackImage func, void * p)
{
	pUser = p;
	callbackfunc = func;
	return true;
}

bool BaslerCamera::getImage(UINT_PTR &data)
{
	UINT_PTR p = (UINT_PTR)(pylonBGRdata.GetBuffer());
	data = (UINT_PTR)(p);
	return true;
}

}  // namespace smartmore
extern "C" __declspec(dllexport) cameramanager::ICameraDevice * __stdcall CreateExportCameraObj()
{
	return new smartmore::BaslerCamera();
};