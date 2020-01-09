#include "stdafx.h"
#include "ServerManager.h"
#include "Network.h"
#include "IOCP.h"
#include "Const.h"
#include "Define.h"
#include "LogSystem.h"
#include "SessionPool.h"
#include "Protocol.h"
#include "Freelist.h"

// use for session id
static volatile uint32_t _SessionID;

CServerManager::CServerManager()
	: pSendIOContextList_(nullptr)
{
}

CServerManager::~CServerManager()
{
}

const bool CServerManager::InitalizeServer()
{
	_LINFO("InitalizeServer() Start");

	if (false == this->InitalizeNetwork()) {
		_LERROR("InitalizeNetwork()");
		return false;
	}

	SessionPool_ = CSessionPool::GetInstance();
	if (false == SessionPool_->Initalize()) {
		_LERROR("SessionPool_ Initalize()");
		return false;
	}

	pSendIOContextList_ = new CFreelist<IOContext>(MAX_SESSION);

	if (false == this->InitalizeThread()) {
		_LERROR("InitalizeThread()");
		return false;
	}

	_LINFO("InitalizeServer() End");

	return true;
}

const bool CServerManager::CloseServer()
{
	_LINFO("CloseServer() Start");

	if (false == AcceptThread_.Join()) {
		_LWARNING("AcceptThread_ Join()");
	}

	if (false == WorkerThread_.Join()) {
		_LWARNING("WorkerThread_ Join()");
	}

	if (false == Network::EndNetwork()) {
		_LERROR("EndNetwork()");
	}

	if (false == SessionPool_->Destroy()) {
		_LERROR("SessionPool_ Destroy()");
	}
	
	_LINFO("CloseServer() End");

	return true;
}

const bool CServerManager::Disconnect(CSession* psession)
{
	bool returnValue = true;

	returnValue = Disconnect(psession->GetID());

	return returnValue;
}

const bool CServerManager::Disconnect(uint32_t sessionID)
{
	if (false == SessionPool_->LeaveSession(sessionID)) {
		_LWARNING("");
		return false;
	}

	return true;
}

const bool CServerManager::InitalizeNetwork()
{
	_LINFO("InitalizeNetwork() Start");

	if (false == Network::StartNetwork()) {
		_LERROR("StartNetwork()");
		return false;
	}

	IOCP_ = CIOCompletionPort::GetInstance();
	if (false == IOCP_->StartIOCP()) {
		_LERROR("StartIOCP()");
		return false;
	}

	_LINFO("InitalizeNetwork() End");

	return true;
}

const bool CServerManager::InitalizeThread()
{
	_LINFO("InitalizeThread() Start");

	AcceptThread_.Initalize(_AcceptThread, this);
	if (false == AcceptThread_.Start()) {
		_LERROR("AcceptThread Start()");
		return false;
	}

	WorkerThread_.Initalize(WORKERTHREAD_NUM, _WorkerThread, this);
	if (false == WorkerThread_.Start()) {
		_LERROR("WorkerThread Start()");
		return false;
	}

	_LINFO("InitalizeThread() End");

	return true;
}

// TODO : IP검사
const bool CServerManager::IsCorrectAddr(SOCKADDR_IN& addr)
{
	return true;
}

const bool CServerManager::StartServer()
{
	_LINFO("StartServer() Start");

	if (this->InitalizeServer() == false)
		return false;

	_LINFO("StartServer() End");

	return true;
}

void CServerManager::_AcceptThread(LPVOID lp)
{
	CServerManager* pthis = reinterpret_cast<CServerManager*>(lp);
	if (nullptr == pthis)
		return;
	pthis->AcceptThread();
}

void CServerManager::_WorkerThread(LPVOID lp)
{
	CServerManager* pthis = reinterpret_cast<CServerManager*>(lp);
	if (nullptr == pthis)
		return;
	pthis->WorkerThread();
}

