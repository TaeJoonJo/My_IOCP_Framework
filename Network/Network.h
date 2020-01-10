
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "../Include/Headers.h"
#include "../Include/Define.h"
#include "../Common/LogSystem.h"

namespace Network
{

	static const bool StartNetwork()
	{
		WSADATA wd{};
		if (0 != WSAStartup(MAKEWORD(2, 2), &wd)) {
			_LERROR("WSAStartup()");
			return false;
		}

		return true;
	}

	static const bool EndNetwork()
	{
		if (SOCKET_ERROR == WSACleanup()) {
			_LERROR("WSACleanup()");
			return false;
		}

		return true;
	}

	static const bool ChangeNagle(SOCKET& socket, const bool b)
	{
		int option = b;
		if (SOCKET_ERROR == setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&socket), option)) {
			_LWARNING("setsockopt()");
			return false;
		}
		return true;
	}
}

#endif // !__NETWORK_H__
