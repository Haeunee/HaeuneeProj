#pragma once
#include "ChatServDefine.h"
#include "CServerBase.h"

#define BUF_SIZE 1024

struct Event
{
    SOCKET socks[WSA_MAXIMUM_WAIT_EVENTS];
    HANDLE eventHandles[WSA_MAXIMUM_WAIT_EVENTS];
    int nowIdx{};
};

class CEventSelectServ : public Server
{
public:
    CEventSelectServ();
    ~CEventSelectServ();

    static CServerBase* evtServerBase;

    void Init();

    static unsigned WINAPI CheckEvent(LPVOID _sock);

    static void AcceptProc(Event& _evtList, int _idx, SOCKET _sock);
    static void ReadProc(Event& _evtList, int _idx);
    static void CloseProc(Event& _evtList, int _idx);
    void SendMsg(char _buf[BUF_SIZE], SOCKET _sock);
};
