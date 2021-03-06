
#include "stdafx.h"
#include "ServerManager.h"
#include "../Include/Const.h"
#include "../Include/Define.h"
#include "../Network/Network.h"
#include "../Network/IOCP.h"
#include "../Network/SessionPool.h"
#include "../Network/Protocol.h"
#include "../Network/PacketHandler.h"
#include "../Common/LogSystem.h"
#include "../Common/Freelist.h"

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

	if (false == this->InitalizeInfo()) {
		_LERROR("InitalizeInfo()");
		return false;
	}

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

	if (false == pSessionPool_->Destroy()) {
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
	if (false == pSessionPool_->LeaveSession(sessionID)) {
		_LWARNING("");
		return false;
	}

	_LINFO("ID : %d Disconnected", sessionID)

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

const bool CServerManager::InitalizeInfo()
{
	pSessionPool_ = CSessionPool::GetInstance();
	if (false == pSessionPool_->Initalize()) {
		_LERROR("SessionPool_ Initalize()");
		return false;
	}

	pSendIOContextList_ = std::make_unique<CFreelist<IOContext>>(MAX_SESSION);
	
	pPacketHandler_ = std::make_unique<Packet::CPacketHandler>();
#pragma region RegisterPacketHandler	
	pPacketHandler_->RegisterPacketHandler(this, Packet::EPacketType::Test, &CServerManager::Recv_Packet_C2S_Test);
#pragma endregion

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

	if (this->InitalizeServer() == false) {
		return false;
	}

	_LINFO("StartServer() End");

	return true;
}

void CServerManager::StopServer()
{
	_LINFO("StopServer()");

	AcceptThread_.Stop();

	WorkerThread_.Stop();
}

void CServerManager::_AcceptThread(LPVOID lp)
{
	CServerManager* pthis = reinterpret_cast<CServerManager*>(lp);
	if (nullptr == pthis) {
		return;
	}
	pthis->AcceptThread();
}

void CServerManager::_WorkerThread(LPVOID lp)
{
	CServerManager* pthis = reinterpret_cast<CServerManager*>(lp);
	if (nullptr == pthis) {
		return;
	}
	pthis->WorkerThread();
}

const bool CServerManager::AcceptThread()
{
	//_LINFO("AcceptThread Start");
	_LINFO("ID : %d AcceptThread Start", std::this_thread::get_id());

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
	serverAddr.sin_port			= htons(SERVERPORT);
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
		CSession* psession = pSessionPool_->JoinSession(clientSoc, sessionID);
		if (nullptr == psession) {
			// TODO : 대기열 or 접속 끊기
			_LWARNING("Fail JoinSession ID : %d", sessionID);
			continue;
		}

		// IOCP에 연결
		if (INVALID_HANDLE_VALUE == CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSoc), IOCP_->GetIOCP(), reinterpret_cast<ULONG_PTR>(psession), 0)) {
			// TODO : 대기열 or 접속 끊기
			_LWARNING("Fail Connect IOCP ID : %d", sessionID);
			this->Disconnect(psession);
		}
		_LINFO("OK SessionJoin ID : %d", sessionID);

		++_SessionID;
		psession->IoContext_.Wsabuf_.buf = psession->IoContext_.Buf_;
		psession->IoContext_.Wsabuf_.len = MAX_BUFFER;
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
	_LINFO("ID : %d WorkerThread Start", std::this_thread::get_id());

	CSession*			psession = nullptr;
	IOContext*			poverlapped = nullptr;
	DWORD				ioByte = 0;
	int					returnValue = 0;
	EIOType				ioType;

	while (true == WorkerThread_.IsRun()) {
		psession = nullptr;
		poverlapped = nullptr;
		ioByte = 0;
		returnValue = 0;

		if (false == GetQueuedCompletionStatus(IOCP_->GetIOCP(), &ioByte, 
												reinterpret_cast<ULONG_PTR*>(&psession), 
												reinterpret_cast<LPOVERLAPPED*>(&poverlapped), INFINITE)) {
			returnValue = WSAGetLastError();
		}

		// 오류 처리
		if (0 != returnValue) {
			if (WAIT_TIMEOUT == returnValue) {
				Disconnect(psession);
			}
			else if (ERROR_NETNAME_DELETED == returnValue) {
				Disconnect(psession);
			}
			else {
				Disconnect(psession);
			}
			continue;
		}

		if (nullptr == psession) {
			continue;
		}

		// 클라이언트쪽에서 연결끊음
		if (0 == ioByte) {
			Disconnect(psession);
		}
		ioType = poverlapped->Type_;

		switch (ioType) {
		case EIOType::RECV_:
		{
			int32_t packetSize = 0;
			char* pbuf = psession->IoContext_.Buf_;
			
			// 이전에 받은 데이터가 있다면 recvbuffer에서 패킷크기를 받아온다.
			if (0 != psession->RecvBuffer_.GetNowQueueSize()) {
				packetSize = *reinterpret_cast<Packet::PACKETSIZE_T*>(psession->RecvBuffer_.GetAt(0));
			}
			while (0 < ioByte) {
				// 패킷 크기를 알아내지 못했다면, 받은 패킷에서 읽는다.
				if (packetSize == 0) {
					packetSize = *reinterpret_cast<Packet::PACKETSIZE_T*>(pbuf);
				}
				UINT required = packetSize - psession->RecvBuffer_.GetNowQueueSize();		// 패킷을 완성하는데 필요한 바이트 수
				if (ioByte < required) { // 완성하지 못하면 RecvBuffer에 넣어주고 빠져나간다.
					psession->RecvBuffer_.Enq(pbuf, ioByte);
					break;
				}
				else {	// 패킷을 완성할 수 있다면
					psession->RecvBuffer_.Enq(pbuf, required);
					if (false == PacketProcess(psession)) {
						_LWARNING("Session ID : [ %d ] PacketProcess Return false", psession->GetID());
					}

					// 다음 패킷을 조립하기 위한 준비
					ioByte -= required;
					pbuf += required;
					packetSize = 0;
				}
			}
			DWORD flags{};
			returnValue = WSARecv(psession->GetSocket(), &(psession->IoContext_.Wsabuf_), 1, NULL, &flags, &(psession->IoContext_.Overlapped_), NULL);

			if (SOCKET_ERROR == returnValue) {
				int errCode = WSAGetLastError();
				if (WSA_IO_PENDING != errCode) {
					_LERROR("WorkerThread WSARecv() Error ");
					return false;
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
	}

	_LINFO("WorkerThread End");

	return true;
}

const bool CServerManager::PacketProcess(CSession* psession)
{
	//int8_t buf[MAX_BUFFER]{};
	//psession->RecvBuffer_.Deq((char*)buf, PACKET_HEADER_SIZE);
	//PACKET_HEADER* pheader = reinterpret_cast<PACKET_HEADER*>(buf);
	//psession->RecvBuffer_.Deq((char*)(buf + sizeof(PACKET_HEADER)), pheader->size_ - sizeof(PACKET_HEADER));

	//int8_t* pbuf = reinterpret_cast<int8_t*>(psession->RecvBuffer_.GetAt(0));
	//const PACKET_HEADER* pheader = reinterpret_cast<const PACKET_HEADER*>(psession->RecvBuffer_.GetAt(0));

	//bool returnValue = pPacketHandler_->ExecutePacketHandler(pheader->type_, psession, pbuf);

	CDataBuffer dataBuffer(psession->RecvBuffer_.GetAt(0));
	Packet::CPacketManager packetManager(dataBuffer);

	bool returnValue = true;
	try 
	{
		returnValue = pPacketHandler_->ExecutePacketHandler(packetManager.GetType(), psession, &packetManager);
	}
	catch(std::exception& e)
	{
		_LERROR(e.what());
	}

	// TODO : pop을 함수 초기에 해줘야하나? (위험이 있는지 생각...)
	if (false == psession->RecvBuffer_.Pop(packetManager.GetSize())) {

	}

	return returnValue;
}

