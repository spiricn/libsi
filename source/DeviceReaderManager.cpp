#include "si/DeviceReaderManager.h"
#include "si/DirectoryMonitor.h"
#include "si/DeviceReader.h"
#include "si/Common.h"

#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef ANDROID
#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "libsi-DeviceReaderManager"
#endif

#define DEV_PATH "/dev/input"

namespace si{

DeviceReaderManager* DeviceReaderManager::sInstance = NULL;

DeviceReaderManager::DeviceReaderManager() : mListener(NULL), mDirMonitor(new DirectoryMonitor()){
	memset(mDevReaders, 0x00, kMAX_DEV_READERS*sizeof(DeviceReader*));

	ALOGI("Initializing DeviceReaderManager");

	pthread_mutex_init(&mMutex, NULL);

	// Add initial devices
	DIR* d = opendir(DEV_PATH);
	struct dirent* dir;

	if(d){
		while((dir = readdir(d)) != NULL){
			// Skip . and ..
			if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0){
				continue;
			}

			// Get full device path
			char devPath[PATH_MAX];
			sprintf(devPath, DEV_PATH "/%s", dir->d_name);

			// Add a reader
			addReader(devPath);
		}

		closedir(d);
	}
	else{
		ALOGE("Error openning directory %s", strerror(errno));
	}

	// Start monitoring /dev/input
	mDirMonitor->monitor(DEV_PATH, this);
}

void DeviceReaderManager::registerListener(IDeviceReaderListener* listener){
	mListener = listener;
}

DeviceReaderManager* DeviceReaderManager::getInstance(){
	if(!sInstance){
		sInstance = new DeviceReaderManager;
	}

	return sInstance;
}

void DeviceReaderManager::addReader(const char* device){
	ALOGD("Adding reader \"%s\"\n", device);

	pthread_mutex_lock(&mMutex);

	for(int i=0; i<kMAX_DEV_READERS; i++){
		if(mDevReaders[i] == NULL){
			mDevReaders[i] = new DeviceReader(this, device);
			break;
		}
	}

	pthread_mutex_unlock(&mMutex);
}

DeviceReader* DeviceReaderManager::findReader(const char* device){
	for(int i=0; i<kMAX_DEV_READERS; i++){
		// Get full device path
		char devicePath[PATH_MAX];
		sprintf(devicePath, DEV_PATH "/%s", device);

		if(mDevReaders[i] && strcmp(mDevReaders[i]->getPath(), devicePath) == 0){
			return mDevReaders[i];
		}
	}

	return NULL;
}

void DeviceReaderManager::onFileDeleted(const char* name){
	ALOGD("Removing reader for device \"%s\"\n", name);

	DeviceReader* reader = findReader(name);

	reader->stop();

	for(int i=0; i<kMAX_DEV_READERS; i++){
		if(mDevReaders[i] == reader){
			mDevReaders[i] = NULL;
			delete reader;

			break;
		}
	}
}

void DeviceReaderManager::onFileCreated(const  char* name){
	ALOGD("Adding reader for device \"%s\"\n", name);

	// Full device path
	char devicePath[PATH_MAX];
	sprintf(devicePath, DEV_PATH "/%s", name);

	addReader(devicePath);
}

void DeviceReaderManager::onDeviceEvent(const DeviceEvent& event){
	pthread_mutex_lock(&mMutex);

	if(mListener){
		mListener->onDeviceEvent(event);
	}

	pthread_mutex_unlock(&mMutex);
}

}; // </si>