const bool CServerManager::AcceptThread()
{
	_LINFO("AcceptThread Start");
	//_LINFO("ID : %d AcceptThread Start", std::this_thread::get_id());

	SOCKET serverSoc = INVALID_SOCKET;
	SOCKET clientSoc = INVALID_SOCKET;
	SOCKADDR_IN serverAddr{};
	SOCKADDR_IN clientAddr{};
	int addrLen = static_cast<int>(sizeof(SOCKADDR_IN));
	DWORD flags{};
	uint32_t sessionID{};
	

	// 리슨 소켓 생성
	serverSoc = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == serverSoc) {
		_LERROR("WSASocketW in AcceptThread");
		return false;
	}
	
	// 바인드
	serverAddr.sin_family		= PF_INET;
	serverAddr.sin_port			= htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	if (SOCKET_ERROR == bind(serverSoc, reinterpret_cast<SOCKADDR*>(&serverAddr), addrLen)) {
		_LERROR("bind()");
		return false;
	}

	// 리슨 
	/// SOMAXCONN으로 쓰는게 최선인가?
	if (SOCKET_ERROR == listen(serverSoc, SOMAXCONN)) {
		_LERROR("listen()");
		return false;
	}
	
	while (true == AcceptThread_.IsRun()) {
		sessionID = _SessionID;
		// Accept 시작
		clientSoc = accept(serverSoc, reinterpret_cast<SOCKADDR*>(&clientAddr), &addrLen);

		_LINFO("Accept Socket");

		if (INVALID_SOCKET == clientSoc) {
			_LWARNING("accept INVALID_SOCKET");
			continue;
		}

		if (false == IsCorrectAddr(clientAddr)) {
			continue;

		}

		// Nagle Turn Off
		/// accept 전에?
		if (false == Network::ChangeNagle(clientSoc, true)) {
		}

		/* 들어온 클라이언트 처리 */
		CSession* psession = SessionPool_->JoinSession(clientSoc, sessionID);
		if (nullptr == psession) {
			// TODO : 대기열 or 접속 끊기
			_LWARNING("Fail JoinSession ID : %d", sessionID);
			continue;
		}

		// IOCP에 연결
		if (INVALID_HANDLE_VALUE == CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSoc), IOCP_->GetIOCP(), reinterpret_cast<ULONG_PTR>(psession), 0)) {
			// TODO : 대기열 or 접속 끊기
			_LWARNING("Fail Connect IOCP ID : %d", sessionID);
		}
		_LINFO("OK SessionJoin ID : %d", sessionID);

		++_SessionID;
		int rtv = WSARecv(clientSoc, &(psession->IoContext_.Wsabuf_), 1, 0, &flags, &(psession->IoContext_.Overlapped_), NULL);
		if (SOCKET_ERROR == rtv) {
			int errCode = WSAGetLastError();
			if (WSA_IO_PENDING != errCode) {
				_LERROR("First WSARecv Fail");
				while (true) {}
			}
		}
	}

	_LINFO("AcceptThread End");

	return true;
}

const bool CServerManager::WorkerThread()
{
	_LINFO("WorkerThread Start");
	//_LINFO("ID : %d WorkerThread Start", std::this_thread::get_id());

	CSession*			psession = nullptr;
	IOContext*			poverlapped = nullptr;
	DWORD				ioByte = 0;
	int					rtv = 0;
	EIOType				ioType;

	while (true == WorkerThread_.IsRun()) {
		psession = nullptr;
		poverlapped = nullptr;
		ioByte = 0;
		rtv = 0;

		if (false == GetQueuedCompletionStatus(IOCP_->GetIOCP(), &ioByte, 
												reinterpret_cast<ULONG_PTR*>(&psession), 
												reinterpret_cast<LPOVERLAPPED*>(&poverlapped), INFINITE)) {
			rtv = WSAGetLastError();
		}

		// 오류 처리
		if (0 != rtv) {
			if (WAIT_TIMEOUT == rtv) {

			}
			else if (ERROR_NETNAME_DELETED == rtv) {

			}
			else {

			}
			continue;
		}

		if (nullptr == psession) {

		}

		// 클라이언트쪽에서 연결끊음
		if (0 == ioByte) {

		}
		ioType = poverlapped->Type_;

		switch (ioType) {
		case EIOType::RECV_:
		{
			const PACKET_HEADER* packetHeader = nullptr;
			int packetSize = 0;
			char* pbuf = psession->IoContext_.Buf_;

			if (0 != psession->RecvBuffer_.GetNowQueueSize()) {
				packetHeader = reinterpret_cast<const PACKET_HEADER*>(psession->RecvBuffer_.GetData(0));
				packetSize = static_cast<int>(packetHeader->size_);
			}
			while (ioByte > 0)
			{
				if (packetSize == 0)
					packetSize = pbuf[0];
				UINT required = packetSize - psession->RecvBuffer_.GetNowQueueSize();		// 패킷을 완성하는데 필요한 바이트 수
				//UINT required = packetSize - psession->m_nPrevSize;		// 패킷을 완성하는데 필요한 바이트 수
				if (ioByte < required) // 완성하지 못하면
				{
					psession->RecvBuffer_.Enq(pbuf, ioByte);
					break;
				}
				else
				{
					psession->RecvBuffer_.Enq(pbuf, required);
					// RecvBuf에 쌓인 데이터 처리
					PacketProcess(psession);

					// 다음 패킷을 조립하기 위한 준비
					ioByte -= required;
					pbuf += required;
					packetSize = 0;
				}
			}
		} break;
		case EIOType::SEND_:
		{ 
			pSendIOContextList_->ReturnData(poverlapped);
		} break;
		default:
		{

		} while (1);
		}

		// TODO 
	}

	_LINFO("WorkerThread End");

	return true;
}

const bool CServerManager::PacketProcess(CSession* psession)
{
	char buf[MAX_BUFFER]{};
	psession->RecvBuffer_.Deq(buf, PACKET_HEADER_SIZE);
	PACKET_HEADER* pheader = reinterpret_cast<PACKET_HEADER*>(buf);
	psession->RecvBuffer_.Deq(buf + sizeof(PACKET_HEADER), pheader->size_ - sizeof(PACKET_HEADER));

	switch (pheader->type_)
	{
	case EPacketType::Test_:
	{
		C2S_PACKET_TEST* pbody = reinterpret_cast<C2S_PACKET_TEST*>(buf);
		uint32_t id = pbody->id;
	} break;

	default:
	{

	} break;
	}

	return true;
}
