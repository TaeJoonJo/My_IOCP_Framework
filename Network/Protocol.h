
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdint.h>
#include "../Include/Const.h"

enum EPacketType {
	Test_
};

class PACKET_HEADER {
public:
	explicit PACKET_HEADER(char packetSize, EPacketType packetType)
		: size_(packetSize)
		, type_(packetType)
	{}
public:
	char		size_;
	EPacketType type_;
};

#define PACKET_HEADER_SIZE sizeof(PACKET_HEADER)

class S2C_PACKET_TEST {
public:
	explicit S2C_PACKET_TEST(char packetSize, EPacketType packetType)
		: header(packetSize, packetType)
		, str()
	{}
public:
	PACKET_HEADER header;
	TCHAR str[MAX_STRING];
};

class C2S_PACKET_TEST {
	explicit C2S_PACKET_TEST(char packetSize, EPacketType packetType)
		: header(packetSize, packetType)
		, str()
	{}
public:
	PACKET_HEADER header;
	TCHAR str[MAX_STRING];
};

#endif // !__PROTOCOL_H__

