
#ifndef __FREELIST_H__
#define __FREELIST_H__

// TODO : list or array , blocking or nonblocking
template<typename T>
class CFreelist {
public:
	CFreelist();
	~CFreelist();
	CFreelist(const CFreelist& o) = delete;
	CFreelist& operator=(const CFreelist& o) = delete;
public:

private:
	
};

#include "Freelist.hpp"
#endif // !__FREELIST_H__
