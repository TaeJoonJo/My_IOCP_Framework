#include "Session.h"
#include "../Include/Define.h"

CSession::CSession()
	: isRun_(false), ID_(UNREGISTED_SESSIONID)
	, Socket_(INVALID_SOCKET)
	, eStatus_(CSession::ESessionStatus::JOIN_)
{
	IoContext_.Clear();
	RecvBuffer_.Clear();
}

CSession::~CSession()
{
	_RELEASESOCKET(Socket_);
}

const bool CSession::Initalize(SOCKET& s, uint32_t id)
{
	if (s == INVALID_SOCKET)
		return false;

	isRun_ = true;

	Socket_ = s;
	ID_ = id;

	IoContext_.Clear();
	RecvBuffer_.Clear();

	return true;
}

const bool CSession::Clear()
{
	isRun_ = false;

	IoContext_.Clear();
	RecvBuffer_.Clear();

	return false;
}

const bool CSession::Send(void* ppacket, IOContext* poverlapped)
{
	if (INVALID_SOCKET == Socket_)
		return false;
	if (false == isRun_)
		return false;

	int retval = 0;
	char* pbuf = reinterpret_cast<char*>(ppacket);

	ULONG size = static_cast<ULONG>(pbuf[0]);

	IOContext* sendContext = poverlapped;
	//ZeroMemory(sendCtx, sizeof(IOContext));
	sendContext->Clear();
	sendContext->Type_ = EIOType::SEND_;
	sendContext->Wsabuf_.buf = sendContext->Buf_;
	sendContext->Wsabuf_.len = size;							// 첫번째 바이트에 사이즈값이 들어있다.

	memcpy(sendContext->Buf_, pbuf, size);
	ZeroMemory(&sendContext->Overlapped_, sizeof(WSAOVERLAPPED));

	retval = WSASend(Socket_, &sendContext->Wsabuf_, 1, 0, 0, &sendContext->Overlapped_, NULL);
	if (SOCKET_ERROR == retval)
	{
		int errCode = WSAGetLastError();
		if (WSA_IO_PENDING != errCode)
		{
			return false;
		}
	}

	return false;
}
