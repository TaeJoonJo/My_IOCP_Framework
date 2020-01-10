#include "ThreadPool.h"

CThreadPool::CThreadPool()
	: MaxThreadNum_(0)
{
}

CThreadPool::~CThreadPool()
{
	this->Clear();
}

const bool CThreadPool::Initalize(const int maxThreadNum)
{
	if (false == this->Clear())
		return false;

	MaxThreadNum_ = maxThreadNum;

	this->ThreadPool_.reserve(static_cast<size_t>(MaxThreadNum_));

	return true;
}

const bool CThreadPool::Clear()
{
	if (0 == setThreads_.size())
		return true;

	std::unordered_set<CThread*>::iterator iter;
	
	for (iter = setThreads_.begin(); iter != setThreads_.end(); ++iter) {
	
	}

	return true;
}
