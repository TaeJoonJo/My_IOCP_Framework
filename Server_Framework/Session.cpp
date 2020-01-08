#include "Session.h"
#include "Define.h"

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

const bool CSession::Send()
{
	SendMutex_.Lock();
	{

	}
	SendMutex_.UnLock();

	return false;
}
