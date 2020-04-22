// Chat_Server_Serv.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include "CEventSelectServ.h"
#include "CIOCPServ.h"

std::shared_ptr<CServerBase> CIOCPServ::iocpServBase(new CServerBase{});
std::shared_ptr<CServerBase> CEventSelectServ::evtServerBase(new CServerBase{});

enum eServerType
{
    EVENT_SEL       = 1,
    IOCP            = 2,
};
int main()
{
    int servType{};
    int threadCnt{};
    while (true)
    {
        cout << "Select Server" << endl;
        cout << "1. EventSel    2. IOCP" << endl;
        cin >> servType;
        cout << "Thread Cnt(0 : Thread Cnt = CPU Cnt)" << endl;
        cin >> threadCnt;

        std::shared_ptr<Server> serv{};
        if (eServerType::EVENT_SEL == servType)
			serv = std::shared_ptr<Server>(new CEventSelectServ{});
        else if (eServerType::IOCP == servType)
            serv = std::shared_ptr<Server>(new CIOCPServ{});
        else
            continue;

        serv->SetThreadCnt(threadCnt);
        serv->Init();
    }
}
