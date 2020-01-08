
#ifndef __SESSIONPOOL_H__
#define __SESSIONPOOL_H__

#include <unordered_map>
#include "Singleton.h"
#include "Lock.h"

class CSession;

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
private:
	std::array<CSession*, MAX_SESSION>	aSessionPool_;

	// 실제로 사용중인 Sessions
	UNMAPSESSIONPOOL					UsingSessions_;
	CMutexLock	SessionLock_;
public:
	const CSession* GetSession(const uint32_t sessionID) const {
		return UsingSessions_.at(sessionID);
	}
};

#endif // !__SESSIONPOOL_H__
