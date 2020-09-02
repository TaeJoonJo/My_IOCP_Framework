
#ifndef __THREAD_H__
#define __THREAD_H__

#include <thread>
#include "../Include/Headers.h"

constexpr uint16_t MAX_THREAD = 16;

constexpr uint16_t WORKERTHREAD_NUM = MAX_THREAD - 2;

static volatile uint16_t _ThreadID = 0;

using THREADFUNC = std::function<void(LPVOID)>;

class CThread {
public:
	CThread();
	~CThread();
	CThread(const CThread& o);
	CThread& operator=(const CThread& o) {
		return *this;
	};
public:
	const bool Initalize(void (*func)(LPVOID), LPVOID lp);
	
	const bool	Start();
	void		Stop();
	const bool	Join();
private:
	static void _ThreadFunc(LPVOID lpc);
	const bool Run();
private:
	volatile bool isRun_;

	std::thread Thread_;
	LPVOID		lpParam_;
	THREADFUNC	Func_;

	uint16_t	ID_;
public:
	inline const bool IsRun() const {
		return isRun_;
	}
	inline const uint16_t GetID() const {
		return ID_;
	}
};

using THREADVECTOR = std::vector<CThread>;

class CMultiThread {
public:
	CMultiThread();
	~CMultiThread();
private:
	volatile bool isRun_;

	uint16_t ThreadNum_;
	LPVOID lpParam_;
	THREADVECTOR vThreads_;
public:
	const bool Initalize(uint16_t threadNum, void (*func)(LPVOID), LPVOID lp);

	const bool	Start();
	void		Stop();
	const bool	Join();
public:
	//const uint16_t GetCurrentThreadNum() const;
	inline const bool IsRun() const {
		return isRun_;
	}
};

#endif // !__THREAD_H__
