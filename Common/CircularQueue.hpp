
#ifndef __CIRCULARQUEUE_HPP__
#define __CIRCULARQUEUE_HPP__

template<typename T>
void CCircularQueue<T>::Clear()
{
	Queue_.fill(0);
	FrontIndex_ = RearIndex_ = 0;
}

template<typename T>
const bool CCircularQueue<T>::Enq(const T* pdata, uint32_t dataSize)
{
	if (dataSize > GetRemainQueueSize())
		return false;

	const T* now = pdata;

	RearIndex_ = (RearIndex_ + 1) % MAX_QUEUE;

	if (RearIndex_ < FrontIndex_) {
		memcpy((void*)&Queue_[RearIndex_], now, dataSize);
		RearIndex_ += dataSize - 1;
	}
	else {
		uint32_t reartoendSize = MAX_QUEUE - (RearIndex_);
		if (reartoendSize > dataSize) {
			memcpy((void*)&Queue_[RearIndex_], now, dataSize);
			RearIndex_ += dataSize - 1;
		}
		else {
			memcpy((void*)&Queue_[RearIndex_], now, reartoendSize);
			dataSize -= reartoendSize;
			now += reartoendSize;
			memcpy((void*)&Queue_[0], now, dataSize);
			RearIndex_ = dataSize - 1;
		}
	}

	return true;
}

template<typename T>
const bool CCircularQueue<T>::Deq(T* pdata, uint32_t dataSize)
{
	if (dataSize > GetNowQueueSize()) {
		return false;
	}

	T* now = pdata;

	FrontIndex_ = (FrontIndex_ + 1) % MAX_QUEUE;
	if (FrontIndex_ < RearIndex_) {
		memcpy(now, (const void*)&Queue_[FrontIndex_], dataSize);
		FrontIndex_ += (dataSize - 1) % MAX_QUEUE;
	}
	else {
		uint32_t fronttoendSize = MAX_BUFFER - (FrontIndex_ + 1);
		if (dataSize < fronttoendSize) {
			memcpy(now, (const void*)&Queue_[FrontIndex_], dataSize);
			FrontIndex_ += (dataSize - 1) % MAX_QUEUE;
		}
		else {
			memcpy(now, (const void*)&Queue_[FrontIndex_], fronttoendSize);
			dataSize -= fronttoendSize;
			now += dataSize;
			memcpy(now, (const void*)&Queue_[0], dataSize);
			FrontIndex_ = dataSize - 1;
		}
	}

	return true;
}

// just pop
template<typename T>
const bool CCircularQueue<T>::Pop(uint32_t dataSize)
{
	if (dataSize > GetNowQueueSize()) {
		return false;
	}

	FrontIndex_ = (FrontIndex_ + 1) % MAX_QUEUE;
	if (FrontIndex_ < RearIndex_) {
		FrontIndex_ += (dataSize - 1) % MAX_QUEUE;
	}
	else {
		uint32_t fronttoendSize = MAX_BUFFER - (FrontIndex_ + 1);
		if (dataSize < fronttoendSize) {
			FrontIndex_ += (dataSize - 1) % MAX_QUEUE;
		}
		else {
			dataSize -= fronttoendSize;
			FrontIndex_ = dataSize - 1;
		}
	}

	return false;
}

#endif // !__CIRCULARQUEUE_HPP__