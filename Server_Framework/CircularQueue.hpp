

template<typename T>
inline void CCircularQueue<T>::Clear()
{
	//memset(Queue_, 0x00, MAX_BUFFER);
	Queue_.fill(0);
	FrontIndex_ = RearIndex_ = 0;
}

template<typename T>
inline const bool CCircularQueue<T>::Enq(const T* pdata, uint32_t dataSize)
{
	if (dataSize > GetRemainQueueSize())
		return false;

	const T* now = pdata;

	RearIndex_ = (RearIndex_ + 1) % MAX_BUFFER;

	if (RearIndex_ < FrontIndex_) {
		memcpy((void*)Queue_[RearIndex_], now, dataSize);
		RearIndex_ += dataSize - 1;
	}
	else {
		uint32_t reartoendSize = MAX_BUFFER - (RearIndex_ + 1);
		memcpy((void*)Queue_[RearIndex_], now, reartoendSize);
		dataSize -= reartoendSize;
		now += dataSize;
		memcpy((void*)Queue_[0], now, dataSize);
		RearIndex_ = dataSize - 1;
	}

	return true;
}

template<typename T>
inline const bool CCircularQueue<T>::Deq(T* pdata, uint32_t dataSize)
{
	if (dataSize < GetNowQueueSize())
		return false;

	T* now = pdata;

	FrontIndex_ = (FrontIndex_ + 1) % MAX_BUFFER;
	if (FrontIndex_ < RearIndex_) {
		memcpy(now, (const void*)Queue_[FrontIndex_], dataSize);
		FrontIndex_ += (dataSize % MAX_BUFFER);
	}
	else {
		uint32_t fronttoendSize = MAX_BUFFER - (FrontIndex_ + 1);
		memcpy(now, (const void*)Queue_[FrontIndex_], fronttoendSize);
		dataSize -= fronttoendSize;
		now += dataSize;
		memcpy(now, (const void*)Queue_[0], dataSize);
		FrontIndex_ = dataSize - 1;
	}

	return true;
}
