#include "pch.h"
#include "CIOCPServ.h"

CIOCPServ::CIOCPServ()
{
}


CIOCPServ::~CIOCPServ()
{
}

void CIOCPServ::Init()
{
    SOCKET sock{ MakeServ() };
    LP_HANDLE_DATA handleData{};
    LP_IO_DATA ioData{};
    HANDLE ioComPort{ CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0,0) };
    unsigned long recvBytes, flags = 0;
    iocpServBase->SetParents(this);
    
    for (int i = 0; i < threadCnt; ++i)
        _beginthreadex(NULL, 0, RecvMsg, (LPVOID)ioComPort, 0, NULL);

    while (true)
    {
        SOCKET clntSock{};
        SOCKADDR clntAddr{};
        int addrLen{ sizeof(clntAddr) };

        clntSock = accept(sock, (SOCKADDR*)&clntAddr, &addrLen);
        if (0 != WSAIoctl(clntSock, SIO_KEEPALIVE_VALS, &iocpServBase->tcpKeepAl, sizeof(tcp_keepalive), 0, 0, &recvBytes, NULL, NULL))
            PrintError("WSAIoctl()");
        handleData = new HANDLE_DATA;
        handleData->clntSock = clntSock;
        memcpy(&(handleData->clntAddr), &clntAddr, addrLen);

        CreateIoCompletionPort((HANDLE)clntSock, ioComPort, (DWORD)handleData, 0);

        ioData = new IO_DATA;
        memset(&(ioData->overlapped), 0, sizeof(OVERLAPPED));
        ioData->wsaBuf.len = BUF_SIZE;
        ioData->wsaBuf.buf = ioData->buf;
        ioData->rwMode = eRW_MODE::READ;
        WSARecv(handleData->clntSock, &(ioData->wsaBuf), 1, &recvBytes, &flags, &(ioData->overlapped), NULL);

        iocpServBase->InsertUser(clntSock);
    }
}

void CIOCPServ::SendMsg(char _buf[BUF_SIZE], SOCKET _sock)
{
    LP_IO_DATA ioData;
    ioData = new IO_DATA;

    memcpy(ioData->buf, _buf, BUF_SIZE);
    memset(&(ioData->overlapped), 0, sizeof(OVERLAPPED));
    ioData->wsaBuf.len = BUF_SIZE;
    ioData->wsaBuf.buf = ioData->buf;
    ioData->rwMode = eRW_MODE::WRITE;
    WSASend(_sock, &(ioData->wsaBuf), 1, NULL, 0, &(ioData->overlapped), NULL);
}

unsigned WINAPI CIOCPServ::RecvMsg(LPVOID pComPort)
{
    CoInitialize(0);

    HANDLE ioComPort = (HANDLE)pComPort;
    DWORD byteTrans;
    LP_HANDLE_DATA handleData;
    LP_IO_DATA ioData;
    DWORD flags = 0;

    bool connErr;
    while (true)
    {
        connErr = GetQueuedCompletionStatus(ioComPort, &byteTrans, (LPDWORD)&handleData, (LPOVERLAPPED*)&ioData, INFINITE);

        int recvIdx{};
        memcpy(&recvIdx, ioData->buf, sizeof(int));

        if (ioData->rwMode == eRW_MODE::READ)
        {
            {
                AutoMutex mutex{};
                if (byteTrans <= 0)
                {
                    iocpServBase->OutUser(handleData->clntSock);
                    delete(ioData);
                    delete(handleData);
                    continue;
                }
                iocpServBase->CheckMsg(ioData->buf, handleData->clntSock);
            }

            ioData = new IO_DATA;
            memset(&(ioData->overlapped), 0, sizeof(OVERLAPPED));
            ioData->wsaBuf.len = BUF_SIZE;
            ioData->wsaBuf.buf = ioData->buf;
            ioData->rwMode = eRW_MODE::READ;
            WSARecv(handleData->clntSock, &(ioData->wsaBuf), 1, NULL, &flags, &(ioData->overlapped), NULL);
        }
        else
        {
            delete(ioData);
            cout << "sendMsg" << endl;
        }
    }
    return 0;
}
