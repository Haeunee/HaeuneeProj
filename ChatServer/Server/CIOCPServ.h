#pragma once
#include "CServerBase.h"
#include "ChatServDefine.h"

enum eRW_MODE
{
    READ        = 3,
    WRITE       = 5,
};

typedef struct
{
    SOCKET clntSock;
    SOCKADDR_IN clntAddr;
} HANDLE_DATA, *LP_HANDLE_DATA;

typedef struct
{
    OVERLAPPED overlapped{};
    WSABUF wsaBuf{};
    char buf[BUF_SIZE];
    eRW_MODE rwMode{};
} IO_DATA, *LP_IO_DATA;

class CIOCPServ : public Server
{
public:
    CIOCPServ();
    ~CIOCPServ();
	static std::shared_ptr<CServerBase> iocpServBase;
    void Init();
    void SendMsg(char _buf[BUF_SIZE], SOCKET _sock);
    static unsigned WINAPI RecvMsg(LPVOID pComPort);
};

