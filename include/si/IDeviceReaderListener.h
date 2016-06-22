#ifndef IDEVICEREADERLISTENER_H_
#define IDEVICEREADERLISTENER_H_

namespace si{

class DeviceEvent;

class IDeviceReaderListener{
public:
	virtual ~IDeviceReaderListener(){
	}

	virtual void onDeviceEvent(const DeviceEvent& event) = 0;

}; // </IDeviceReaderListener>

}; // </si>

#endif // </IDEVICEREADERLISTENER_H_>
