
#ifndef __LOCK_H__
#define __LOCK_H__

#include "../Include/Headers.h"

class CLock {
public:
	CLock() = default;
	virtual ~CLock() = default;
public:
	//virtual bool Initalize() = 0;
	//virtual bool Destroy() = 0;

	virtual void Lock() = 0;
	virtual void UnLock() = 0;
};

class CCSLock :
	public CLock
{
public:
	CCSLock() {
		InitializeCriticalSection(&CS_);
	};
	~CCSLock() {
		DeleteCriticalSection(&CS_);
	};
private:
	CRITICAL_SECTION CS_;
public:
	inline void Lock() override {
		EnterCriticalSection(&CS_);
	}
	inline void UnLock() override {
		LeaveCriticalSection(&CS_);
	}
};

class CMutexLock :
	public CLock
{
public:
	CMutexLock() = default;
	~CMutexLock() = default;
private:
	std::mutex Mutex_;
public:
	inline void Lock() override {
		Mutex_.lock();
	}
	inline void UnLock() override {
		Mutex_.unlock();
	}
};

#endif // !__LOCK_H__
