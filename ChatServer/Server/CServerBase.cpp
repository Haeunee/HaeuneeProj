#include "pch.h"
#include "CServerBase.h"
#include "CAdoDBConn.h"

CAdoDBConn* dbConn = new CAdoDBConn;

void CServerBase::CheckMsg(const char _msg[BUF_SIZE], SOCKET _sock)
{
    sMessage message{};
    memcpy(&message, _msg, sizeof(sMessage));
    sRoomMessage roomMsg{};
    memcpy(&roomMsg, _msg, sizeof(sRoomMessage));

    switch (static_cast<eChatFlag>(message.idx))
    {
    case eChatFlag::SEL_NICK:
        SelectNick(message, _sock);
        break;
    case eChatFlag::GLOBAL_CHAT:
        GlobalChat(message, _sock);
        break;
    case eChatFlag::MAKE_ROOM:
        MakeRoom(roomMsg, _sock);
        break;
    case eChatFlag::ROOM_CHAT:
        ChatRoom(roomMsg, _sock);
        break;
    case eChatFlag::INSERT_ROOM:
        InsertRoom(roomMsg, _sock);
        break;
    case eChatFlag::OUT_ROOM:
        OutRoom(roomMsg, _sock);
        break;
    default:
        break;
    }
}

void CServerBase::InsertUser(SOCKET _sock)
{
    sUser userInfo{};
    userInfo.sock = _sock;
    m_UserList.push_back(userInfo);

    sChkRoomList msg{};
    msg.flag = eRoomChkFlag::MAKE;
    int roomCnt{};
    int roomSize = m_RoomList.size();
    for (m_RoomItor = m_RoomList.begin(); m_RoomItor != m_RoomList.end(); ++m_RoomItor)
    {
        msg.roomNum[roomCnt] = m_RoomItor->first;
        ++roomCnt;
    }
    memset(buffer, 0, BUF_SIZE);
    memcpy(buffer, &msg, sizeof(msg));
    SendMsg(buffer, _sock);

    SendDBLog(_sock, eChatLogType::ACCEPT_SERV_LOG, buffer);
    cout << "Clnt conn (userSock : " << _sock << ")" << endl;
}

void CServerBase::OutUser(SOCKET _sock)
{
    sChkRoomList msg{};
    msg.idx = eChatFlag::CHK_ROOM;
    msg.flag = eRoomChkFlag::REMOVE;

    sRoomMessage roomMsg{};
    roomMsg.idx = eChatFlag::OUT_ROOM;
    sprintf_s(roomMsg.msg, "Out User : %s", GetNick(_sock).c_str());

    int roomCnt{};
    m_RoomItor = m_RoomList.begin();
    while (m_RoomItor != m_RoomList.end())
    {
        sChatRoom& nowRoom = m_RoomItor->second;
        roomMsg.roomNum = m_RoomItor->first;
        memset(buffer, 0, BUF_SIZE);
        memcpy(buffer, &roomMsg, sizeof(roomMsg));
        RoomOutChk(_sock, nowRoom);
        if (nowRoom.users[0] == 0)
        {
            msg.roomNum[roomCnt] = m_RoomItor->first;
            ++roomCnt;
            m_RoomItor = m_RoomList.erase(m_RoomItor);
            if (m_RoomItor == m_RoomList.end())
                break;
        }
        else
            ++m_RoomItor;
    }

    memset(buffer, 0, BUF_SIZE);
    memcpy(buffer, &msg, sizeof(msg));

    for (unsigned int userIdx = 0; userIdx < m_UserList.size(); ++userIdx)
    {
        if (m_UserList[userIdx].sock == _sock)
        {
            m_UserList[userIdx] = m_UserList[m_UserList.size() - 1];
            m_UserList.erase(--m_UserList.end());
            --userIdx;
        }
        else
            SendMsg(buffer, m_UserList[userIdx].sock);
    }

    SendDBLog(_sock, eChatLogType::OUT_SERV_LOG, buffer);
    cout << "CloseClnt (userSock : " << _sock << ")" << endl;
}

void CServerBase::GlobalChat(sMessage& _msg, SOCKET _sock)
{
    string tmpStr = _msg.msg;
    sprintf_s(_msg.msg, "%s : %s", GetNick(_sock).c_str(), tmpStr.c_str());

    memset(buffer, 0, BUF_SIZE);
    memcpy(buffer, &_msg, sizeof(_msg));

    int max = m_UserList.size();
    for (int i = 0; i < max; ++i)
        SendMsg(buffer, m_UserList[i].sock);

    SendDBLog(_sock, eChatLogType::GLOBAL_CHAT_LOG, _msg.msg);
    cout << "Send GlobalChat Msg (userSock : " << _sock << ")" << endl;
}

