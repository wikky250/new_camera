#include "balserdevice.h"
#include "logger.h"
#include <functional>


bool BalserDevice::enumCamera(std::vector<cameramanager::DeviceInfo> &devices)
{
    smartmore::BaslerCamera::enumCamera(devices);

    return true;
}

BalserDevice::BalserDevice(const cameramanager::DeviceInfo &device_info, const std::string &config_file)
    : m_device_info(device_info), m_config_file(config_file), m_camera(nullptr)
{
    LOGD("");
    m_camera = std::make_shared<smartmore::BaslerCamera>(device_info);
}

BalserDevice::~BalserDevice()
{
    LOGD("");
    m_camera.reset();
}

bool BalserDevice::isConnected()
{
    LOGD("");
    return m_camera ? m_camera->isConnected() : false;
}

bool BalserDevice::initCamera()
{
    LOGD("");
    // std::function<void(void*, const cv::Mat&)> callback = std::bind(&BalserDevice::triggerCallback, this,
    // std::placeholders::_1, std::placeholders::_2); m_camera->registerImageCallBack(callback, this);
    // m_camera->registerImageCallBack(std::bind(&BalserDevice::triggerCallback, this), this);
    return m_camera ? m_camera->initCamera() : false;
}

bool BalserDevice::registerEvent(uint16_t event)
{
    return false;
}

bool BalserDevice::waitForEvent(uint16_t event, int32_t timeout_millseconds)
{
    return false;
}

bool BalserDevice::openCamera()
{
    LOGD("");
    if (m_camera)
    {
        return (m_camera->openCamera(m_config_file.empty() ? nullptr : m_config_file.c_str()));
    }
    return false;
}

bool BalserDevice::startGrabbing(bool extern_trigger)
{
    LOGD("");
    return m_camera ? m_camera->startGrabbing(extern_trigger) : false;
}

bool BalserDevice::triggerOne(cv::Mat &image)
{
    LOGD("");
    return m_camera ? m_camera->triggerOne(image) : false;
}

bool BalserDevice::stopGrabbing()
{
    LOGD("");
    return m_camera ? m_camera->stopGrabbing() : false;
}

bool BalserDevice::closeCamera()
{
    LOGD("");
    return m_camera ? m_camera->closeCamera() : false;
}

const cameramanager::DeviceInfo &BalserDevice::getDeviceInfo()
{
    LOGD("");
    return m_device_info;
}

void BalserDevice::triggerCallback(void *cb_param, const cv::Mat &image) {}

bool BalserDevice::getExposureTime(double &us_count)
{
    return m_camera ? m_camera->getExposureTime(us_count) : false;
}

bool BalserDevice::setExposureTime(const double us_count)
{
    return m_camera ? m_camera->setExposureTime(us_count) : false;
}

bool BalserDevice::getGain(double &gain)
{
    return m_camera ? m_camera->getGain(gain) : false;
}

bool BalserDevice::setGain(const double gain)
{
    return m_camera ? m_camera->setGain(gain) : false;
}

bool BalserDevice::getTriggerDelay(double &value)
{
    return m_camera ? m_camera->getTriggerDelay(value) : false;
}

bool BalserDevice::setTriggerDelay(const double value)
{
    return m_camera ? m_camera->setTriggerDelay(value) : false;
}

bool BalserDevice::setShaftEncoderParam(const uint32_t drop, const uint32_t multiply)
{
    return false;
}

bool BalserDevice::getShaftEncoderParam(uint32_t &drop, uint32_t &multiply)
{
    return false;
}

bool BalserDevice::setCropParam(const uint32_t top, const uint32_t left, const uint32_t width, const uint32_t height)
{
    return false;
}

bool BalserDevice::getCropParam(uint32_t &top, uint32_t &left, uint32_t &width, uint32_t &height)
{
    return false;
}
