// main.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
// use typedef in <stdint.h>

#include "../Include/Headers.h"
#include "../Include/Define.h"
#include "../Common/LogSystem.h"
#include <conio.h>
#include "ServerManager.h"

constexpr uint8_t MAIN_THREAD_SLEEP_TIME = 128;

int main()
{
	std::unique_ptr<CServerManager> pserverMgr = std::make_unique<CServerManager>();
	std::shared_ptr<CLogSystem> plogSystem = CLogSystem::GetInstance();

	plogSystem->Initalize(DEFALUT_LOG_PATH);
	
	if (false == pserverMgr->StartServer()) {
		_LERROR("StartServer()");
	}

	_LINFO("Press 'q' to Stop Server");
	while (true)
	{
		if (kbhit()) 
		{
			if (getch() == 'q') 
			{
				pserverMgr->StopServer();
				break;
			}
		}
		Sleep(MAIN_THREAD_SLEEP_TIME);
	}

	if (false == pserverMgr->CloseServer()) {
		_LERROR("CloseServer()");
	}

	_LINFO("End main");

    return 0;
}