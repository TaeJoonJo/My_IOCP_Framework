
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdint.h>

enum EPacketType {
	Test_
};


class PACKET_HEADER {
public:
	char		size_;
	EPacketType type_;
};

#define PACKET_HEADER_SIZE sizeof(PACKET_HEADER)

class S2C_PACKET_TEST {
public:
	PACKET_HEADER header;
	uint32_t id;
};

class C2S_PACKET_TEST {
public:
	PACKET_HEADER header;
	uint32_t id;
};

#endif // !__PROTOCOL_H__

