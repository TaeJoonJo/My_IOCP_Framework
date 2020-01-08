
#ifndef __DEFINE_H__
#define __DEFINE_H__


// printf
#define _PFDEBUG(f) printf("Fail %s in %s\n", #f, __FUNCTION__)

#define _RELEASEPTR(ptr)	if(ptr) delete(ptr);\
							ptr = nullptr;
#define _RELEASEHANDLE(handle)	CloseHandle(handle);\
								handle = INVALID_HANDLE_VALUE;
#define _RELEASESOCKET(socket)	closesocket(socket);\
								socket = INVALID_SOCKET;


#endif // !__DEFINE_H__
