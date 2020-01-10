#include "Thread.h"
#include "../Include/Const.h"

CThread::CThread()
	: isRun_(false),
	ID_(UINT16_MAX), Func_(nullptr), lpParam_(nullptr)
{
}

CThread::~CThread()
{
	this->Stop();
}

CThread::CThread(const CThread& o)
	: isRun_(false),
	ID_(UINT16_MAX), lpParam_(nullptr)
{
}

const bool CThread::Initalize(void (*func)(LPVOID), LPVOID lp)
{
	ID_ = _ThreadID++;
	Func_ = func;
	lpParam_ = lp;

	return true;
}

const bool CThread::Start()
{
	if (true == isRun_)
		return false;

	isRun_ = true;

	//if(false == this->Run())
	//	return false;

	this->Thread_ = std::thread{ _ThreadFunc, (LPVOID)this };

	return true;
}

// 실행중인 스레드 중단?
const bool CThread::Stop()
{
	isRun_ = false;

	return true;
}

const bool CThread::Join()
{
	if (false == this->isRun_)
		return false;

	Thread_.join();

	isRun_ = false;

	return true;
}

void CThread::_ThreadFunc(LPVOID lp)
{
	CThread* pthis = reinterpret_cast<CThread*>(lp);
	if (nullptr != pthis)
		pthis->Run();
}

const bool CThread::Run()
{
	if (nullptr == this->Func_) {
		this->isRun_ = false;
		return false;
	}
	
	this->Func_(lpParam_);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

CMultiThread::CMultiThread()
	: isRun_(false), ThreadNum_(0)
	, lpParam_(nullptr)
{
}

CMultiThread::~CMultiThread()
{
}

const bool CMultiThread::Initalize(uint16_t threadNum, void(*func)(LPVOID), LPVOID lp)
{
	// 현재 실행되고 있는 스레드 개수가 threadNum을 더했을때 최대 스레드 개수를 넘어가면 return false 하자

	bool brt = true;

	ThreadNum_ = threadNum;
	lpParam_ = lp;

	vThreads_.assign(static_cast<size_t>(threadNum), CThread());
	for (auto& t : vThreads_) {
		if (false == t.Initalize(func, lpParam_)) {
			brt = false;
		}
	}

	return brt;
}

const bool CMultiThread::Start()
{
	if (true == isRun_)
		return false;

	bool brt = true;

	isRun_ = true;

	for (auto& t : vThreads_) {
		if (false == t.Start()) {
			brt = false;
		}
	}

	return brt;
}

const bool CMultiThread::Stop()
{
	bool brt = true;

	for (auto& t : vThreads_) {
		if (false == t.Stop())
			brt = false;
	}

	isRun_ = false;

	return brt;
}

const bool CMultiThread::Join()
{
	bool brt = true;

	for (auto& t : vThreads_) {
		if (false == t.Join()) {
			brt = false;
		}
	}

	return brt;
}
