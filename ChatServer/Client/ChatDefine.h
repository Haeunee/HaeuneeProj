#pragma once

#define BUF_SIZE 1024
#define MSG_SIZE 512
#define MAX_ROOM_CNT 10
#define FAIL 0
#define USER_INSERT_MAX 5
#define THREAD_CNT 2

enum class eChatFlag
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
    MAKE = 0,
    REMOVE = 1,
};

struct sMessage
{
    int idx{};
    char msg[MSG_SIZE]{};
};

struct sRoomMessage
{
    int idx{};
    int roomNum{};
    char msg[MSG_SIZE]{};
};

struct sChkRoomList
{
    int idx = static_cast<int>(eChatFlag::CHK_ROOM);
    int flag{};
    int roomNum[MAX_ROOM_CNT]{};
};

class AutoMutex
{
    HANDLE mutex{};
public:
    AutoMutex()
    {
        mutex = CreateMutex(NULL, FALSE, NULL);
        WaitForSingleObject(mutex, INFINITE);
    };
    ~AutoMutex()
    {
        ReleaseMutex(mutex);
    }
};