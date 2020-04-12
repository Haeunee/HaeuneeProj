
// ChatServerDlg.h: 헤더 파일
//

#pragma once
#include "ChatDefine.h"
#include <iostream>
#include <vector>
class CServConn;
class CRoomDlg;

// CChatServerDlg 대화 상자
class CChatServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CChatServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CServConn* eventServConn{};
    CRoomDlg* roomList[USER_INSERT_MAX];

    CListBox m_MsgList;
    CEdit m_ChatEdit;
    CEdit m_NickEdit;
    afx_msg void OnBnClickedServerConn();
    afx_msg void OnBnClickedSendBtn();
    afx_msg void OnBnClickedNickBtn();
    afx_msg void OnBnClickedInsertRoomBtn();
    afx_msg void OnBnClickedMakeRoomBtn();

    void MakeRoom(const sRoomMessage& _roomMsg);
    void InsertRoom(const sRoomMessage& _roomMsg);
    void RoomChat(const sRoomMessage& _roomMsg);
    void EraseRoom(const sRoomMessage& _roomMsg);
    CListBox m_RoomList;
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};

