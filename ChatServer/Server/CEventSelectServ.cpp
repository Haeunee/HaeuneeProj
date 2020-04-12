#include "pch.h"
#include "CEventSelectServ.h"

CEventSelectServ::CEventSelectServ()
{
}


CEventSelectServ::~CEventSelectServ()
{
}

void CEventSelectServ::Init()
{
    SOCKET sock = MakeServ();
    evtServerBase->SetParents(this);

    CoInitialize(0);

    WSANETWORKEVENTS netEvent{};
    HANDLE accHandle = WSACreateEvent();
    WSAEventSelect(sock, accHandle, FD_ACCEPT);
    vector<Event*> evtThreadList{};

    int cnt{ threadCnt };
    for (int i = 0; i < cnt; ++i)
    {
        evtThreadList.push_back(new Event{});
        _beginthreadex(NULL, 0, CheckEvent, (LPVOID)evtThreadList[i], 0, NULL);
    }

    int idx{};
    int eventIdx{};
    while (true)
    {
        eventIdx = WSAWaitForMultipleEvents(1, &accHandle, FALSE, 100, FALSE);
        eventIdx -= WSA_WAIT_EVENT_0;

        if ((eventIdx == WSA_WAIT_FAILED) || (eventIdx == WSA_WAIT_TIMEOUT))
            continue;

        Event* eventList = evtThreadList[idx];
        WSAEnumNetworkEvents(sock, accHandle, &netEvent);

        if (netEvent.lNetworkEvents == FD_ACCEPT)
            AcceptProc(*eventList, eventIdx, sock);

        for (int i = 0; i < cnt; ++i)
        {
            if (evtThreadList[i]->nowIdx < WSA_MAXIMUM_WAIT_EVENTS)
            {
                idx = i;
                break;
            }
        }
    }
}

unsigned WINAPI CEventSelectServ::CheckEvent(LPVOID _eventList)
{
    CoInitialize(0);

    Event* eventList = (Event*)_eventList;
    WSANETWORKEVENTS netEvent{};

    int eventIdx{};
    while (true)
    {
        eventIdx = WSAWaitForMultipleEvents(eventList->nowIdx, eventList->eventHandles, FALSE, 100, FALSE);
        eventIdx -= WSA_WAIT_EVENT_0;

        if ((eventIdx == WSA_WAIT_FAILED) || (eventIdx == WSA_WAIT_TIMEOUT))
            continue;

        WSAEnumNetworkEvents(eventList->socks[eventIdx], eventList->eventHandles[eventIdx], &netEvent);

        switch (netEvent.lNetworkEvents)
        {
        case FD_READ:
            ReadProc(*eventList, eventIdx);
            break;
        case FD_CLOSE:
            CloseProc(*eventList, eventIdx);
            break;
        }
    }
    return 0;
}

void CEventSelectServ::AcceptProc(Event& _evtList, int _idx, SOCKET _sock)
{
    AutoMutex mutex{};
    SOCKADDR_IN clntAddr{};
    unsigned long recvBytes{};
    int len = sizeof(clntAddr);
    _evtList.socks[_evtList.nowIdx] = accept(_sock, (SOCKADDR*)&clntAddr, &len);
    if (0 != WSAIoctl(_evtList.socks[_evtList.nowIdx], SIO_KEEPALIVE_VALS, &evtServerBase->tcpKeepAl, sizeof(tcp_keepalive), 0, 0, &recvBytes, NULL, NULL))
        PrintError("WSAIoctl()");
    
    _evtList.eventHandles[_evtList.nowIdx] = WSACreateEvent();
    WSAEventSelect(_evtList.socks[_evtList.nowIdx], _evtList.eventHandles[_evtList.nowIdx], FD_READ | FD_CLOSE);

    evtServerBase->InsertUser(_evtList.socks[_evtList.nowIdx]);

    ++_evtList.nowIdx;
}

void CEventSelectServ::ReadProc(Event& _evtList, int _idx)
{
    AutoMutex mutex{};
    char buffer[BUF_SIZE];
    SOCKET sock = _evtList.socks[_idx];

    memset(buffer, 0, BUF_SIZE);
    if (0 >= recv(sock, buffer, BUF_SIZE, 0))
        return;

    cout << "RecvMsg (userIdx : " << _idx << ")" << endl;

    evtServerBase->CheckMsg(buffer, sock);
}

void CEventSelectServ::CloseProc(Event& _evtList, int _idx)
{
    AutoMutex mutex{};
    WSACloseEvent(_evtList.eventHandles[_idx]);
    closesocket(_evtList.socks[_idx]);

    evtServerBase->OutUser(_evtList.socks[_idx]);

    --_evtList.nowIdx;
    _evtList.eventHandles[_idx] = _evtList.eventHandles[_evtList.nowIdx];
    _evtList.socks[_idx] = _evtList.socks[_evtList.nowIdx];
}

void CEventSelectServ::SendMsg(char _buf[BUF_SIZE], SOCKET _sock)
{
    int sendSize, leftSize, idx = 0;
    leftSize = BUF_SIZE;
    while (leftSize > 0)
    {
        sendSize = send(_sock, &_buf[idx], leftSize, 0);
        leftSize -= sendSize;
        idx += sendSize;
    }
}
