#include "../Include/Headers.h"
#include "../Include/Define.h"
#include "IOCP.h"

CIOCompletionPort::CIOCompletionPort()
	: IOCP_(INVALID_HANDLE_VALUE)
{

}

CIOCompletionPort::~CIOCompletionPort()
{
	_RELEASEHANDLE(IOCP_);
	//CloseHandle(IOCP_);
	//IOCP_ = INVALID_HANDLE_VALUE;
}

const bool CIOCompletionPort::StartIOCP()
{
	IOCP_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// Failed Create Port
	if (INVALID_HANDLE_VALUE == IOCP_) {
		printf("Failed CreateIoCompletionPort in StartIOCP\n");
		return false;
	}

	return true;
}
