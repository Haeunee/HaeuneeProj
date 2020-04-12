#pragma once
#pragma comment(lib,"ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <winsock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <unordered_map>
#include "ChatServDefine.h"
#include <mstcpip.h>

class Server;

class AutoMutex
{
    HANDLE mutex{};

public:
    AutoMutex()
    {
        mutex = CreateMutex(NULL, FALSE, NULL);
        WaitForSingleObject(mutex, INFINITE);
    }
    ~AutoMutex()
    {
        ReleaseMutex(mutex);
    }
};

class CServerBase
{
private:
    Server* m_ParentServer{};

    typedef unordered_map<int, sChatRoom> ChatRoomMap;
    ChatRoomMap m_RoomList;
    ChatRoomMap::iterator m_RoomItor;
    vector<sUser> m_UserList; 
    char buffer[BUF_SIZE];

public:
    CServerBase() 
    {
        tcpKeepAl.onoff = 1;
        tcpKeepAl.keepalivetime = 10000;
        tcpKeepAl.keepaliveinterval = 1000;
    };
    ~CServerBase() {};
    tcp_keepalive tcpKeepAl;
    
    void SetParents(Server* _serv) { m_ParentServer = _serv; };
    void CheckMsg(const char[BUF_SIZE], SOCKET _sock);
    void InsertUser(SOCKET _sock);
    void OutUser(SOCKET _sock);
    void GlobalChat(sMessage& _msg, SOCKET _sock);
    void SelectNick(sMessage& _msg, SOCKET _sock);
    void MakeRoom(sRoomMessage& _msg, SOCKET _sock);
    void InsertRoom(sRoomMessage& _msg, SOCKET _sock);
    void ChatRoom(sRoomMessage& _msg, SOCKET _sock);
    void OutRoom(sRoomMessage& _msg, SOCKET _sock);
    void RoomOutChk(SOCKET _sock, sChatRoom& _nowRoom);
    string GetNick(SOCKET _sock);

    void SendDBLog(SOCKET _sock, eChatLogType _logType, char[MSG_SIZE]);
    void SendMsg(char _buf[BUF_SIZE], SOCKET _sock);
};

class Server
{
protected:
    SOCKET MakeServ();
    int threadCnt{};

public:
    virtual void Init() = 0;
    virtual void SendMsg(char _buf[BUF_SIZE], SOCKET _sock) = 0;
    void SetThreadCnt(int _threadCnt);
    static void PrintError(const string& _err);
};
