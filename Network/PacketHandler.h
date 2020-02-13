
#ifndef __PACKETHANDLER_H__
#define __PACKETHANDLER_H__

#include "../Include/Headers.h"
#include "../Network/Protocol.h"
#include "../Common/LogSystem.h"

class CServerManager;
class CSession;
class CPacketManager;

// parameter : Sessionpointer, datapointer
//using PacketHandlerFunc = std::function<const bool(CSession*, int8_t*)>;			
namespace Packet {
	using PacketHandlerFunc = std::function<const bool(CSession*, Packet::CPacketManager*)>;

	class CPacketHandler {
	public:
		CPacketHandler() = default;
		~CPacketHandler() {};
	public:
		//std::function<const bool(CServerManager*, CSession*, int8_t*)> handlerFunc    const bool(CServerManager::* handlerFunc)(CSession*, int8_t*))
		const bool RegisterPacketHandler(CServerManager* pinstance, Packet::EPacketType epacketType, std::function<const bool(CServerManager*, CSession*, Packet::CPacketManager*)> handlerFunc) {
			std::unordered_map<Packet::EPacketType, PacketHandlerFunc>::iterator mapIter = HandlerHashMap_.find(epacketType);
			if (HandlerHashMap_.end() == mapIter) {
				HandlerHashMap_.insert(std::make_pair(epacketType, std::bind(handlerFunc, pinstance, std::placeholders::_1, std::placeholders::_2)));
			}
			return true;
		}
		const bool ExecutePacketHandler(Packet::EPacketType epacketType, CSession* psession, Packet::CPacketManager* ppacketManager) {
			std::unordered_map<Packet::EPacketType, PacketHandlerFunc>::iterator mapIter = HandlerHashMap_.find(epacketType);
			if (HandlerHashMap_.end() == mapIter) {
				_LWARNING("Not Registered PacketType : [ %d ] ExecutePacketHandler", epacketType)
					return false;
			}
			return mapIter->second(psession, ppacketManager);
		}
	private:
		std::unordered_map<Packet::EPacketType, PacketHandlerFunc> HandlerHashMap_;
	};
}

#endif // !__PACKETHANDLER_H__
