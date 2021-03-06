// main.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
// use typedef in <stdint.h>

#include "../Include/Headers.h"
#include "../Include/Define.h"
#include "ServerManager.h"
#include "../Common/LogSystem.h"

int main()
{
	std::unique_ptr<CServerManager> pserverMgr = std::make_unique<CServerManager>();
	std::shared_ptr<CLogSystem> plogSystem = CLogSystem::GetInstance();

	plogSystem->Initalize(DEFALUT_LOG_PATH);

	_LINFO("main");
	
	if (false == pserverMgr->StartServer()) {
		_LERROR("StartServer()");
	}

	if (false == pserverMgr->CloseServer()) {
		_LERROR("CloseServer()");
	}

    return 0;
}