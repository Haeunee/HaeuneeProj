#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define BUF_SIZE 1024
#define MSG_SIZE 512
#define MAX_ROOM_CNT 10
#define MAX_ROOM_USER_CNT 50
#define FAIL 0
#define EVT_THREAD_CNT 2

enum eChatFlag
{
    SEL_NICK        = 1,
    GLOBAL_CHAT     = 2,
    MAKE_ROOM       = 3,
    INSERT_ROOM     = 4,
    ROOM_CHAT       = 5,
    OUT_ROOM        = 6,
    ERASE_ROOM      = 7,
    CHK_ROOM        = 8,
};

enum eRoomChkFlag
{
    MAKE        = 0,
    REMOVE      = 1,
};

enum eChatLogType
{
    ACCEPT_SERV_LOG     = 0,
    OUT_SERV_LOG        = 1,
    GLOBAL_CHAT_LOG     = 2,
    CHANGE_NICK_LOG     = 3,
    MAKE_ROOM_LOG       = 4,
    INSERT_ROOM_LOG     = 5,
    ROOM_CHAT_LOG       = 6,
    OUT_ROOM_LOG        = 7,
};

struct sMessage
{
    int idx{};
    char msg[MSG_SIZE]{};
};

struct sChatRoom
{
    int nowIdx{};
    unsigned long users[MAX_ROOM_USER_CNT]{};
};

struct sUser
{
    string userNick{};
    unsigned long sock{};
};

struct sRoomMessage
{
    int idx{};
    int roomNum = 1;
    char msg[MSG_SIZE]{};
};

struct sChkRoomList
{
    int idx = eChatFlag::CHK_ROOM;
    int flag{};
    int roomNum[MAX_ROOM_CNT]{};
}; 
