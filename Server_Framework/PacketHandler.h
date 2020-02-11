
#ifndef __PACKETHANDLER_H__
#define __PACKETHANDLER_H__

#include "../Include/Headers.h"
#include "../Network/Protocol.h"

class CSession;

template<typename... Ts>
constexpr void PacketFunc(CSession*, Ts...) {
	sizeof...(Ts);
}

class CPacketHandler {
public:
	CPacketHandler() = default;
	~CPacketHandler() {};
public:
private:
	// Ű������ ���� �Լ��� ȣ������, �׸��� �� �Լ� ����
	//std::unordered_map<EPacketType, PacketFunc> a;
};

#define _EXECPACKET(ppacket)

#endif // !__PACKETHANDLER_H__
