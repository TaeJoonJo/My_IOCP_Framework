
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "../Include/Headers.h"

namespace Singleton {

	template<typename T>
	class CDynamicSingleton {
	protected:
		CDynamicSingleton() {};
		~CDynamicSingleton() {};
		CDynamicSingleton(const T& o) {};
		T& operator=(const T& o) {};
	public:
		static std::shared_ptr<T> GetInstance() {
			static std::shared_ptr<T> ins{ new T };
			return ins;
		}
	};

}

#endif // !__SINGLETON_H__
