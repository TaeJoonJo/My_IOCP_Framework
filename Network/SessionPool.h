
#ifndef __SESSIONPOOL_H__
#define __SESSIONPOOL_H__

#include <unordered_map>
#include "../Common/Singleton.h"
#include "../Common/Lock.h"
#include "../Common/Freelist.h"
#include "../Common/MemoryPool.h"
#include "Session.h"

#ifdef _DEBUG
constexpr uint32_t MAX_SESSION = 1000;
#else
constexpr uint32_t MAX_SESSION = 10000;
#endif

using UNMAPSESSIONPOOL = std::unordered_map<uint32_t, CSession*>;

class CSessionPool :
	public Singleton::CDynamicSingleton<CSessionPool>
{
public:
	CSessionPool();
	~CSessionPool();
public:
	const bool Initalize();
	const bool Destroy();

	CSession*		JoinSession(SOCKET& socket, uint32_t sessionID);
	const bool		LeaveSession(CSession* psession);
	const bool		LeaveSession(uint32_t sessionID);

	const bool	Broadcasting(void* ppacket, std::unique_ptr<CFreelist<IOContext>>& freelist);
	const bool	Broadcasting(void* ppacket, std::unique_ptr<CFreelist<IOContext>>& freelist, CSession::ESessionStatus sendSessionStatus);
private:
	std::array<CSession*, MAX_SESSION>	aSessionPool_;
	CDynamicMemoryPool<CSession, MAX_SESSION>		SessionPool_;
	// 실제로 사용중인 Sessions
	UNMAPSESSIONPOOL					UsingSessions_;
	CMutexLock							SessionLock_;
public:
	inline const CSession* GetSession(const uint32_t sessionID) const {
		return UsingSessions_.at(sessionID);
	}
	const uint32_t GetCurrentSessionNum();
};

#endif // !__SESSIONPOOL_H__
