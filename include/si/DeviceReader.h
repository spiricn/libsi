#ifndef DEVICEREADER_H_
#define DEVICEREADER_H_

#include <pthread.h>

namespace si{

class IDeviceReaderListener;

class DeviceReader{
public:
	DeviceReader(IDeviceReaderListener* listener, const char* path);

	void stop();

	const char* getPath() const;

private:
	void mainLoop();

	static void* sMainLoop(void* arg);

private:
	bool mRunning;
	pthread_t mReadThread;
	int mFd;
	IDeviceReaderListener* mListener;
	char mDevicePath[256];

}; // </DeviceReader>

}; // </si>

#endif // </DEVICEREADER_H_>
