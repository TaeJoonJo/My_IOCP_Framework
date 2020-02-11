
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
	// 키값으로 무슨 함수를 호출할지, 그리고 그 함수 인자
	//std::unordered_map<EPacketType, PacketFunc> a;
};

#define _EXECPACKET(ppacket)

#endif // !__PACKETHANDLER_H__
