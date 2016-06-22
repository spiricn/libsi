#include "si/DirectoryMonitor.h"
#include "si/IDirectoryMonitorListener.h"
#include "si/Common.h"

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <linux/input.h>
#include <unistd.h>
#include <sys/inotify.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "libsi-DirectoryMonitor"

namespace si{

static const int kEVENT_SIZE  = sizeof (struct inotify_event);

static const int kEVENT_BUF_LEN  = 1024 * ( kEVENT_SIZE + 16 );

void DirectoryMonitor::monitor(const char* path, IDirectoryMonitorListener* listener){
	mListener = listener;

	// Initialize library
	mFd = inotify_init();
	if(mFd < 0){
		ALOGE("inotify_init failed, %d", mFd);
		return;
	}

	// Start watching directory
	mWd = inotify_add_watch(mFd, path, IN_CREATE | IN_DELETE );
	if(mWd < 0){
		ALOGE( "inotify_add_watch failed, %d", mWd);
		return;
	}

	pthread_create(&mThread, NULL, &DirectoryMonitor::sMainLoop, static_cast<void*>(this));

	ALOGD("Watching directory %s ..",  path);
}

void DirectoryMonitor::mainLoop(){
	char* buffer = new char[kEVENT_BUF_LEN];

	while(true){
		int length = read(mFd, buffer, kEVENT_BUF_LEN);

		if(length < 0){
			ALOGE("read failed; %d", length);
			break;
		}

		int i =0;

		while(i < length){
			struct inotify_event *event = (struct inotify_event *) &buffer[i];

			if(event->len && !(event->mask & IN_ISDIR)){
				if(event->mask & IN_CREATE ){
					if(mListener){
						mListener->onFileCreated(event->name);
					}
				}
				else if(event->mask & IN_DELETE){
					mListener->onFileDeleted(event->name);
				}
			}

			i += kEVENT_SIZE + event->len;
		}
	}

	delete[] buffer;

	inotify_rm_watch(mFd, mWd);

	close(mFd);
}

void* DirectoryMonitor::sMainLoop(void* arg){
	DirectoryMonitor* thiz = static_cast<DirectoryMonitor*>(arg);

	thiz->mainLoop();

	return NULL;
}

}; // </si>
