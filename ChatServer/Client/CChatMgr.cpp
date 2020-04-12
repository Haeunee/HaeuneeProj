#include "stdafx.h"
#include "CChatMgr.h"
#include "ChatServerDlg.h"

CChatMgr* CChatMgr::listPrinter = 0;
CChatMgr::CChatMgr()
{
}

CChatMgr::~CChatMgr()
{
}

void CChatMgr::PrintMsg(const LPCTSTR _msg)
{
    msgList->AddString(_msg);
}

void CChatMgr::PrintChat(const LPCTSTR _msg)
{
    chatList->AddString(_msg);
}

void CChatMgr::SetList(CListBox * _msgList, CListBox * _chatList)
{
    msgList = _msgList;
    chatList = _chatList;
}

void CChatMgr::InsertRoom(const sRoomMessage& _roomMsg)
{
    mainDlg->InsertRoom(_roomMsg);
}

void CChatMgr::MakeRoom(const sRoomMessage & _roomMsg)
{
    mainDlg->MakeRoom(_roomMsg);
}

void CChatMgr::RoomChat(const sRoomMessage & _roomMsg)
{
    mainDlg->RoomChat(_roomMsg);
}

void CChatMgr::EraseRoom(const sRoomMessage & _roomMsg)
{
    mainDlg->EraseRoom(_roomMsg);
}
