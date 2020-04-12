#include "ChatDefine.h"
#include "ChatServerDlg.h"

class CServConn
{
    WSAData wsaData{};
    SOCKADDR_IN addr{};
    SOCKET sock{};

public:
    CServConn();
    ~CServConn();

    void ConnectServ();
    void ErrorHandling(const LPCTSTR _err);
    static unsigned WINAPI RecvChat(void*);
    static void GlobalChat(sMessage& _msg);
    static void MakeRoom(sRoomMessage& _msg);
    static void InsertRoom(sRoomMessage& _msg);
    static void RoomChat(sRoomMessage& _msg);
    static void EraseRoom(sRoomMessage& _msg);
    static void CheckRoom(char _recvMsg[BUF_SIZE]);
    void SendMsg(CString, eChatFlag);
    void SendRoomMsg(CString _msg, eChatFlag _flag, int _roomNum);
};