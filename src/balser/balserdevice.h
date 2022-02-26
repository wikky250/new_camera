#pragma once

#include <condition_variable>
#include <mutex>


#include "baslercamera.h"
#include "icameradevice.h"


class BalserDevice : public cameramanager::ICameraDevice
{
public:
    static bool enumCamera(std::vector<cameramanager::DeviceInfo> &devices);

    BalserDevice(const cameramanager::DeviceInfo &device_info, const std::string &config_file);
    virtual ~BalserDevice();

    virtual bool isConnected() override;

    virtual bool initCamera() override;
    virtual bool registerEvent(uint16_t event) override;
    virtual bool waitForEvent(uint16_t event, int32_t timeout_millseconds) override;
    virtual bool openCamera() override;
    virtual bool startGrabbing(bool extern_trigger) override;
    virtual bool triggerOne(cv::Mat &image) override;
    virtual bool stopGrabbing() override;
    virtual bool closeCamera() override;

    virtual const cameramanager::DeviceInfo &getDeviceInfo() override;

    virtual bool getExposureTime(double &us_count) override;
    virtual bool setExposureTime(const double us_count) override;
    virtual bool getGain(double &gain) override;
    virtual bool setGain(const double gain) override;
    virtual bool getTriggerDelay(double &value) override;
    virtual bool setTriggerDelay(const double value) override;

    virtual bool setShaftEncoderParam(const uint32_t drop, const uint32_t multiply) override;
    virtual bool getShaftEncoderParam(uint32_t &drop, uint32_t &multiply) override;

    virtual bool setCropParam(const uint32_t top, const uint32_t left, const uint32_t width, const uint32_t height) override;
    virtual bool getCropParam(uint32_t &top, uint32_t &left, uint32_t &width, uint32_t &height) override;
private:
    void triggerCallback(void *cb_param, const cv::Mat &image);

    std::string m_config_file;
    cameramanager::DeviceInfo m_device_info;

    std::shared_ptr<smartmore::BaslerCamera> m_camera;
};
