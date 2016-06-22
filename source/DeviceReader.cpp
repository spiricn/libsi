#include "si/DeviceReader.h"
#include "si/DeviceEvent.h"
#include "si/IDeviceReaderListener.h"
#include "si/Common.h"

#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#if ANDROID

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "libsi-DeviceReader"
#endif


namespace si{

DeviceReader::DeviceReader(IDeviceReaderListener* listener, const char* path) : mRunning(true), mFd(0), mListener(listener){
	strcpy(mDevicePath, path);

	mFd = open(mDevicePath, O_RDONLY);

	if(mFd == -1){
		ALOGE("Error opening device \"%s\" %s", mDevicePath, strerror(errno));
		return;
	}

	pthread_create(&mReadThread, NULL, &DeviceReader::sMainLoop, static_cast<void*>(this));
}

const char* DeviceReader::getPath() const{
	return mDevicePath;
}

void DeviceReader::mainLoop(){
	{

		// Send a device added event
		DeviceEvent devEvent;
		memset(&devEvent, 0x00, sizeof(DeviceEvent));

		devEvent.type = DeviceEvent::eTYPE_DEVICE_ADDED;
		strcpy(devEvent.devicePath, mDevicePath);

		mListener->onDeviceEvent(devEvent);
	}

	while(mRunning){
		struct input_event event;
		memset(&event, 0x00, sizeof(struct input_event));

		DeviceEvent devEvent;
		memset(&devEvent, 0x00, sizeof(DeviceEvent));

		int res = read(mFd, &event, sizeof(struct input_event));

		if(res != sizeof(struct input_event)){
			ALOGE("Error reading device \"%s\"\n", mDevicePath);

			mRunning = false;

			break;
		}

		if(event.type == EV_KEY){
			if(event.code == BTN_LEFT){
				devEvent.type = DeviceEvent::eTYPE_MOUSE_CLICK;
				devEvent.data.mouseClick.button = DeviceEvent::eBUTTON_LEFT;
				devEvent.data.mouseClick.down = event.value;
			}
			else if(event.code == BTN_RIGHT){
				devEvent.type = DeviceEvent::eTYPE_MOUSE_CLICK;
				devEvent.data.mouseClick.button = DeviceEvent::eBUTTON_RIGHT;
				devEvent.data.mouseClick.down = event.value;
			}
			else if(event.code == BTN_MIDDLE){
				devEvent.type = DeviceEvent::eTYPE_MOUSE_CLICK;
				devEvent.data.mouseClick.button = DeviceEvent::eBUTTON_MIDDLE;
				devEvent.data.mouseClick.down = event.value;
			}
			else{
				devEvent.type = DeviceEvent::eTYPE_KEYBOARD;
				devEvent.data.keyboard.code = event.code;
				devEvent.data.keyboard.down = event.value;
			}
		}
		else if(event.type == EV_REL){
			if(event.code == REL_X){
				// Horizontal
				devEvent.type = DeviceEvent::eTYPE_MOUSE_MOVE;
				devEvent.data.mouseMove.dx = event.value;

			}
			else if(event.code == REL_Y){
				// Vertical
				devEvent.type = DeviceEvent::eTYPE_MOUSE_MOVE;
				devEvent.data.mouseMove.dy = event.value;
			}
		}

		strcpy(devEvent.devicePath, mDevicePath);

		mListener->onDeviceEvent(devEvent);
	}

	{
		// Send a device removed event
		DeviceEvent devEvent;
		memset(&devEvent, 0x00, sizeof(DeviceEvent));

		devEvent.type = DeviceEvent::eTYPE_DEVICE_REMOVED;
		strcpy(devEvent.devicePath, mDevicePath);

		mListener->onDeviceEvent(devEvent);
	}
}

void* DeviceReader::sMainLoop(void* arg){
	DeviceReader* thiz = static_cast<DeviceReader*>(arg);

	thiz->mainLoop();

	return NULL;
}

void DeviceReader::stop(){
	mRunning = false;

	pthread_join(mReadThread, NULL);
}

} // </si>
