
#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__

#include "../Common/Thread.h"
#include "../Network/Session.h"
#include "PacketHandler.h"

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
	const bool InitalizeInfo();

	const bool IsCorrectAddr(SOCKADDR_IN& addr);

	/* Thread Function */
	static void _AcceptThread(LPVOID lp);
	static void _WorkerThread(LPVOID lp);

	/* work for IOCP */
	const bool AcceptThread();
	const bool WorkerThread();

	const bool PacketProcess(CSession* psession);

	const bool Recv_Packet_C2S_Test(CSession* psession, int8_t* ppacketData);
public:
	const bool StartServer();
	const bool CloseServer();

	const bool Disconnect(CSession* psession);
	const bool Disconnect(uint32_t sessionID);
private:
	std::shared_ptr<CIOCompletionPort> IOCP_;

	CThread				AcceptThread_;
	CMultiThread		WorkerThread_;

	std::shared_ptr<CSessionPool> pSessionPool_;

	std::unique_ptr<CFreelist<IOContext>>	pSendIOContextList_;
	std::unique_ptr<CPacketHandler>			pPacketHandler_;
};

#endif // !__SERVERMANAGER_H__
