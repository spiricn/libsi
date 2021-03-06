cmake_minimum_required(VERSION 2.8)

set(CPACK_PACKAGE_NAME "libsi")
set(CPACK_PACKAGE_VENDOR "libsi-Vendor")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libsi library")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

###############################################################################

project("libRingBuffer")


set(LIB_ROOT "../../")

set(SOURCE_DIR "${LIB_ROOT}/source")

set(INCLUDE_DIR "${LIB_ROOT}/include")


set(SOURCES
	${SOURCE_DIR}/DeviceReader.cpp
	${SOURCE_DIR}/DeviceReaderManager.cpp
	${SOURCE_DIR}/DirectoryMonitor.cpp
)

set(HEADERS
	${INCLUDE_DIR}/si/DeviceEvent.h
	${INCLUDE_DIR}/si/DeviceReader.h
	${INCLUDE_DIR}/si/DeviceReaderManager.h
	${INCLUDE_DIR}/si/DirectoryMonitor.h
	${INCLUDE_DIR}/si/IDeviceReaderListener.h
	${INCLUDE_DIR}/si/IDirectoryMonitorListener.h
	${INCLUDE_DIR}/si/Common.h
)

include_directories(${INCLUDE_DIR})

# With -fPIC
IF(UNIX AND NOT WIN32)
  FIND_PROGRAM(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin )
  IF(CMAKE_UNAME)
    EXEC_PROGRAM(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
    SET(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL
"processor type (i386 and x86_64)")
    IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
      ADD_DEFINITIONS(-fPIC)
    ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
  ENDIF(CMAKE_UNAME)
ENDIF(UNIX AND NOT WIN32)

add_library(
	si SHARED
	${SOURCES}
	${HEADERS}
)

