#include "stdafx.h"
#include "CServConn.h"
#include "CChatMgr.h"
#include "ChatServerDlg.h"
#include <process.h>
#include <mstcpip.h>

CServConn::CServConn()
{
}

CServConn::~CServConn()
{
    closesocket(sock);
    WSACleanup();
}


void CServConn::ConnectServ()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
        ErrorHandling(L"WSAStartup() error!");

    unsigned long recvBytes{};
	tcp_keepalive tcpKeepAl{};
    sock = socket(PF_INET, SOCK_STREAM, 0);
    WSAIoctl(sock, SIO_KEEPALIVE_VALS, &tcpKeepAl, sizeof(tcp_keepalive), 0, 0, &recvBytes, NULL, NULL);
    if(INVALID_SOCKET == sock)
        ErrorHandling(L"socket() error!");
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "10.10.10.52", &addr.sin_addr.s_addr);
    addr.sin_port = htons(10000);

    if(SOCKET_ERROR == connect(sock, (SOCKADDR*)&addr, sizeof(addr)))
        ErrorHandling(L"connect() error!");

    SOCKET* recvSock = &sock;
    HANDLE recvThread = (HANDLE)_beginthreadex(NULL, 0, RecvChat, (void*)recvSock, 0, NULL);
}

void CServConn::ErrorHandling(LPCTSTR _err)
{
   CChatMgr::GetInstance()->PrintMsg(_err);
   exit(1);
}

unsigned WINAPI CServConn::RecvChat(void * _sock)
{
    SOCKET socket = *(SOCKET*)_sock;
    char recvMsg[BUF_SIZE]{};

    while (true)
    {
        if (recv(socket, recvMsg, BUF_SIZE, 0) <= 0)
            exit(1);

        sMessage message{};
        memcpy(&message, recvMsg, sizeof(message));

        sRoomMessage roomMsg{};
        memcpy(&roomMsg, recvMsg, sizeof(roomMsg));

        switch (static_cast<eChatFlag>(message.idx))
        {
        case eChatFlag::SEL_NICK:
            GlobalChat(message);
            break;
        case eChatFlag::GLOBAL_CHAT:
            GlobalChat(message);
            break;
        case eChatFlag::MAKE_ROOM:
            MakeRoom(roomMsg);
            break;
        case eChatFlag::INSERT_ROOM:
            InsertRoom(roomMsg);
            break;
        case eChatFlag::ROOM_CHAT:
            RoomChat(roomMsg);
            break;
        case eChatFlag::OUT_ROOM:
            RoomChat(roomMsg);
            break;
        case eChatFlag::ERASE_ROOM:
            EraseRoom(roomMsg);
            break;
        case eChatFlag::CHK_ROOM:
            CheckRoom(recvMsg);
            break;
        default:
            break;
        }
    }

    return 0;
}

void CServConn::GlobalChat(sMessage & _msg)
{
    CString msg{ _msg.msg };
    CChatMgr::GetInstance()->PrintMsg(msg);
}

void CServConn::MakeRoom(sRoomMessage & _msg)
{
    if (_msg.roomNum == FAIL)
    {
        sMessage msg{};
        strcpy_s(msg.msg, _msg.msg);
        GlobalChat(msg);
        return;
    }

    CChatMgr::GetInstance()->MakeRoom(_msg);
}

void CServConn::InsertRoom(sRoomMessage & _msg)
{
    if (_msg.roomNum == FAIL)
    {
        sMessage msg{};
        strcpy_s(msg.msg, _msg.msg);
        GlobalChat(msg);
        return;
    }

    CChatMgr::GetInstance()->InsertRoom(_msg);
}

void CServConn::RoomChat(sRoomMessage & _msg)
{
    CChatMgr::GetInstance()->RoomChat(_msg);
}

void CServConn::EraseRoom(sRoomMessage & _msg)
{
    CChatMgr::GetInstance()->EraseRoom(_msg);
}

void CServConn::CheckRoom(char _recvMsg[BUF_SIZE])
{
    sChkRoomList roomList{};
    memcpy(&roomList, _recvMsg, sizeof(roomList));

    sRoomMessage roomMsg{};
    for (int i = 0; i < MAX_ROOM_CNT; ++i)
    {
        if (roomList.roomNum[i] == FAIL)
            break;;

        roomMsg.roomNum = roomList.roomNum[i];
        if(eRoomChkFlag::MAKE == roomList.flag)
            CChatMgr::GetInstance()->MakeRoom(roomMsg);
        else if (eRoomChkFlag::REMOVE == roomList.flag)
            CChatMgr::GetInstance()->EraseRoom(roomMsg);
    }
}

void CServConn::SendMsg(CString _msg, eChatFlag _flag)
{
    sMessage sendMsg{};
    sendMsg.idx = static_cast<int>(_flag);
    int len = WideCharToMultiByte(CP_ACP, 0, _msg, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, _msg, -1, sendMsg.msg, len, 0, 0);
    send(sock, (char*)&sendMsg, BUF_SIZE, 0);
}

void CServConn::SendRoomMsg(CString _msg, eChatFlag _flag, int _roomNum)
{
    sRoomMessage sendMsg{};
    sendMsg.idx = static_cast<int>(_flag);
    sendMsg.roomNum = _roomNum;
    int len = WideCharToMultiByte(CP_ACP, 0, _msg, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, _msg, -1, sendMsg.msg, len, 0, 0);
    send(sock, (char*)&sendMsg, BUF_SIZE, 0);
}
