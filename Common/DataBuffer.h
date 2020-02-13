
#ifndef __DATABUFFER_H__
#define __DATABUFFER_H__

#include <stdint.h>
#include <memory>

// 예외까지 처리해주지 않음
class CDataBuffer {
public:
	CDataBuffer();
	CDataBuffer(char* pbuf, uint32_t bufferSize);
	explicit CDataBuffer(char* pbuf);
	explicit CDataBuffer(uint32_t allocSize);
	CDataBuffer(const CDataBuffer& o);
	~CDataBuffer();
public:
	void AllocateBuffer(uint32_t allocSize);
	void ReleaseBuffer();
	void ClearBuffer();

	void CopyBuffer(const char* pbuf, uint32_t bufferSize);

	const bool AppendData(const char* pdataBuffer, const uint32_t appendSize);
	template<typename T> const bool AppendData(T& appendData);

	constexpr void SetBufferPointer(char* pbuf, uint32_t bufferSize) {
		pBuf_ = pbuf;
		MaxSize_ = bufferSize;
		NowSize_ = 0;
	}

	constexpr char* GetAt(uint32_t offset) const {
		return &pBuf_[offset];
	}
	constexpr char* GetBufferPointer() const {
		return pBuf_;
	}
	constexpr const uint32_t GetNowSize() const {
		return NowSize_;
	}
	constexpr const uint32_t GetMaxSize() const {
		return MaxSize_;
	}
	constexpr const uint32_t GetRemainingSize() const {
		return MaxSize_ - NowSize_;
	}
private:
	char* pBuf_;
	uint32_t NowSize_;
	uint32_t MaxSize_;
};

#endif // !__DATABUFFER_H__


template<typename T>
inline const bool CDataBuffer::AppendData(T& appendData)
{
	uint32_t dataSize = sizeof(T);
	if (dataSize > this->GetRemainingSize()) {
		return false;
	}
	memcpy(pBuf_, &appendData, dataSize);

	return true;
}