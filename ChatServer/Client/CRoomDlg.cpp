// CRoomDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ChatServer.h"
#include "CRoomDlg.h"
#include "afxdialogex.h"
#include "CServConn.h"


// CRoomDlg 대화 상자

IMPLEMENT_DYNAMIC(CRoomDlg, CDialogEx)

CRoomDlg::CRoomDlg(CServConn* _eventServ, CWnd * pParent)
    : CDialogEx(IDD_ROOM_DLG, pParent), eventServ(_eventServ)
{
}

CRoomDlg::~CRoomDlg()
{
}

void CRoomDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ROOM_CHAT_LIST, m_RoomChatList);
    DDX_Control(pDX, IDC_ROOM_CHAT_BOX, m_RoomChatEdit);
}


BEGIN_MESSAGE_MAP(CRoomDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_BN_CLICKED(IDC_ROOM_SEND_BTN, &CRoomDlg::OnBnClickedRoomSendBtn)
END_MESSAGE_MAP()


// CRoomDlg 메시지 처리기


void CRoomDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialogEx::OnSysCommand(nID, lParam);

    if (nID == SC_CLOSE)
    {
        eventServ->SendRoomMsg(nullptr, eChatFlag::OUT_ROOM, roomNum);
        roomNum = FAIL;
        m_RoomChatList.ResetContent();
    }
}


void CRoomDlg::OnBnClickedRoomSendBtn()
{
    CString msg{};
    m_RoomChatEdit.GetWindowTextW(msg);
    if (msg == L"") return;
    m_RoomChatEdit.SetWindowTextW(L"");
    eventServ->SendRoomMsg(msg, eChatFlag::ROOM_CHAT, roomNum);
}
