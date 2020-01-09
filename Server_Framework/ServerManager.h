
#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__

#include "Thread.h"
#include "Session.h"

class CIOCompletionPort;

class CSessionPool;

template<typename T>
class CFreelist;

class CServerManager
{
public:
	CServerManager();
	~CServerManager();
	CServerManager(const CServerManager& o) = delete;
	CServerManager& operator=(const CServerManager& o) = delete;
private:
	const bool InitalizeServer();

	const bool InitalizeNetwork();
	const bool InitalizeThread();

	const bool IsCorrectAddr(SOCKADDR_IN& addr);

	/* Thread Function */
	static void _AcceptThread(LPVOID lp);
	static void _WorkerThread(LPVOID lp);

	/* work for IOCP */
	const bool AcceptThread();
	const bool WorkerThread();

	const bool PacketProcess(CSession* psession);
public:
	const bool StartServer();
	const bool CloseServer();

	const bool Disconnect(CSession* psession);
	const bool Disconnect(uint32_t sessionID);
private:
	std::shared_ptr<CIOCompletionPort> IOCP_;

	CThread				AcceptThread_;
	CMultiThread		WorkerThread_;

	std::shared_ptr<CSessionPool> SessionPool_;

	CFreelist<IOContext>* pSendIOContextList_;
};

#endif // !__SERVERMANAGER_H__
