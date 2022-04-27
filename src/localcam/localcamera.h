#include "icameradevice.h"
using namespace std;
namespace smartmore
{
class  LocalCamera:public cameramanager::ICameraDevice
{
public:
	void* pUser = nullptr;
	cameramanager::CallbackImage callbackfunc = nullptr;
	LocalCamera();
    LocalCamera(const cameramanager::DeviceInfo &device_info);
    virtual ~LocalCamera();
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

	virtual bool getCurrentTrigger(std::string& str);
	virtual bool setCurrentTrigger(std::string str);
	virtual bool getTriggerList(std::list<std::string>& list);

	virtual bool triggerOnce();

	virtual bool getCurrentFormat(std::string &str);
	virtual bool setCurrentFormat(std::string str);
	virtual bool getFormatList(std::list<std::string>& list);

	virtual bool SetCallback(cameramanager::CallbackImage func, void* p);
	virtual bool getImage(UINT_PTR&);

private:
    cameramanager::DeviceInfo m_device_info;
    bool m_extern_trigger = false;
	std::vector<std::string> m_vFilePath;

	string m_sfolderPath;

};

}  // namespace smartmore
extern "C" __declspec(dllexport) cameramanager::ICameraDevice * __stdcall CreateExportCameraObj();