void CServerBase::SelectNick(sMessage & _msg, SOCKET _sock)
{
    for (auto& userData : m_UserList)
    {
        if (userData.sock == _sock)
        {
            userData.userNick = _msg.msg;
            break;
        }
    }
    string tmpStr = _msg.msg;
    sprintf_s(_msg.msg, "nick : %s", tmpStr.c_str());
    SendDBLog(_sock, eChatLogType::CHANGE_NICK_LOG, _msg.msg);

    memset(buffer, 0, BUF_SIZE);
    memcpy(buffer, &_msg, sizeof(_msg));
    SendMsg(buffer, _sock);
    cout << "Send SelectNick Msg (userSock : " << _sock << ")" << endl;
}

void CServerBase::MakeRoom(sRoomMessage & _msg, SOCKET _sock)
{
    int roomNum{};
    for (roomNum = 1; roomNum <= MAX_ROOM_CNT; ++roomNum)
    {
        if (0 == m_RoomList.count(roomNum))
            break;
    }
    if (m_RoomList.size() <= MAX_ROOM_CNT)
    {
        sprintf_s(_msg.msg, "Make Room : %s", GetNick(_sock).c_str());
        _msg.roomNum = roomNum;

        sChatRoom makeRoom{};
        makeRoom.users[0] = _sock;
        makeRoom.nowIdx = 1;
        m_RoomList.insert(make_pair(_msg.roomNum, makeRoom));

        int max = m_UserList.size();
        for (int i = 0; i < max; ++i)
        {
            if (m_UserList[i].sock == _sock)
            {
                _msg.idx = eChatFlag::INSERT_ROOM;
                memset(buffer, 0, BUF_SIZE);
                memcpy(buffer, &_msg, sizeof(_msg));
                SendMsg(buffer, _sock);
            }
            else
            {
                _msg.idx = eChatFlag::MAKE_ROOM;
                memset(buffer, 0, BUF_SIZE);
                memcpy(buffer, &_msg, sizeof(_msg));
                SendMsg(buffer, m_UserList[i].sock);
            }
        }

        cout << "Send MakeRoom Msg (userSock : " << _sock << ")" << endl;
    }
    else
    {
        sprintf_s(_msg.msg, "Already Max Room : %d", MAX_ROOM_CNT);
        _msg.roomNum = FAIL;

        memset(buffer, 0, BUF_SIZE);
        memcpy(buffer, &_msg, sizeof(_msg));
        SendMsg(buffer, _sock);
        cout << "Send Can't MakeRoom Msg (userSock : " << _sock << ")" << endl;
    }

    sprintf_s(buffer, "%s, RoomNum : %d", _msg.msg, _msg.roomNum);
    SendDBLog(_sock, eChatLogType::MAKE_ROOM_LOG, buffer);
}

void CServerBase::InsertRoom(sRoomMessage & _msg, SOCKET _sock)
{
    sChatRoom& nowRoom{ m_RoomList.find(_msg.roomNum)->second };
    for (const auto& userSock : nowRoom.users)
    {
        if (userSock == _sock)
        {
            sprintf_s(_msg.msg, "Already Insert Room : %d", _msg.roomNum);
            _msg.roomNum = FAIL;
            memset(buffer, 0, BUF_SIZE);
            memcpy(buffer, &_msg, sizeof(_msg));
            SendMsg(buffer, _sock);
            sprintf_s(buffer, "%s, RoomNum : %d", _msg.msg, _msg.roomNum);
            SendDBLog(_sock, eChatLogType::INSERT_ROOM_LOG, buffer);
            return;
        }
    }
    sprintf_s(_msg.msg, "Insert User : %s", GetNick(_sock).c_str());
    memset(buffer, 0, BUF_SIZE);
    memcpy(buffer, &_msg, sizeof(_msg));

    nowRoom.users[nowRoom.nowIdx] = _sock;
    ++nowRoom.nowIdx;

    for (const auto& userSock : nowRoom.users)
        SendMsg(buffer, userSock);

    cout << "Send InsertRoom Msg (roomNum : " << _msg.roomNum << ")" << endl;

    sprintf_s(buffer, "%s, RoomNum : %d", _msg.msg, _msg.roomNum);
    SendDBLog(_sock, eChatLogType::INSERT_ROOM_LOG, buffer);
}

void CServerBase::ChatRoom(sRoomMessage & _msg, SOCKET _sock)
{
    string tmpStr = _msg.msg;
    sprintf_s(_msg.msg, "%s : %s", GetNick(_sock).c_str() , tmpStr.c_str());

    memset(buffer, 0, BUF_SIZE);
    memcpy(buffer, &_msg, sizeof(_msg));

    const sChatRoom& nowRoom{ m_RoomList.find(_msg.roomNum)->second };
    for (const auto& userSock : nowRoom.users)
        SendMsg(buffer, userSock);

    cout << "Send ChatRoom Msg (roomNum : " << _msg.roomNum << ")" << endl;

    sprintf_s(buffer, "%s, RoomNum : %d", _msg.msg, _msg.roomNum);
    SendDBLog(_sock, eChatLogType::ROOM_CHAT_LOG, buffer);
}

