#include "SessionPool.h"
#include "../Include/Define.h"
#include "../Common/LogSystem.h"

CSessionPool::CSessionPool()
{
	aSessionPool_.fill(nullptr);
}

CSessionPool::~CSessionPool()
{
}

const bool CSessionPool::Initalize()
{
	SessionLock_.Lock();
	{
		for (int i = 0; i < MAX_SESSION; ++i) {
			aSessionPool_[i] = new CSession();
		}
	}
	SessionLock_.UnLock();

	return true;
}

const bool CSessionPool::Destroy()
{
	SessionLock_.Lock();
	{
		for (int i = 0; i < MAX_SESSION; ++i) {
			_RELEASEPTR(aSessionPool_[i]);
		}
	}
	SessionLock_.UnLock();

	return true;
}

CSession* CSessionPool::JoinSession(SOCKET& socket, uint32_t sessionID)
{
	CSession* psession = nullptr;

	SessionLock_.Lock();
	{
		for (int i = 0; i < MAX_SESSION; ++i) {
			if (false == aSessionPool_[i]->IsRun()) {
				if (false == aSessionPool_[i]->Initalize(socket, sessionID)) {
					continue;
				}
				psession = aSessionPool_[i];
				UsingSessions_[sessionID] = psession;
				break;
			}
		}
	}
	SessionLock_.UnLock();

	return psession;
}

const bool CSessionPool::LeaveSession(CSession* psession)
{
	bool returnValue = false;
	uint32_t sessionID = UNREGISTED_SESSIONID;

	SessionLock_.Lock();
	{
		uint32_t sessionID = psession->GetID();
	}
	SessionLock_.UnLock();

	if (UNREGISTED_SESSIONID != sessionID) {
		returnValue = LeaveSession(sessionID);
	}

	return returnValue;
}

const bool CSessionPool::LeaveSession(uint32_t sessionID)
{
	CSession* psession = nullptr;

	SessionLock_.Lock();
	{
		psession = UsingSessions_.at(sessionID);
		if (nullptr == psession) {
			SessionLock_.UnLock();
			return false;
		}
		else {
			psession = UsingSessions_[sessionID];
			psession->Clear();
			UsingSessions_.erase(sessionID);
		}
	}
	SessionLock_.UnLock();

	return true;
}

const bool CSessionPool::Broadcasting(void* ppacket, std::unique_ptr<CFreelist<IOContext>>& freelist)
{
	bool returnValue = true;
	CSession* psession = nullptr;

	SessionLock_.Lock();
	{
		for (auto& session : UsingSessions_) {
			psession = session.second;
			returnValue = psession->Send(ppacket, freelist->GetData());
			if (false == returnValue) {
				_LWARNING("Broadcasting");
			}
		}
	}
	SessionLock_.UnLock();

	return returnValue;
}

const bool CSessionPool::Broadcasting(void* ppacket, std::unique_ptr<CFreelist<IOContext>>& freelist, CSession::ESessionStatus sendSessionStatus)
{
	bool returnValue = true;
	CSession* psession = nullptr;

	SessionLock_.Lock();
	{
		for (auto& session : UsingSessions_) {
			psession = session.second;
			if (sendSessionStatus != psession->GetStatus()) {
				continue;
			}
			returnValue = psession->Send(ppacket, freelist->GetData());
			if (false == returnValue) {
				_LWARNING("Broadcasting");
			}
		}
	}
	SessionLock_.UnLock();

	return returnValue;
}

const uint32_t CSessionPool::GetCurrentSessionNum()
{
	uint32_t currentSessionNum = 0;

	SessionLock_.Lock();
	{
		currentSessionNum = static_cast<uint32_t>(UsingSessions_.size());
	}
	SessionLock_.UnLock();

	return currentSessionNum;
}
