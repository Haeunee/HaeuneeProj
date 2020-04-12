#pragma once
#include "ChatDefine.h"

class CChatServerDlg;

class CChatMgr
{
private:
    static CChatMgr* listPrinter;
    CChatMgr();
    ~CChatMgr();

    CListBox* msgList{};
    CListBox* chatList{};

    CChatServerDlg* mainDlg;

public:
    static CChatMgr* GetInstance()
    {
        if (listPrinter == nullptr)
            listPrinter = new CChatMgr;

        return listPrinter;
    }
    void PrintMsg(const LPCTSTR _msg);
    void PrintChat(const LPCTSTR _msg);
    void SetList(CListBox* _msgList, CListBox* _chatList);

    void SetMainDlg(CChatServerDlg* _dlg) { mainDlg = _dlg; };
    void MakeRoom(const sRoomMessage& _roomMsg);
    void InsertRoom(const sRoomMessage& _roomMsg);
    void RoomChat(const sRoomMessage& _roomMsg);
    void EraseRoom(const sRoomMessage& _roomMsg);
};

