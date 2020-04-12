#pragma once
#include "ChatDefine.h"

// CRoomDlg 대화 상자
class CServConn;

class CRoomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRoomDlg)

    int roomNum{FAIL};
    CServConn* eventServ{};

public:
	CRoomDlg(CServConn* _eventServ, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRoomDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROOM_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnBnClickedRoomSendBtn();
    void SetRoomNum(int _roomNum) { roomNum = _roomNum; };
    int GetRoomNum() const { return roomNum; };
    CListBox m_RoomChatList;
    CEdit m_RoomChatEdit;
};
