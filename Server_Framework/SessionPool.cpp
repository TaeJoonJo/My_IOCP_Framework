#include "SessionPool.h"
#include "Session.h"
#include "Define.h"

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
				if (false == aSessionPool_[i]->Initalize(socket, sessionID))
					break;
				psession = aSessionPool_[i];
				UsingSessions_[sessionID] = psession;
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

	if(UNREGISTED_SESSIONID != sessionID)
		returnValue = LeaveSession(sessionID);

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
