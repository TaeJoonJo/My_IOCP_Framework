
#ifndef __IOCP_H__
#define __IOCP_H__

#include "../Common/Singleton.h"

class CIOCompletionPort : public
	Singleton::CDynamicSingleton<CIOCompletionPort>
{
public:
	CIOCompletionPort();
	~CIOCompletionPort();
public:
	const bool StartIOCP();

	inline const HANDLE GetIOCP() const {
		return IOCP_;
	}
private:
	HANDLE IOCP_;
};

#endif // !__IOCP_H__
