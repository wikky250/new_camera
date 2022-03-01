#include "icameradevice.h"
// Include files to use the pylon API.
#include <pylon/PylonIncludes.h>

// Include file to use pylon universal instant camera parameters.
#include <pylon/BaslerUniversalInstantCamera.h>

// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using pylon universal instant camera parameters.
using namespace Basler_UniversalCameraParams;

namespace smartmore
{
class  BaslerCamera:public cameramanager::ICameraDevice
{
public:
	CPylonImage pylonBGRdata;
	void* pUser = nullptr;
	cameramanager::CallbackImage callbackfunc = nullptr;
	BaslerCamera();
    BaslerCamera(const cameramanager::DeviceInfo &device_info);
    virtual ~BaslerCamera();
    virtual const cameramanager::DeviceInfo &getDeviceInfo();
    virtual bool initCamera(const char* cameranumber);
    virtual bool isConnected();
    virtual bool uninitCamera();
    virtual bool openCamera();
    virtual bool closeCamera();
    virtual bool startGrabbing();
    virtual bool stopGrabbing();

	virtual bool getCameraInt(cameramanager::CameraInt, int &);
	virtual bool setCameraInt(cameramanager::CameraInt, int);

    virtual bool getExposureTime(double &us_count);
    virtual bool setExposureTime(const double us_count);
    virtual bool getGain(double &gain);
    virtual bool setGain(const double gain);
    virtual bool getTriggerDelay(double &value);
    virtual bool setTriggerDelay(const double value);

	virtual bool SetCallback(cameramanager::CallbackImage func, void* p);
	virtual bool getImage(UINT_PTR&);

private:
    cameramanager::DeviceInfo m_device_info;
    std::shared_ptr<CBaslerUniversalInstantCamera> m_camera;
    bool m_extern_trigger = false;

};

}  // namespace smartmore
extern "C" __declspec(dllexport) cameramanager::ICameraDevice * __stdcall CreateExportCameraObj();