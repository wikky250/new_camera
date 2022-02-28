#pragma once

#include "opencv2/opencv.hpp"
#include <condition_variable>
#include <memory>
#include <stdint.h>
#include <thread>


namespace cameramanager
{
#define CameraEventFrameStart (0x01 << 0)
#define CameraEventLine0RisingEdge (0x01 << 1)
#define CameraEventReadOut (0x01 << 2)


enum class DeviceManufacturer
{
    Unknown = 0,
    Emulator = 1,
    Hik = 2,
    Dalsa = 3,
    Basler = 4,
    Keyence = 5,
    Dahua = 6,
    Daheng = 7,
    Baumer = 8,
    AlliedVision = 9
};

enum class TransLayerType
{
    Gige = 0,
    Usb = 1,
    CameraLink = 2,
    Unknown = 3
};

struct DeviceInfo
{
    std::string model_name;
    std::string serial_number;
    DeviceManufacturer device_type;
    TransLayerType layer_type;
    std::string full_name;

    // Dalsa板卡信息
    int board_server_index = -1;
    std::string board_server_name;
    int board_resouce_index = -1;
    std::string board_resouce_name;

    // Dalsa相机信息
    int camera_server_index = -1;
    std::string camera_server_name;
    int camera_resource_index = -1;
    std::string camera_resouce_name;

    std::string getCameraId() const
    {
        return model_name + "_" + serial_number;
    }
};

class ICameraDevice
{
public:
    virtual ~ICameraDevice() {}

    virtual bool initCamera(const char*) = 0;
    //virtual bool registerEvent(uint16_t event) = 0;
    //virtual bool waitForEvent(uint16_t event, int32_t timeout_millseconds) = 0;
    virtual bool openCamera() = 0;
    virtual bool isConnected() = 0;
    virtual bool startGrabbing(bool extern_trigger = false) = 0;
    virtual bool triggerOne(cv::Mat &image) = 0;
    virtual bool stopGrabbing() = 0;
    virtual bool closeCamera() = 0;
    virtual const DeviceInfo &getDeviceInfo() = 0;

    virtual bool getExposureTime(double &us_count) = 0;
    virtual bool setExposureTime(const double us_count) = 0;

    virtual bool getGain(double &gain) = 0;
    virtual bool setGain(const double gain) = 0;

    virtual bool getTriggerDelay(double &value) = 0;
    virtual bool setTriggerDelay(const double value) = 0;

    //virtual bool setShaftEncoderParam(const uint32_t drop, const uint32_t multiply) = 0;
    //virtual bool getShaftEncoderParam(uint32_t &drop, uint32_t &multiply) = 0;

    //virtual bool setCropParam(const uint32_t top, const uint32_t left, const uint32_t width, const uint32_t height) = 0;
    //virtual bool getCropParam(uint32_t &top, uint32_t &left, uint32_t &width, uint32_t &height) = 0;
};

typedef ICameraDevice* (__stdcall* pExportCamera)();
}  // namespace cameramanager

