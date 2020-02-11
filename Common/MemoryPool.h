
#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__

#include <array>
#include <queue>

template<typename T, uint32_t poolSize>
class CNonDynamicMemoryPool {
public:
	CNonDynamicMemoryPool() {};
	~CNonDynamicMemoryPool() {};
	CNonDynamicMemoryPool(const CNonDynamicMemoryPool&) = delete;
	CNonDynamicMemoryPool& operator=(const CNonDynamicMemoryPool&) = delete;
private:
public:
	const bool Initalize();
	const bool Destroy();
	T* GetMemory();
	void ReturnMemory(T* pmemory);

	inline const uint32_t GetMemorySize() const {
		return MemorySize_;
	}
	inline const uint32_t GetPoolSize() const {
		return PoolSize_;
	}
	inline const uint32_t GetMemoryPoolSize() const {
		return PoolSize_ * MemorySize_;
	}
	inline const uint32_t GetActivedMemoryNum() const {
		return ActivedMemoryNum_;
	}
	inline const uint32_t GetActivedMemorySize() const {
		return ActivedMemoryNum_ * MemorySize_;
	}
	inline const uint32_t GetNotActivedMemoryNum() const {
		return NotActivedMemoryNum_;
	}
	inline const uint32_t GetNotActivedMemorySize() const {
		return NotActivedMemoryNum_ * MemorySize_;
	}
private:
	std::array<T, poolSize> MemoryArray_;
	std::queue<T*>			NotActivedMemoryQueue_;

	uint32_t	MemorySize_;
	
	uint32_t	PoolSize_;
	uint32_t	ActivedMemoryNum_;
	uint32_t	NotActivedMemoryNum_;
};


template<typename T, uint32_t poolSize>
class CDynamicMemoryPool {
public:
	CDynamicMemoryPool();
	~CDynamicMemoryPool() { Destroy(); };
	CDynamicMemoryPool(const CDynamicMemoryPool&) = delete;
	CDynamicMemoryPool& operator=(const CDynamicMemoryPool&) = delete;
private:
public:
	const bool Initalize();
	const bool Destroy();
	T* GetMemory();
	void ReturnMemory(T* pmemory);

	inline const uint32_t GetMemorySize() const {
		return MemorySize_;
	}
	inline const uint32_t GetPoolSize() const {
		return PoolSize_;
	}
	inline const uint32_t GetMemoryPoolSize() const {
		return PoolSize_ * MemorySize_;
	}
	inline const uint32_t GetActivedMemoryNum() const {
		return ActivedMemoryNum_;
	}
	inline const uint32_t GetActivedMemorySize() const {
		return ActivedMemoryNum_ * MemorySize_;
	}
	inline const uint32_t GetNotActivedMemoryNum() const {
		return NotActivedMemoryNum_;
	}
	inline const uint32_t GetNotActivedMemorySize() const {
		return NotActivedMemoryNum_ * MemorySize_;
	}
private:
	std::array<T*, poolSize> MemoryArray_;
	// 처음에 이 큐를 어떻게 array의 값으로 채울 것인가?
	std::queue<T*>			NotActivedMemoryQueue_;

	uint32_t	MemorySize_;

	uint32_t	PoolSize_;
	uint32_t	ActivedMemoryNum_;
	uint32_t	NotActivedMemoryNum_;
};

#endif // !__MEMORYPOOL_H__
#include "MemoryPool.hpp"