
#ifndef __FREELIST_HPP__
#define __FREELIST_HPP__

template<typename T>
CFreelist<T>::CFreelist(uint32_t dataNum)
	: pFront_(nullptr)
{
	Lock_.Lock();
	{
		pFront_ = new SData();
		SData* pnow = pFront_;
		SData* pnewData = nullptr;
		for (uint32_t i = 0; i < dataNum; ++i) {
			pnewData = new SData();
			pnow->pNext_ = pnewData;
			pnow = pnewData;
		}
	}
	Lock_.UnLock();
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
	Lock_.Lock();
	{
		SData* pnow = pFront_->pNext_;
		SData* pnext = pnow->pNext_;
		while (nullptr != pnow) {
			delete pnow;
			pnow = pnext;
			pnext = pnext->pNext_;
		}
	}
	Lock_.UnLock();

	return true;
}

template<typename T>
T* CFreelist<T>::GetData()
{
	T* pdata = nullptr;

	Lock_.Lock();
	{
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
	}
	Lock_.UnLock();

	return pdata;
}

template<typename T>
const bool CFreelist<T>::ReturnData(T* preturnData)
{
	Lock_.Lock();
	{
		SData* pnow = pFront_->pNext_;
		while (nullptr != pnow) {
			if (true == pnow->IsUse_ && &(pnow->Data_) == preturnData) {
				pnow->IsUse_ = false;
				break;
			}
			pnow = pnow->pNext_;
		}
	}
	Lock_.UnLock();

	return true;
}

#endif // !__FREELIST_HPP__