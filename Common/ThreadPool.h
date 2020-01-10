
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Singleton.h"

class CThread;

/* 구현 보류 */
class CThreadPool : 
	public Singleton::CDynamicSingleton<CThreadPool>
{
public:
	CThreadPool();
	~CThreadPool();
private:
	UINT MaxThreadNum_;
	std::vector<CThread>			ThreadPool_;

	std::unordered_set<CThread*>	setThreads_;
public:
	const bool Initalize(const int maxThreadNum);
	const bool Clear();

	//const bool RunThread(void (*threadFunc)(), const int runThreadNum = 1);
	//static void	_RunThread(void (*threadFunc)(), const int runThreadNum = 1);
	//
	//const bool isRunnableThread();
};

#endif // !__THREADPOOL_H__
