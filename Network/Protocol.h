
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdint.h>
#include "../Include/Const.h"
#include "../Include/Define.h"
#include "../Common/DataBuffer.h"
#include "testinfo_generated.h"


//#pragma pack(push, 1)
//class PACKET_HEADER {
//public:
//	explicit PACKET_HEADER(char packetSize, EPacketType packetType)
//		: size_(packetSize)
//		, type_(packetType)
//	{}
//public:
//	char		size_;
//	EPacketType type_;
//};
//
//#define PACKET_HEADER_SIZE sizeof(PACKET_HEADER)
//
//class S2C_PACKET_TEST {
//public:
//	explicit S2C_PACKET_TEST(char packetSize, EPacketType packetType)
//		: header(packetSize, packetType)
//		, str()
//	{}
//public:
//	PACKET_HEADER header;
//	TCHAR str[MAX_STRING];
//};
//
//class C2S_PACKET_TEST {
//	explicit C2S_PACKET_TEST(char packetSize, EPacketType packetType)
//		: header(packetSize, packetType)
//		, str()
//	{}
//public:
//	PACKET_HEADER header;
//	TCHAR str[MAX_STRING];
//};

namespace Serialize {

	class CSerializeManager {
	protected:
		CSerializeManager() : Size_(0) {};
		~CSerializeManager() = default;
	protected:
		enum EOffset {
			SizeOffset = 0
		};
	public:
		virtual void SetSize(uint16_t size) = 0;
		virtual const uint16_t GetSize() const = 0;
	private:
		uint16_t Size_;
	};

}

namespace Packet {

	enum EPacketType : uint8_t {
		Test = 0,
	};

	using PACKETSIZE_T = uint16_t;
	constexpr uint32_t PACKETSIZE_S = sizeof(PACKETSIZE_T);
	using PACKETTYPE_T = uint8_t;
	constexpr uint32_t PACKETTYPE_S = sizeof(PACKETTYPE_T);

	constexpr uint32_t BASICPACKETSIZE = PACKETSIZE_S + PACKETTYPE_S;

	class CPacketManager
		: public Serialize::CSerializeManager
	{
	public:
		CPacketManager() : pPacketBuffer_() {}
		explicit CPacketManager(CDataBuffer& dataBuffer) : pPacketBuffer_(&dataBuffer) {}
		~CPacketManager() { pPacketBuffer_->ReleaseBuffer(); }
		_DISABLECOPY(CPacketManager);
	private:
		enum EOffset {
			SizeOffset = Serialize::CSerializeManager::EOffset::SizeOffset,
			TypeOffset = SizeOffset + PACKETSIZE_S,
			DataOffset = TypeOffset + PACKETTYPE_S
		};
	public:
		inline CDataBuffer* AllocatePacketBuffer(uint32_t size) {
			pPacketBuffer_ = new CDataBuffer(size);
		}
		constexpr CDataBuffer* GetPacketBuffer() const {
			return pPacketBuffer_;
		}
		constexpr const char* GetPacketBuffer() {
			return pPacketBuffer_->GetBufferPointer();
		}

		constexpr void SetPacketBuffer(CDataBuffer* pdataBuffer) {
			pPacketBuffer_ = pdataBuffer;
		}

		virtual void SetSize(PACKETSIZE_T size) override {
			char* ppacketBuf = pPacketBuffer_->GetBufferPointer();
			memcpy(ppacketBuf, &size, PACKETSIZE_S);
		}
		virtual const PACKETSIZE_T GetSize() const override {
			char* ppacketBuf = pPacketBuffer_->GetBufferPointer();
			return *reinterpret_cast<PACKETSIZE_T*>(ppacketBuf);
		}

		inline void SetType(PACKETTYPE_T type) {
			char* ppacketBuf = pPacketBuffer_->GetAt(EOffset::TypeOffset);
			memcpy(ppacketBuf, &type, PACKETTYPE_S);
		}
		inline void SetType(EPacketType etype) {
			char* ppacketBuf = pPacketBuffer_->GetAt(EOffset::TypeOffset);
			memcpy(ppacketBuf, &etype, PACKETTYPE_S);
		}
		inline const PACKETTYPE_T GetType() const {
			char* ppacketBuf = pPacketBuffer_->GetAt(EOffset::TypeOffset);
			return static_cast<PACKETTYPE_T>(*ppacketBuf);
		}
		inline const EPacketType GetType() {
			char* ppacketBuf = pPacketBuffer_->GetAt(EOffset::TypeOffset);
			return static_cast<EPacketType>(*ppacketBuf);
		}

		// before call this method, must set packetdata
		inline void SetData(char* pbuf, PACKETSIZE_T bufferSize) {
			char* ppacketBuf = pPacketBuffer_->GetAt(EOffset::DataOffset);
			memcpy(ppacketBuf, pbuf, bufferSize);
		}

		constexpr const char* GetDataPointer() const {
			return pPacketBuffer_->GetAt(EOffset::DataOffset);
		}
		inline const uint8_t* GetFlatDataPointer() const {
			return reinterpret_cast<uint8_t*>(pPacketBuffer_->GetAt(EOffset::DataOffset));
		}

		// before call this function, must call rbuilder.finish()
		inline void AssemblePacketBuffer(flatbuffers::FlatBufferBuilder& rbuilder, PACKETTYPE_T type) {
			PACKETSIZE_T packetSize = rbuilder.GetSize() + BASICPACKETSIZE;
			pPacketBuffer_->AppendData(reinterpret_cast<char*>(&packetSize), PACKETSIZE_S);
			pPacketBuffer_->AppendData(type);
			pPacketBuffer_->AppendData(reinterpret_cast<char*>(rbuilder.GetBufferPointer()), rbuilder.GetSize());
		}
		inline void AssemblePacketBuffer(char* pbuf, PACKETTYPE_T type, PACKETSIZE_T bufSize) {
			PACKETSIZE_T packetSize = bufSize + BASICPACKETSIZE;
			pPacketBuffer_->AppendData(reinterpret_cast<char*>(&packetSize), PACKETSIZE_S);
			pPacketBuffer_->AppendData(type);
			pPacketBuffer_->AppendData(pbuf, bufSize);
		}
	private:
		CDataBuffer* pPacketBuffer_;
	};
}

#endif // !__PROTOCOL_H__

