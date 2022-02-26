/****************************************************************************
 *  @file     cameramanager.cpp
 *  @brief    相机设备管理员
 *  @author   lichao.xu
 *  @email    lichao.xu@smartmore.com
 *  @version
 *  @date     2021.7.7
 *  Copyright (c) 2021 SmartMore All rights reserved.
 ****************************************************************************/

#include "cameramanager.h"
#include "logger.h"

#ifdef BUILD_HIK_SDK
#include "hik/hikdevice.h"
#endif

#ifdef BUILD_BALSER_SDK
#include "balser/balserdevice.h"
#endif

#ifdef BUILD_DALSA_SDK
#include "dalsa/dalsadevice.h"
#endif

#ifdef BUILD_BAUMER_SDK
#include "baumer/baumerdevice.h"
#endif

#ifdef BUILD_DAHUA_SDK
#include "dahua/dahuadevice.h"
#endif

namespace cameramanager
{
CameraManager &CameraManager::getInstance()
{
    static CameraManager s_cameras_manager;
    return s_cameras_manager;
}

CameraManager::CameraManager() {}

CameraManager::~CameraManager() {}

bool CameraManager::enumCamera(std::vector<DeviceInfo> &devices)
{
    devices.clear();

#ifdef BUILD_HIK_SDK
    {
        std::vector<DeviceInfo> hik_devices;
        HikDevice::enumCamera(hik_devices);
        devices.insert(std::end(devices), std::begin(hik_devices), std::end(hik_devices));
    }
#endif

#ifdef BUILD_BALSER_SDK
    {
        std::vector<DeviceInfo> balser_devices;
        BalserDevice::enumCamera(balser_devices);
        devices.insert(std::end(devices), std::begin(balser_devices), std::end(balser_devices));
    }
#endif

#ifdef BUILD_DALSA_SDK
    {
        std::vector<DeviceInfo> dalsa_devices;
        DalsaDevice::enumCamera(dalsa_devices);
        devices.insert(std::end(devices), std::begin(dalsa_devices), std::end(dalsa_devices));
    }
#endif

#ifdef BUILD_BAUMER_SDK
    {
        std::vector<DeviceInfo> baumer_devices;
        BaumerDevice::enumCamera(baumer_devices);
        devices.insert(std::end(devices), std::begin(baumer_devices), std::end(baumer_devices));
    }
#endif

#ifdef BUILD_DAHUA_SDK
    {
        std::vector<DeviceInfo> dahua_devices;
        DahuaDevice::enumCamera(dahua_devices);
        devices.insert(std::end(devices), std::begin(dahua_devices), std::end(dahua_devices));
    }
#endif
    return true;
}

std::shared_ptr<ICameraDevice> CameraManager::getCameraDevice(const std::string camera_id)
{
    if (m_camera_devices.find(camera_id) != std::end(m_camera_devices))
    {
        return m_camera_devices[camera_id];
    }
    return nullptr;
}

bool CameraManager::createCameraDevice(const DeviceInfo &device_info, const std::string &config_file)
{
    const std::string camera_id = device_info.getCameraId();
    std::shared_ptr<ICameraDevice> camera = nullptr;
    switch (device_info.device_type)
    {
#ifdef BUILD_HIK_SDK
        case DeviceManufacturer::Hik:
        case DeviceManufacturer::Daheng:
        case DeviceManufacturer::AlliedVision:
            camera = std::make_shared<HikDevice>(device_info, config_file);
            break;
#endif

#ifdef BUILD_BALSER_SDK
        case DeviceManufacturer::Basler:
            camera = std::make_shared<BalserDevice>(device_info, config_file);
            break;
#endif

#ifdef BUILD_DALSA_SDK
        case DeviceManufacturer::Dalsa:
            camera = std::make_shared<DalsaDevice>(device_info, config_file);
            break;
#endif

#ifdef BUILD_BAUMER_SDK
        case DeviceManufacturer::Baumer:
            camera = std::make_shared<BaumerDevice>(device_info, config_file);
            break;
#endif

#ifdef BUILD_DAHUA_SDK
        case DeviceManufacturer::Dahua:
            camera = std::make_shared<DahuaDevice>(device_info, config_file);
            break;
#endif

        case DeviceManufacturer::Unknown:
        default:
            LOGE("not supported: {}", camera_id);
            break;
    }

    if (camera)
    {
        m_camera_devices.insert({camera_id, camera});
        return true;
    }
    return false;
}

bool CameraManager::destryoAllCameras()
{
    for (auto &item : m_camera_devices)
    {
        item.second->closeCamera();
    }
    m_camera_devices.clear();
    return false;
}

int CameraManager::getVersionMajor()
{
    return INDUSTRIAL_CAMERA_MANAMGER_VERSION_MAJOR;
}

int CameraManager::getVersionMinor()
{
    return INDUSTRIAL_CAMERA_MANAMGER_VERSION_MINOR;
}

bool CameraManager::isHikSDKSupproted()
{
#ifdef BUILD_HIK_SDK
    return true;
#else
    return false;
#endif
}

bool CameraManager::isBalserSDKSupproted()
{
#ifdef BUILD_BALSER_SDK
    return true;
#else
    return false;
#endif
}

bool CameraManager::isDalsaSDKSupproted()
{
#ifdef BUILD_DALSA_SDK
    return true;
#else
    return false;
#endif
}

}  // namespace cameramanager