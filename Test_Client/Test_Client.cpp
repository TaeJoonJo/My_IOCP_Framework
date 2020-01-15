// Test_Client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include "../Include/Define.h"
#include "../Network/Protocol.h"

SOCKET _Socket;

HANDLE _EventHandle;
WSANETWORKEVENTS _WSANetworkEvents;

WSABUF _WSASendBuf;
char   _SendBuf[MAX_BUFFER];
WSABUF _WSARecvBuf;
char   _RecvBuf[MAX_BUFFER];

char        _PacketBuf[MAX_BUFFER];
uint32_t    _PrevPacketSize;

void SendC2SPacketTest(const wchar_t* str)
{
    ZeroMemory(_SendBuf, MAX_BUFFER);
    C2S_PACKET_TEST* ppacket = reinterpret_cast<C2S_PACKET_TEST*>(_SendBuf);
    int strLen = wcslen(str) + 2;
    memcpy(ppacket->str, ppacket, strLen);
    ppacket->header.size_ = sizeof(C2S_PACKET_TEST);
    _WSASendBuf.len = sizeof(C2S_PACKET_TEST);
    DWORD iobyte;
    ppacket->header.type_ = EPacketType::Test_;
    int ret = WSASend(_Socket, &_WSASendBuf, 1, &iobyte, 0, NULL, NULL);
    if (ret) {
        int error_code = WSAGetLastError();
        printf("Error while sending packet [%d]", error_code);
    }
}

void PacketProcess(const char* packet)
{
    const PACKET_HEADER* pheader = reinterpret_cast<const PACKET_HEADER*>(packet);

    switch (pheader->type_) {
    case EPacketType::Test_:
    {
        const S2C_PACKET_TEST* ppacket = reinterpret_cast<const S2C_PACKET_TEST*>(packet);
        TCHAR str[MAX_STRING]{};
        memcpy(str, ppacket->str, MAX_STRING);
        wprintf(L"%s\n", str);
    } break;
    default:
        break;
    }
}

void ReadPacket(SOCKET socket) 
{
    DWORD ioByte, flag = 0;

    int ret = WSARecv(socket, &_WSARecvBuf, 1, &ioByte, &flag, NULL, NULL);
    if (ret) {
        int err_code = WSAGetLastError();
        printf("Recv Error [%d]\n", err_code);
    }

    BYTE* ptr = reinterpret_cast<BYTE*>(_RecvBuf);
    int requiredSize = 0;

    if (0 != _PrevPacketSize) {
        requiredSize = _PacketBuf[0];
    }
    while (0 != ioByte) {
        if (0 == requiredSize) {
            requiredSize = ptr[0];
        }
        if (ioByte + _PrevPacketSize >= requiredSize) {
            memcpy(_PacketBuf + _PrevPacketSize, ptr, requiredSize - _PrevPacketSize);
            PacketProcess(_PacketBuf);
            ptr += requiredSize - _PrevPacketSize;
            ioByte -= requiredSize - _PrevPacketSize;
            requiredSize = 0;
            _PrevPacketSize = 0;
        }
        else {
            memcpy(_PacketBuf + _PrevPacketSize, ptr, ioByte);
            _PrevPacketSize += ioByte;
            ioByte = 0;
        }
    }
}

void Run()
{
    DWORD index{};
    while (true) {
        index = WSAWaitForMultipleEvents(1, &_EventHandle, FALSE, 1000, FALSE);

        if (index == WSA_WAIT_FAILED) {
            continue;
        }
        else if (index == WSA_WAIT_TIMEOUT) {
            continue;
        }

        int retval = WSAEnumNetworkEvents(_Socket, _EventHandle, &_WSANetworkEvents);
        if (retval == SOCKET_ERROR) {
            //this->Disconnect();
            continue;
        }

        if (_WSANetworkEvents.lNetworkEvents & FD_READ) {
            if (0 != _WSANetworkEvents.iErrorCode[FD_READ]) {
                continue;
            }
            
            ReadPacket(_Socket);
        }

        if (_WSANetworkEvents.lNetworkEvents & FD_CLOSE) {
            break;
        }
    }
    WSACleanup();
}

int main()
{
    WSADATA wsaData;
    if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        exit(1);
    }

    _Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    if (INVALID_SOCKET == _Socket) {
        WSACleanup();
        exit(1);
    }

    SOCKADDR_IN serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVERPORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVERIP);
    
    int result = WSAConnect(_Socket, (sockaddr*)&serverAddr, sizeof(serverAddr), NULL, NULL, NULL, NULL);
    if (SOCKET_ERROR == result) {
        WSACleanup();
        exit(1);
    }

    _EventHandle = WSACreateEvent();
    if (_EventHandle == WSA_INVALID_EVENT) {
        WSACleanup();
        exit(1);
    }

    WSAEventSelect(_Socket, _EventHandle, FD_CLOSE | FD_READ);

    _WSASendBuf.buf = _SendBuf;
    _WSASendBuf.len = MAX_BUFFER;
    _WSARecvBuf.buf = _RecvBuf;
    _WSARecvBuf.len = MAX_BUFFER;

    std::thread runThread{ Run };

    runThread.join();
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
