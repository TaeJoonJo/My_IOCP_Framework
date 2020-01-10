
#ifndef __SESSION_H__
#define __SESSION_H__

#include "../Include/Headers.h"
#include "../Include/Const.h"
#include "../Common/Lock.h"
#include "../Common/CircularQueue.h"

enum EIOType {
	RECV_ = 0,
	SEND_,
};

// For Overlapped
typedef struct stIOContext {
	WSAOVERLAPPED	Overlapped_;
	WSABUF			Wsabuf_;
	char			Buf_[MAX_BUFFER];
	EIOType			Type_;

	void Clear() {
		ZeroMemory(&Overlapped_, sizeof(WSAOVERLAPPED));
		ZeroMemory(&Wsabuf_, sizeof(WSABUF));
		ZeroMemory(&Buf_, static_cast<size_t>(MAX_BUFFER));
		Type_ = EIOType::RECV_;
	}
}IOCONTEXT, IOContext;

constexpr uint32_t UNREGISTED_SESSIONID = UINT32_MAX;

class CSession {
public:
	CSession();
	~CSession();
	CSession(const CSession& o) = delete;
	CSession& operator=(const CSession& o) = delete;
public:
	enum class ESessionStatus {
		JOIN_ = 1,
	};

public:
	const bool Initalize(SOCKET& s, uint32_t id);
	const bool Clear();
	// TODO : IoContext Freelist 구현
	const bool Send(void* ppacket, IOContext* poverlapped);
private:
	volatile bool isRun_;

	uint32_t ID_;
	SOCKET Socket_;

	// Send는 동시다발적으로 발생할 수 있으므로 lock필요(?)
	CMutexLock SendMutex_;

	ESessionStatus eStatus_;
public:
	IOContext IoContext_;
	CCircularQueue<char> RecvBuffer_;
public:
	inline const bool IsRun() const {
		return isRun_;
	}
	inline const uint32_t GetID() const {
		return ID_;
	}
	inline const ESessionStatus GetStatus() const {
		return eStatus_;
	}
};

class CGameSession {
public:
	CGameSession() {};
	~CGameSession() {};
};

#endif // !__SESSION_H__
