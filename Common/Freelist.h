
#ifndef __FREELIST_H__
#define __FREELIST_H__

#include "../Include/Headers.h"
#include "Lock.h"

template<typename T>
class CFreelist {
public:
	explicit CFreelist(uint32_t num);
	~CFreelist();
	CFreelist(const CFreelist& o) = delete;
	CFreelist& operator=(const CFreelist& o) = delete;
private:
	struct SData {
		SData() : Data_(), IsUse_(false), pNext_(nullptr) {}
		T				Data_;
		volatile bool	IsUse_;
		SData*			pNext_;
	};
public:
	const bool Clear();
	const T* GetData();
	const bool ReturnData(T* preturnData);
private:
	SData* pFront_;
	CMutexLock Lock_;
};

#endif // !__FREELIST_H__
#include "Freelist.hpp"