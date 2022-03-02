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
    //m_camera->reset();
    // Releases all pylon resources.
    PylonTerminate();
}

bool BaslerCamera::initCamera(const char* cameranumber)
{
    bool ret = false;

	if (m_battach)
	{
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
			m_battach = true;
		}
	}
	catch (const GenericException& e)
	{
		// Error handling.
		std::cerr << "An exception occurred." << std::endl << e.GetDescription() << std::endl;
		exitCode = 1;
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
		cam->RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
		converter.OutputBitAlignment = OutputBitAlignment_MsbAligned;
		converter.Convert(basler->pylonBGRdata, ptrGrabResult);
		basler->callbackfunc(basler->pUser);
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
			m_camera->Open();
			pylonBGRdata = CPylonImage::Create(PixelType_RGB8packed, m_camera->Width.GetValue(), m_camera->Height.GetValue());
            m_camera->StartGrabbing();

			std::thread cam1(catchIncircle, m_camera,this);
			cam1.detach();
        }
        catch (const std::exception &e)
        {
            ret = false;
            std::cerr << e.what() << '\n';
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

bool BaslerCamera::getCameraInt(cameramanager::CameraInt param, int & ret_value)
{
	bool ret = false;
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
			ret_value =  m_camera->AcquisitionFrameRate.GetMax();
		}
		if (m_camera->IsGigE())
		{
			ret_value =  m_camera->AcquisitionFrameRateAbs.GetMax();
		}
		ret = true;
		break;
	case cameramanager::EXPOUSETIME:
		if (m_camera->IsGigE())
		{
			ret_value =  m_camera->ExposureTimeAbs.GetValue();
		}
		if (m_camera->IsUsb())
		{
			ret_value =  m_camera->ExposureTime.GetValue();
		}
		ret = true;
		break;
	case cameramanager::EXPOUSETIMEMAX:
		if (m_camera->IsGigE())
		{
			ret_value =  m_camera->ExposureTimeAbs.GetMax();
		}
		if (m_camera->IsUsb())
		{
			ret_value =  m_camera->ExposureTime.GetMax();
		}
		ret = true;
		break;
	case cameramanager::EXPOUSETIMEMIN:
		if (m_camera->IsGigE())
		{
			ret_value =  m_camera->ExposureTimeAbs.GetMax();
		}
		if (m_camera->IsUsb())
		{
			ret_value =  m_camera->ExposureTime.GetMax();
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
	return false;
}

bool BaslerCamera::setCameraInt(cameramanager::CameraInt, int)
{
	return false;
}


bool BaslerCamera::getExposureTime(double &us_count)
{
    bool ret = false;
    do
    {
        if (!m_camera)
        {
            break;
        }

        ret = m_camera->ExposureTime.IsReadable();
        if (!ret)
        {
            break;
        }
        us_count = (float)m_camera->ExposureTime.GetValue();

        ret = true;
    } while (0);

    return ret;
}

bool BaslerCamera::setExposureTime(const double us_count)
{
    bool ret = false;
    do
    {
        if (!m_camera)
        {
            break;
        }

        ret = m_camera->ExposureTime.IsWritable();
        if (!ret)
        {
            break;
        }

        ret = m_camera->ExposureAuto.TrySetValue(ExposureAuto_Off);
        if (!ret)
        {
            break;
        }

        ret = m_camera->ExposureTime.TrySetValue(us_count);
        if (!ret)
        {
            break;
        }

        ret = true;
    } while (0);

    return ret;
}

bool BaslerCamera::getGain(double &gain)
{
    bool ret = false;
    do
    {
        if (!m_camera)
        {
            break;
        }

        ret = m_camera->Gain.IsReadable();
        if (!ret)
        {
            break;
        }
        //gairet_value =  (float)m_camera->Gain.GetValue();
        ret = true;
    } while (0);
    return ret;
}

bool BaslerCamera::setGain(const double gain)
{
    bool ret = false;
    do
    {
        if (!m_camera)
        {
            break;
        }

        ret = m_camera->GainAuto.TrySetValue(GainAuto_Off);
        if (!ret)
        {
            break;
        }

        ret = m_camera->Gain.TrySetValue(gain);
        if (!ret)
        {
            break;
        }
        ret = true;
    } while (0);

    return ret;
}

bool BaslerCamera::getTriggerDelay(double &value)
{
    bool ret = false;
    do
    {
        if (!m_camera)
        {
            break;
        }

        ret = m_camera->TriggerDelay.IsReadable();
        if (!ret)
        {
            break;
        }
        value = (float)m_camera->TriggerDelay.GetValue();
        ret = true;
    } while (0);
    return ret;
}

bool BaslerCamera::setTriggerDelay(const double value)
{
    bool ret = false;
    do
    {
        if (!m_camera)
        {
            break;
        }

        ret = m_camera->TriggerDelay.IsWritable();
        if (!ret)
        {
            break;
        }

        ret = m_camera->TriggerDelay.TrySetValue(value);
        if (!ret)
        {
            break;
        }

        ret = true;
    } while (0);

    return ret;
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