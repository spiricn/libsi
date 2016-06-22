#ifndef DEVICEEVENT_H_
#define DEVICEEVENT_H_

#include <limits.h>

namespace si{

struct DeviceEvent{
	enum Type{
		eTYPE_UNKOWN,
		eTYPE_MOUSE_MOVE,
		eTYPE_MOUSE_CLICK,
		eTYPE_KEYBOARD,
		eTYPE_DEVICE_ADDED,
		eTYPE_DEVICE_REMOVED,
		eTYPE_MAX
	};

	enum MouseButton{
		eBUTTON_LEFT,
		eBUTTON_MIDDLE,
		eBUTTON_RIGHT,
	};

	DeviceEvent() : type(eTYPE_UNKOWN){
	}

	Type type;

	union Data{
		struct{
			int dx;
			int dy;
		} mouseMove;

		struct{
			MouseButton button;
			int down;
		} mouseClick;

		struct{
			int code;
			int down;
		} keyboard;
	} data;

	char devicePath[PATH_MAX];

}; // </DeviceEvent>

}; // </si>

#endif //  </DEVICEEVENT_H_>
