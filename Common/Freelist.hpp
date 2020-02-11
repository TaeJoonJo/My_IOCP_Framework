
#ifndef __FREELIST_HPP__
#define __FREELIST_HPP__

template<typename T>
CFreelist<T>::CFreelist(uint32_t dataNum)
	: pFront_(nullptr)
{
	_BEGINLOCKGUARD(Lock_.GetMutex())
	
		pFront_ = new SData();
		SData* pnow = pFront_;
		SData* pnewData = nullptr;
		for (uint32_t i = 0; i < dataNum; ++i) {
			pnewData = new SData();
			pnow->pNext_ = pnewData;
			pnow = pnewData;
		}

	_ENDLOCKGUARD
}

template<typename T>
CFreelist<T>::~CFreelist()
{
	Clear();

	delete pFront_;
}

template<typename T>
const bool CFreelist<T>::Clear()
{
	_BEGINLOCKGUARD(Lock_.GetMutex())
	
		SData* pnow = pFront_->pNext_;
		SData* pnext = pnow->pNext_;
		while (nullptr != pnow) {
			delete pnow;
			pnow = pnext;
			pnext = pnext->pNext_;
		}
	
	_ENDLOCKGUARD

	return true;
}

template<typename T>
T* CFreelist<T>::GetData()
{
	T* pdata = nullptr;

	_BEGINLOCKGUARD(Lock_.GetMutex())
	
		SData* pnow = pFront_->pNext_;
		while (nullptr != pnow) {
			if (false == pnow->IsUse_) {
				pdata = &pnow->Data_;
				pnow->IsUse_ = true;
				break;
			}
			pnow = pnow->pNext_;
		}
		if (nullptr == pdata) {
			SData* pnewData = new SData();
			if (nullptr == pnewData) {
				Lock_.UnLock();
				return pdata;
			}
			pdata = &pnewData->Data_;
			pnow->pNext_ = pnewData;
			pnewData->IsUse_ = true;
		}
	
	_ENDLOCKGUARD

	return pdata;
}

template<typename T>
const bool CFreelist<T>::ReturnData(T* preturnData)
{
	_BEGINLOCKGUARD(Lock_.GetMutex())
	
		SData* pnow = pFront_->pNext_;
		while (nullptr != pnow) {
			if (true == pnow->IsUse_ && &(pnow->Data_) == preturnData) {
				pnow->IsUse_ = false;
				break;
			}
			pnow = pnow->pNext_;
		}
	
	_ENDLOCKGUARD

	return true;
}

#endif // !__FREELIST_HPP__