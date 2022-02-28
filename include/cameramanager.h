/****************************************************************************
 *  @file     cameramanager.h
 *  @brief    相机设备管理员
 *  @author   lichao.xu
 *  @email    lichao.xu@smartmore.com
 *  @version
 *  @date     2021.7.7
 *  Copyright (c) 2021 SmartMore All rights reserved.
 ****************************************************************************/

#pragma once
#define BUILD_BALSER_SDK

#include "icameradevice.h"
#include <memory>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <thread>
#include <unordered_map>


namespace cameramanager
{
#define INDUSTRIAL_CAMERA_MANAMGER_VERSION_MAJOR 1
#define INDUSTRIAL_CAMERA_MANAMGER_VERSION_MINOR 1

class __declspec(dllexport) CameraManager
{
public:
    static CameraManager &getInstance();
    virtual ~CameraManager();
    bool enumCamera(std::vector<DeviceInfo> &devices);
    bool createCameraDevice(const DeviceInfo &device_info, const std::string &config_file);
    std::shared_ptr<ICameraDevice> getCameraDevice(const std::string camera_id);
    bool destryoAllCameras();

    int getVersionMajor();
    int getVersionMinor();
    bool isHikSDKSupproted();
    bool isBalserSDKSupproted();
    bool isDalsaSDKSupproted();

private:
    CameraManager();
    CameraManager(const CameraManager &) = delete;
    CameraManager &operator=(const CameraManager &) = delete;

private:
    std::unordered_map<std::string, std::shared_ptr<ICameraDevice>> m_camera_devices;
};
}  // namespace cameramanager