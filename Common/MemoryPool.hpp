
#ifndef __MEMORYPOOL_HPP__
#define __MEMORYPOOL_HPP__

#include "../Include/Define.h"

template<typename T, uint32_t poolSize>
inline const bool CNonDynamicMemoryPool<T, poolSize>::Initalize()
{

	return false;
}

template<typename T, uint32_t poolSize>
inline const bool CNonDynamicMemoryPool<T, poolSize>::Destroy()
{

	return false;
}

template<typename T, uint32_t poolSize>
inline T* CNonDynamicMemoryPool<T, poolSize>::GetMemory()
{
	if (0 == NotActivedMemoryNum_) {
		return nullptr;
	}

	T* pmemory = NotActivedMemoryQueue_.front();
	NotActivedMemoryQueue_.pop();
	--NotActivedMemoryNum_;
	++ActivedMemoryNum_;

	return pmemory;
}

template<typename T, uint32_t poolSize>
inline void CNonDynamicMemoryPool<T, poolSize>::ReturnMemory(T* pmemory)
{
	// 기존에 있던 데이터인지 검사를 해야 하나?

	if (PoolSize_ == NotActivedMemoryNum_) {
		return;
	}

	NotActivedMemoryQueue_.push(pmemory);
	++NotActivedMemoryNum_;
	--ActivedMemoryNum_;

}


//////////////////////////////////////////////////////////////////////////////////////////////


template<typename T, uint32_t poolSize>
CDynamicMemoryPool<T, poolSize>::CDynamicMemoryPool()
	: MemorySize_(sizeof(T))
	, PoolSize_(poolSize)
	, NotActivedMemoryNum_(poolSize), ActivedMemoryNum_(0)
{

}

template<typename T, uint32_t poolSize>
inline const bool CDynamicMemoryPool<T, poolSize>::Initalize()
{
	for (int i = PoolSize_ - 1; i >= 0; --i) {
		MemoryArray_[i] = new T;
		NotActivedMemoryQueue_.push(MemoryArray_[i]);
	}

	NotActivedMemoryNum_ = PoolSize_;
	ActivedMemoryNum_ = 0;

	return false;
}

template<typename T, uint32_t poolSize>
inline const bool CDynamicMemoryPool<T, poolSize>::Destroy()
{
	for (int i = 0; i < PoolSize_; ++i) {
		_RELEASEPTR(MemoryArray_[i]);
	}

	ActivedMemoryNum_ = NotActivedMemoryNum_ = 0;

	return false;
}

template<typename T, uint32_t poolSize>
inline T* CDynamicMemoryPool<T, poolSize>::GetMemory()
{
	if (0 == NotActivedMemoryNum_) {
		return nullptr;
	}

	T* pmemory = NotActivedMemoryQueue_.front();
	NotActivedMemoryQueue_.pop();
	--NotActivedMemoryNum_;
	++ActivedMemoryNum_;

	return pmemory;
}	

template<typename T, uint32_t poolSize>
inline void CDynamicMemoryPool<T, poolSize>::ReturnMemory(T* pmemory)
{
	// 기존에 있던 데이터인지 검사를 해야 하나?

	if (PoolSize_ == NotActivedMemoryNum_) {
		return;
	}

	NotActivedMemoryQueue_.push(pmemory);
	++NotActivedMemoryNum_;
	--ActivedMemoryNum_;

}

#endif // !__MEMORYPOOL_HPP__