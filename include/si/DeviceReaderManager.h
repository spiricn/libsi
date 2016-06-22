#ifndef DEVICEREADERMANAGER_H_
#define DEVICEREADERMANAGER_H_

#include "si/IDeviceReaderListener.h"
#include "si/IDirectoryMonitorListener.h"

#include <pthread.h>

namespace si{

class DeviceReader;
class DeviceEvent;
class DirectoryMonitor;

class DeviceReaderManager : public IDeviceReaderListener, public IDirectoryMonitorListener{
public:
	void registerListener(IDeviceReaderListener* listener);

	static void initialize();

	static DeviceReaderManager* getInstance();

private:
	void onFileDeleted(const  char* name);

	void onFileCreated(const  char* name);

	void onDeviceEvent(const DeviceEvent& event);

	DeviceReaderManager();

	void addReader(const char* device);

	void mainLoop();

	static void* sMainLoop(void* arg);

	DeviceReader* findReader(const char* device);

private:
	static const int kMAX_DEV_READERS = 64;
	DeviceReader* mDevReaders[kMAX_DEV_READERS];
	pthread_mutex_t mMutex;
	IDeviceReaderListener* mListener;
	DirectoryMonitor* mDirMonitor;

private:
	static DeviceReaderManager* sInstance;

}; // </DeviceReaderManager>

}; // </si>

#endif  // </DEVICEREADERMANAGER_H_>
