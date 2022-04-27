#include "icameradevice.h"
using namespace std;
namespace smartmore
{
class  LocalCamera:public cameramanager::ICameraDevice
{
public:
	char* BGRdata = nullptr;
	std::vector<std::string>::iterator m_currentReadIterator;
	int w, h, c;
	int m_fps = 5;
	bool m_grabbing = false;
	bool m_stop = false;
	void* pUser = nullptr;
	int m_trigger = -1;

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

	std::vector<std::string> m_vFilePath;
private:
    cameramanager::DeviceInfo m_device_info;
    bool m_extern_trigger = false;

	string m_sfolderPath;

};

}  // namespace smartmore
extern "C" __declspec(dllexport) cameramanager::ICameraDevice * __stdcall CreateExportCameraObj();