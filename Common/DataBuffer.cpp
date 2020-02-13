#include "DataBuffer.h"
#include <Windows.h>

CDataBuffer::CDataBuffer()
	: pBuf_()
	, MaxSize_(), NowSize_()
{
}

CDataBuffer::CDataBuffer(char* pbuf, uint32_t bufferSize)
	: pBuf_(pbuf)
	, MaxSize_(bufferSize), NowSize_(0)
{
}

CDataBuffer::CDataBuffer(char* pbuf)
	: pBuf_(pbuf)
	, MaxSize_(0), NowSize_(0)
{
	
}

CDataBuffer::CDataBuffer(uint32_t allocSize)
	: MaxSize_(allocSize), NowSize_()
{
	pBuf_ = new char[allocSize] {};
}

CDataBuffer::CDataBuffer(const CDataBuffer& o)
{
	pBuf_ = o.pBuf_;
	NowSize_ = o.NowSize_;
	MaxSize_ = o.MaxSize_;
}

CDataBuffer::~CDataBuffer()
{
	if (pBuf_) {
		delete[] pBuf_;
	}
}

void CDataBuffer::AllocateBuffer(uint32_t allocSize)
{
	pBuf_ = new char[allocSize] {};

	MaxSize_ = allocSize;
	NowSize_ = 0;
}

void CDataBuffer::ReleaseBuffer()
{
	delete[] pBuf_;
	NowSize_ = MaxSize_ = 0;
}

void CDataBuffer::ClearBuffer()
{
	memset(pBuf_, 0x00, MaxSize_);
	NowSize_ = 0;
}

void CDataBuffer::CopyBuffer(const char* pbuf, uint32_t bufferSize)
{
	memcpy(pBuf_, pbuf, bufferSize);
	NowSize_ = bufferSize;
}

const bool CDataBuffer::AppendData(const char* pdataBuffer, const uint32_t appendSize)
{
	if (appendSize > this->GetRemainingSize()) {
		return false;
	}

	memcpy(&pBuf_[NowSize_], pdataBuffer, appendSize);
	NowSize_ += appendSize;

	return true;
}
