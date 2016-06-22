#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>

#if ANDROID
#include <utils/Log.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "libs"
#else
#define ALOGE(fmt, ...) do{ printf(fmt "\n", ## __VA_ARGS__); }while(0)
#define ALOGI(fmt, ...) do{ printf(fmt "\n", ## __VA_ARGS__); }while(0)
#define ALOGD(fmt, ...) do{ printf(fmt "\n", ## __VA_ARGS__); }while(0)
#endif


#endif
