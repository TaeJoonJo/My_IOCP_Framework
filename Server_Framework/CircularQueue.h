
#ifndef __CIRCULARQUEUE_H__
#define __CIRCULARQUEUE_H__

#include <iostream>
#include "Const.h"

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
private:
	std::array<T, MAX_BUFFER>	Queue_;

	// Front 자리는 비워둔다.
	uint32_t					FrontIndex_;
	uint32_t					RearIndex_;
public:
	// 데이터가 있는 인덱스부터 몇번째 데이터를 얻어올지
	inline const T* GetData(uint32_t index) const {
		return &Queue_[(FrontIndex_ + 1 + index) % MAX_BUFFER];
	}
	inline const uint32_t GetNowQueueSize() const {
		uint32_t front = ((FrontIndex_ + 1) % MAX_BUFFER);
		return front <= RearIndex_ ? (RearIndex_ - front) : ((MAX_BUFFER - front) + RearIndex_);
	}
	inline const uint32_t GetRemainQueueSize() const {
		return MAX_BUFFER - GetNowQueueSize();
	}
};

#endif // !__CIRCULARQUEUE_H__
#include "CircularQueue.hpp"