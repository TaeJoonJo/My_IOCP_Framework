
#ifndef __CIRCULARQUEUE_H__
#define __CIRCULARQUEUE_H__

#include "../Include/Headers.h"

constexpr uint32_t MAX_QUEUE = 1024;

template<typename T>
class CCircularQueue {
public:
	CCircularQueue() {
		Clear();
	}
	~CCircularQueue() {
		Clear();
	}
	CCircularQueue(const CCircularQueue& o) {
		Clear();
	}
	CCircularQueue& operator=(const CCircularQueue& o) = delete;
public:
	void Clear();
	const bool Enq(const T* pdata, uint32_t dataSize);
	const bool Deq(T* pdata, uint32_t dataSize);
	const bool Deq(uint32_t dataSize);
private:
	std::array<T, MAX_QUEUE>	Queue_;

	// Front 자리는 비워둔다.
	uint32_t					FrontIndex_;
	uint32_t					RearIndex_;
public:
	// 데이터가 있는 인덱스부터 몇번째 데이터를 얻어올지
	inline const T* GetAt(uint32_t index) const {
		return &Queue_[(FrontIndex_ + 1 + index) % MAX_QUEUE];
	}
	inline T* GetAt(uint32_t index) {
		return &Queue_[(FrontIndex_ + 1 + index) % MAX_QUEUE];
	}
	inline const uint32_t GetNowQueueSize() const {
		if (FrontIndex_ == RearIndex_) return 0;
		uint32_t front = ((FrontIndex_ + 1) % MAX_QUEUE);
		return front <= RearIndex_ ? (RearIndex_ - FrontIndex_) : ((MAX_QUEUE - front) + RearIndex_);
	}
	inline const uint32_t GetRemainQueueSize() const {
		return (MAX_QUEUE - 1) - GetNowQueueSize();
	}
};

#endif // !__CIRCULARQUEUE_H__
#include "CircularQueue.hpp"