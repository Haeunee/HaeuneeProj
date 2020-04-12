
// ChatServerDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "afxdialogex.h"
#include "CServConn.h"
#include "CChatMgr.h"
#include "CRoomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChatServerDlg::CChatServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MSG_LIST, m_MsgList);
    DDX_Control(pDX, IDC_CHAT_BOX, m_ChatEdit);
    DDX_Control(pDX, IDC_NICK_BOX, m_NickEdit);
    DDX_Control(pDX, IDC_ROOM_LIST, m_RoomList);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_SERVER_CONN, &CChatServerDlg::OnBnClickedServerConn)
    ON_BN_CLICKED(IDC_SEND_BTN, &CChatServerDlg::OnBnClickedSendBtn)
    ON_BN_CLICKED(IDC_NICK_BTN, &CChatServerDlg::OnBnClickedNickBtn)
    ON_BN_CLICKED(IDC_INSERT_ROOM_BTN, &CChatServerDlg::OnBnClickedInsertRoomBtn)
    ON_BN_CLICKED(IDC_MAKE_ROOM_BTN, &CChatServerDlg::OnBnClickedMakeRoomBtn)
    ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


BOOL CChatServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
    GetDlgItem(IDC_SERVER_CONN)->EnableWindow(TRUE);
    GetDlgItem(IDC_SEND_BTN)->EnableWindow(FALSE);
    GetDlgItem(IDC_NICK_BTN)->EnableWindow(FALSE);
    GetDlgItem(IDC_INSERT_ROOM_BTN)->EnableWindow(FALSE);
    GetDlgItem(IDC_MAKE_ROOM_BTN)->EnableWindow(FALSE);
	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChatServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatServerDlg::OnBnClickedServerConn()
{
    GetDlgItem(IDC_SERVER_CONN)->EnableWindow(FALSE);
    GetDlgItem(IDC_SEND_BTN)->EnableWindow(TRUE);
    GetDlgItem(IDC_NICK_BTN)->EnableWindow(TRUE);
    GetDlgItem(IDC_INSERT_ROOM_BTN)->EnableWindow(TRUE);
    GetDlgItem(IDC_MAKE_ROOM_BTN)->EnableWindow(TRUE);

    CChatMgr::GetInstance()->SetMainDlg(this);
    CChatMgr::GetInstance()->SetList(&m_MsgList, &m_MsgList);
    eventServConn = new CServConn{};
    eventServConn->ConnectServ();

    for (int i = 0; i < USER_INSERT_MAX; ++i)
    {
        CRoomDlg* roomDlg = new CRoomDlg(eventServConn);
        roomDlg->Create(IDD_ROOM_DLG);
        roomDlg->ShowWindow(SW_HIDE);
        roomList[i] = roomDlg;
    }
}

void CChatServerDlg::MakeRoom(const sRoomMessage & _roomMsg)
{
    AutoMutex mutex{};
    int max = m_RoomList.GetCount();
    CString msg{};
    for (int i = 0; i < max; ++i)
    {
        m_RoomList.GetText(i, msg);
        if (_wtoi(msg) == _roomMsg.roomNum)
            return;
    };

    msg.Format(L"%d", _roomMsg.roomNum);
    m_RoomList.AddString(msg);
}

void CChatServerDlg::InsertRoom(const sRoomMessage& _roomMsg)
{
    AutoMutex mutex{};
    CString msg{ _roomMsg.msg };

    MakeRoom(_roomMsg);

    for (const auto& room : roomList)
    {
        if (room->GetRoomNum() == _roomMsg.roomNum)
        {
            room->ShowWindow(SW_SHOW);
            room->SetRoomNum(_roomMsg.roomNum);
            room->m_RoomChatList.AddString(msg);
            return;
        }
    }
    for (const auto& room : roomList)
    {
        if (room->GetRoomNum() == FAIL)
        {
            room->ShowWindow(SW_SHOW);
            room->SetRoomNum(_roomMsg.roomNum);
            room->m_RoomChatList.AddString(msg);
            return;
        }
    }
}

void CChatServerDlg::RoomChat(const sRoomMessage & _roomMsg)
{
    AutoMutex mutex{};
    CString msg{ _roomMsg.msg };
    for (const auto& room : roomList)
    {
        if (room->GetRoomNum() == _roomMsg.roomNum)
        {
            room->m_RoomChatList.AddString(msg);
            break;
        }
    }
}

void CChatServerDlg::EraseRoom(const sRoomMessage & _roomMsg)
{
    AutoMutex mutex{};

    int max = m_RoomList.GetCount();
    CString msg{};
    int idx = 0;
    for (int i = 0; i < max; ++i)
    {
        m_RoomList.GetText(i, msg);
        if (_wtoi(msg) == _roomMsg.roomNum)
        {
            m_RoomList.DeleteString(i);
            break;
        }
    }
}

void CChatServerDlg::OnBnClickedSendBtn()
{
    CString msg{};
    m_ChatEdit.GetWindowTextW(msg);
    if (msg == L"") return;
    m_ChatEdit.SetWindowTextW(L"");
    eventServConn->SendMsg(msg, eChatFlag::GLOBAL_CHAT);
}

void CChatServerDlg::OnBnClickedNickBtn()
{
    CString msg{};
    m_NickEdit.GetWindowTextW(msg);
    if (msg == L"") return;
    eventServConn->SendMsg(msg, eChatFlag::SEL_NICK);
}

void CChatServerDlg::OnBnClickedMakeRoomBtn()
{
    AutoMutex mutex{};
    for (const auto& room : roomList)
    {
        if (room->GetRoomNum() == FAIL)
        {
            CString msg{};
            eventServConn->SendMsg(msg, eChatFlag::MAKE_ROOM);
            return;
        }
    }
    CChatMgr::GetInstance()->PrintMsg(L"유저가 들어갈 수 있는 방의 최대 수가 넘었습니다.");
}

void CChatServerDlg::OnBnClickedInsertRoomBtn()
{
    AutoMutex mutex{};
    for (const auto& room : roomList)
    {
        if (room->GetRoomNum() == FAIL)
        {
            CString msg{};
            int listIdx = m_RoomList.GetCurSel();
            m_RoomList.GetText(listIdx, msg);
            eventServConn->SendRoomMsg(msg, eChatFlag::INSERT_ROOM, _wtoi(msg));
            return;
        }
    }
    CChatMgr::GetInstance()->PrintMsg(L"유저가 들어갈 수 있는 방의 최대 수가 넘었습니다.");
}


void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    CDialogEx::OnSysCommand(nID, lParam);
    if (nID == SC_CLOSE)
    {
        delete(eventServConn);
    }
}
