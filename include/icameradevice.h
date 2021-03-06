#pragma once

#include <condition_variable>
#include <memory>
#include <string>
#include <list>
#include <stdint.h>
#include <thread>
#include <chrono>
#include <future>
#include <intsafe.h>


namespace cameramanager
{
#define CameraEventFrameStart (0x01 << 0)
#define CameraEventLine0RisingEdge (0x01 << 1)
#define CameraEventReadOut (0x01 << 2)

enum CameraInt
{
	WIDTH,
	WIDTHMAX,
	HEIGHT,
	HEIGHTMAX,
	IMAGECHANNEL,
	OFFSETX,
	OFFSETXMAX,
	OFFSETY,
	OFFSETYMAX,
	FRAMES,
	FRAMESMAX,
	EXPOUSETIME,
	EXPOUSETIMEMAX,
	EXPOUSETIMEMIN,
	CAMIMGCOUNT
};

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
	int width;
	int height;
	int channel;
	int offsetx;
	int offsety;
	int frames;
	int expousetime;
};

using CallbackImage = void(*) (void*);
class ICameraDevice
{
public:
    virtual ~ICameraDevice() {}

    virtual bool initCamera(const char*) = 0;
    virtual bool startGrabbing() = 0;
    virtual bool stopGrabbing() = 0;
    virtual const DeviceInfo &getDeviceInfo() = 0;


	virtual bool getCameraInt(CameraInt, int &) = 0;
	virtual bool setCameraInt(CameraInt, int) = 0;

	virtual bool getCurrentTrigger(std::string&) = 0;
	virtual bool setCurrentTrigger(std::string) = 0;
	virtual bool getTriggerList(std::list<std::string>& list) = 0;

	virtual bool getCurrentFormat(std::string &str) = 0;
	virtual bool setCurrentFormat(std::string str) = 0;
	virtual bool getFormatList(std::list<std::string>& list) = 0;

	virtual bool triggerOnce() = 0;

	virtual bool SetCallback(CallbackImage func, void * p) = 0;
	virtual bool getImage(UINT_PTR&) = 0;

    //virtual bool setShaftEncoderParam(const uint32_t drop, const uint32_t multiply) = 0;
    //virtual bool getShaftEncoderParam(uint32_t &drop, uint32_t &multiply) = 0;

    //virtual bool setCropParam(const uint32_t top, const uint32_t left, const uint32_t width, const uint32_t height) = 0;
    //virtual bool getCropParam(uint32_t &top, uint32_t &left, uint32_t &width, uint32_t &height) = 0;

};

typedef ICameraDevice* (__stdcall* pExportCamera)();
}  // namespace cameramanager

