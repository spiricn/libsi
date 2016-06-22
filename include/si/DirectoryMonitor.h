#ifndef DIRECTORYMONITOR_H
#define DIRECTORYMONITOR_H

#include <pthread.h>

namespace si{

class IDirectoryMonitorListener;

class DirectoryMonitor{
public:
	void monitor(const char* path, IDirectoryMonitorListener* listener);

private:
	void mainLoop();

	static void* sMainLoop(void* arg);

private:
	IDirectoryMonitorListener* mListener;
	pthread_t mThread;
	int mWd;
	int mFd;

}; // </DirectoryMonitor>

}; // </si>

#endif // </DIRECTORYMONITOR_H>
