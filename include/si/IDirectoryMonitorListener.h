#ifndef DIRECTORYMONITORLISTENER_H_
#define DIRECTORYMONITORLISTENER_H_

namespace si{

class IDirectoryMonitorListener{
public:
	virtual ~IDirectoryMonitorListener(){
	}

	virtual void onFileDeleted(const  char* name) = 0;

	virtual void onFileCreated(const  char* name) = 0;

}; // </IDirectoryMonitorListener>

}; // </si>

#endif // </DIRECTORYMONITORLISTENER_H_>