void CServerBase::OutRoom(sRoomMessage & _msg, SOCKET _sock)
{
    sprintf_s(_msg.msg, "Out User : %s", GetNick(_sock).c_str());

    sChatRoom& nowRoom{ m_RoomList.find(_msg.roomNum)->second };
    memset(buffer, 0, BUF_SIZE);
    memcpy_s(buffer, sizeof(_msg), &_msg, sizeof(_msg));
    RoomOutChk(_sock, nowRoom);

    if (0 == nowRoom.users[0])
    {
        _msg.idx = eChatFlag::ERASE_ROOM;
        m_RoomList.erase(m_RoomList.find(_msg.roomNum));

        memset(buffer, 0, BUF_SIZE);
        memcpy_s(buffer, sizeof(_msg), &_msg, sizeof(_msg));
        int max = m_UserList.size();
        for (int i = 0; i < max; ++i)
            SendMsg(buffer, m_UserList[i].sock);
    }

    sprintf_s(buffer, "RoomNum : %d", _msg.roomNum);
    SendDBLog(_sock, eChatLogType::OUT_ROOM_LOG, buffer);
    cout << "Send OutRoom Msg (roomNum : " << _msg.roomNum << ")" << endl;
}

void CServerBase::RoomOutChk(SOCKET _sock, sChatRoom& _nowRoom)
{
    int userIdx = 0;
    while (_nowRoom.users[userIdx] != 0)
    {
        if (_nowRoom.users[userIdx] == _sock)
        {
            --_nowRoom.nowIdx;
            _nowRoom.users[userIdx] = _nowRoom.users[_nowRoom.nowIdx];
            _nowRoom.users[_nowRoom.nowIdx] = 0;
        }
        SendMsg(buffer, _nowRoom.users[userIdx]);
        ++userIdx;
    }
}

string CServerBase::GetNick(SOCKET _sock)
{
    string nick{};
    for (const auto& userData : m_UserList)
    {
        if (userData.sock == _sock)
        {
            nick = userData.userNick;
            break;
        }
    }
    if ("" == nick)
        nick = to_string(_sock);
    return nick;
}

void CServerBase::SendDBLog(SOCKET _sock, eChatLogType _logType, char _msg[MSG_SIZE])
{
    AutoMutex mutex{};
    switch (_logType)
    {
    case ACCEPT_SERV_LOG:
        sprintf_s(_msg, MSG_SIZE, "User Cnt : %d", m_UserList.size());
        break;
    case OUT_SERV_LOG:
        sprintf_s(_msg, MSG_SIZE, "User Cnt : %d", m_UserList.size());
        break;
    default:
        break;
    }

    dbConn->AddDBChatLog(_logType, _sock, _msg);
}

void CServerBase::SendMsg(char _buf[BUF_SIZE], SOCKET _sock)
{
    m_ParentServer->SendMsg(_buf, _sock);
}

SOCKET Server::MakeServ()
{
    WSADATA wsaData{};
    if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
        PrintError("WSAStartup()");

    SOCKET sock{ socket(PF_INET, SOCK_STREAM, 0) };

    SOCKADDR_IN servAddr{};
    servAddr.sin_family = AF_INET;
    if (1 != inet_pton(AF_INET, "10.10.10.52", &servAddr.sin_addr.s_addr))
        PrintError("inet_pton()");
    servAddr.sin_port = htons(10000);

    if (SOCKET_ERROR == bind(sock, (SOCKADDR*)&servAddr, sizeof(servAddr)))
        PrintError("bind()");
    if (SOCKET_ERROR == listen(sock, 5))
        PrintError("listen()");

	LPINT lpiProtocols{ new INT() };
    LPWSAPROTOCOL_INFO lpProtocolBuffer{ new WSAPROTOCOL_INFO() };
    LPDWORD lpProtocolBufferLen{ new DWORD() };
    WSAEnumProtocols(lpiProtocols, NULL, lpProtocolBufferLen);
    lpProtocolBuffer->dwMessageSize = *lpProtocolBufferLen;

    WSAEnumProtocols(lpiProtocols, lpProtocolBuffer, lpProtocolBufferLen);

    std::cout << "MakeServer Succ!" << endl;
    return sock;
}

void Server::SetThreadCnt(int _threadCnt)
{
    if (_threadCnt != 0)
        threadCnt = _threadCnt;
    else
    {
        SYSTEM_INFO sysInfo{};
        GetSystemInfo(&sysInfo);
        threadCnt = sysInfo.dwNumberOfProcessors;
    }
}

void Server::PrintError(const string & _err)
{
    cout << _err << " error! / Error Num : "<< WSAGetLastError() << endl;
    exit(10);
}
