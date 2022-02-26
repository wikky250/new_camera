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

int BaslerCamera::enumCamera(std::vector<cameramanager::DeviceInfo> &device_list)
{
    LOGI("+");
    LOGI("Compile PYLON_VERSION:{}.{}.{}.{}", PYLON_VERSION_MAJOR, PYLON_VERSIONSTRING_MINOR,
         PYLON_VERSIONSTRING_SUBMINOR, PYLON_VERSION_BUILD);
    LOGI("Runtime PYLON_VERSION:{}", VersionInfo::getVersionString());
    // Before using any pylon methods, the pylon runtime must be initialized.
    PylonInitialize();

    int exitCode = 0;
    try
    {
        DeviceInfoList_t info_list;
        CTlFactory::GetInstance().EnumerateDevices(info_list);
        for (auto &it = info_list.begin(); it != info_list.end(); ++it)
        {
            const std::string device_vendor_name = it->GetVendorName();
            LOGD("device_vendor_name:{}", device_vendor_name);
            if (device_vendor_name != "Basler")
            {
                continue;
            }

            cameramanager::DeviceInfo device_info;
            device_info.device_type = cameramanager::DeviceManufacturer::Basler;
            device_info.model_name = it->GetModelName();
            device_info.serial_number = it->GetSerialNumber();
            device_info.full_name = it->GetFullName();

            const std::string tlt_type_str = it->GetTLType().c_str();
            LOGD("tlt_type_str:{}", tlt_type_str);

            if (tlt_type_str == Pylon::TLType::TLTypeUSB)
            {
                device_info.layer_type = cameramanager::TransLayerType::Usb;
            }
            else if (tlt_type_str == Pylon::TLType::TLTypeCL)
            {
                device_info.layer_type = cameramanager::TransLayerType::CameraLink;
            }
            else if (tlt_type_str == Pylon::TLType::TLTypeGigE)
            {
                device_info.layer_type = cameramanager::TransLayerType::Gige;
            }
            else
            {
                device_info.layer_type = cameramanager::TransLayerType::Unknown;
                LOGI("Unknown type:{}", tlt_type_str);
            }
            device_list.push_back(device_info);
        }
    }
    catch (const GenericException &e)
    {
        // Error handling.
        std::cerr << "An exception occurred." << std::endl << e.GetDescription() << std::endl;
        exitCode = 1;
    }

    // Releases all pylon resources.
    PylonTerminate();
    return exitCode;
}

BaslerCamera::BaslerCamera() {}

BaslerCamera::~BaslerCamera()
{
    m_camera.reset();
    // Releases all pylon resources.
    PylonTerminate();
}

bool BaslerCamera::initCamera()
{
    bool ret = false;
    // CreateDevice by fullname
    m_camera = std::make_shared<CBaslerUniversalInstantCamera>(
        CTlFactory::GetInstance().CreateDevice(CDeviceInfo().SetFullName(m_device_info.full_name.c_str())));
    do
    {
        if (m_camera)
        {
            // Register the standard configuration event handler for enabling software triggering.
            // The software trigger configuration handler replaces the default configuration
            // as all currently registered configuration handlers are removed by setting the registration mode to
            // RegistrationMode_ReplaceAll.
            // m_camera->RegisterConfiguration(new CSoftwareTriggerConfiguration, RegistrationMode_Append,
            // Cleanup_Delete); For demonstration purposes only, register another image event handler.
            // m_camera->RegisterImageEventHandler(m_image_event_handler.get(), RegistrationMode_Append, Cleanup_None);
            // Camera event processing must be activated first, the default is off.
            // m_camera->GrabCameraEvents = true;
            ret = true;
        }
    } while (0);
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

    m_camera.reset();
    return true;
}

bool BaslerCamera::openCamera(const char *config_file = nullptr)
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

bool BaslerCamera::startGrabbing(bool extern_trigger)
{
    m_extern_trigger = extern_trigger;
    LOGD("m_extern_trigger:{}", m_extern_trigger);
    m_camera->TriggerMode.TrySetValue(TriggerModeEnums::TriggerMode_On);
    m_camera->TriggerSource.TrySetValue(TriggerSourceEnums::TriggerSource_Software);

    bool ret = false;
    if (m_camera && m_camera->IsOpen() && !m_camera->IsGrabbing())
    {
        try
        {
            m_camera->StartGrabbing();
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

bool BaslerCamera::stopGrabbing()
{
    bool ret = false;
    if (m_camera && m_camera->IsOpen() && m_camera->IsGrabbing())
    {
        try
        {
            m_camera->StopGrabbing();
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

bool BaslerCamera::registerImageCallBack(std::function<void(void *, const cv::Mat &)> callback, void *cb_param)
{
    // m_image_event_handler->m_frame_callback = callback;
    // m_image_event_handler->m_cb_param = cb_param;
    return false;
}

bool BaslerCamera::triggerOne(cv::Mat &image)
{
    bool ret = false;
    if (m_camera->IsGrabbing())
    {
        try
        {
            if (!m_extern_trigger)
            {
                // Execute the software trigger. Wait up to 1000 ms for the camera to be ready for trigger.
                if (m_camera->WaitForFrameTriggerReady(1000, TimeoutHandling_ThrowException))
                {
                    m_camera->ExecuteSoftwareTrigger();
                }
                LOGD("Software trigger");
            }
            else
            {
                LOGD("Wait for extern trigger");
            }

            // Retrieve grab results and notify the camera event and image event handlers.
            CGrabResultPtr ptrGrabResult;
            m_camera->RetrieveResult(INFINITE, ptrGrabResult, TimeoutHandling_ThrowException);
            // XXXXX : Nothing to do here with the grab result, the grab results are handled by the registered event
            // handler.

            // Create a target image.
            if (ptrGrabResult.IsValid() && ptrGrabResult->GrabSucceeded())
            {  // Create the converter and set parameters.
                CImageFormatConverter converter;
                converter.OutputPixelFormat = PixelType_BGR8packed;
                // Now we can check if conversion is required.
                if (converter.ImageHasDestinationFormat(ptrGrabResult))
                {
                    // No conversion is needed. It can be skipped for saving processing
                    // time.
                    image = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3,
                                    ptrGrabResult->GetBuffer())
                                .clone();
                }
                else
                {
                    CPylonImage targetImage;
                    // Conversion is needed.
                    converter.Convert(targetImage, ptrGrabResult);
                    image = cv::Mat(targetImage.GetHeight(), targetImage.GetWidth(), CV_8UC3, targetImage.GetBuffer())
                                .clone();
                }

                ret = true;
            }
        }
        catch (const std::exception &e)
        {
            LOGE("{}", e.what());
        }
    }

    return ret;
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
        gain = (float)m_camera->Gain.GetValue();
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

}  // namespace smartmore