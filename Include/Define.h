
#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "Headers.h"

#ifdef DEBUG
#define SERVERIP "127.0.0.1"
#else
#define SERVERIP "127.0.0.1"
#endif // DEBUG

#define SERVERPORT 3600

// printf
#define _PFDEBUG(f) printf("Fail %s in %s\n", #f, __FUNCTION__)



#define _DISABLECOPY(type)			public:\
									type(const type&) = delete;\
									type& operator=(const type&) = delete;

#define _DEAFAULT(type)				type() = default;\
									~type() = default;


#define _RELEASEPTR(ptr)	if(ptr) delete(ptr);\
							ptr = nullptr;
#define _RELEASEHANDLE(handle)	CloseHandle(handle);\
								handle = INVALID_HANDLE_VALUE;
#define _RELEASESOCKET(socket)	closesocket(socket);\
								socket = INVALID_SOCKET;


#endif // !__DEFINE_H__
