
// ChatClientDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "afxdialogex.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatClientDlg 대화 상자



CChatClientDlg::CChatClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATCLIENT_DIALOG, pParent)
	, m_strIP(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//  DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
	DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
	DDX_Control(pDX, IDC_STATIC_PAINT, m_ctlPaint);
	DDX_Control(pDX, IDC_EDIT_SEND, m_strSend);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_btnConnect);
}

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//ON_BN_CLICKED(IDC_BUTTON2, OnButton2)

	//ON_MESSAGE(UM_ACCEPT, OnAccept)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CChatClientDlg::OnBnClickedButtonConnect)
END_MESSAGE_MAP()


// CChatClientDlg 메시지 처리기

BOOL CChatClientDlg::OnInitDialog()
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_strIP = "localhost";

	isRankSend = FALSE;
	UpdateData(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CChatClientDlg::OnPaint()
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
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CChatClientDlg::OnBnClickedButtonConnect()
{
	// TODO: 여기에 구현 코드 추가.
	//접속
	UpdateData(TRUE);
	m_socCom.Create();
	//IP주소와 포트번호를 지정
	m_socCom.Connect(m_strIP, 5000);
	m_socCom.Init(this->m_hWnd);
	m_btnConnect.EnableWindow(FALSE);
}

afx_msg LRESULT CChatClientDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	//접속된 곳에서 데이터가 도착했을 때
	char pTmp[256];
	CString strTmp;
	struct Data data;
	memset(pTmp, '\0', 256);
	memset(&data, '\0', 256);
	//데이터를 pTmp에 받는다.
	m_socCom.Receive(pTmp, sizeof(pTmp));

	memcpy(&data, pTmp, sizeof(pTmp));

	if (data.Opcode == 0x03)
	{
		memcpy(pTmp, data.Operand, sizeof(data.Operand));
		StrToData(pTmp);
	}
	else
	{
		if (strcmp(pTmp, "정답입니다.") == 0 || strcmp(pTmp, "정답자가 나왔습니다.") == 0 || strcmp(pTmp, "그림을 초기화합니다.") == 0)
			DeleteContents();



		//게임이 끝났습니다
		if (strcmp(pTmp, "게임이 끝났습니다.") == 0)
		{
			m_strSend.EnableWindow(FALSE);

		}
		else if (strcmp(pTmp, "ranker") == 0)
		{

			m_strSend.EnableWindow(TRUE);
			isRankSend = TRUE;
			int i = m_list.GetCount();
			m_list.InsertString(i, "이름을 입력하세요.");
			return TRUE;
		}

		//리스트박스에 보여준다

		int i = m_list.GetCount();
		strTmp.Format("%s", pTmp);
		strTmp.Replace("\t", "     ");
		printf("%s\n", strTmp);
		m_list.InsertString(i, strTmp);
	}
	return 0;
}



void CChatClientDlg::DataAnalysis(char* data)
{
	// TODO: 여기에 구현 코드 추가.
	CPoint curPos;
	CRect rect;
	struct lineData line;

	m_rectView.GetWindowRect(rect);
	ScreenToClient(rect);

	memcpy(&line, data, sizeof(line));

	memcpy(&m_pntOld, line.prev, sizeof(m_pntOld));
	memcpy(&curPos, line.pos, sizeof(curPos));
	memcpy(&m_penWidth, line.penwidth, sizeof(m_penWidth));
	memcpy(&m_color, line.color, sizeof(m_color));

	CDC* pDC = m_rectView.GetWindowDC();
	CLine *pLine = AddLine(m_pntOld, curPos, m_color, m_penWidth);
	pLine->Draw(pDC);
}



CLine* CChatClientDlg::AddLine(CPoint ptFrom, CPoint ptTo, COLORREF color, int nPenWidth)
{
	// TODO: 여기에 구현 코드 추가.
	CLine *pLine = new CLine(ptFrom, ptTo, color, nPenWidth);
	try {
		//새 직선 객체를 객체 배열에 추가한다.
		m_oaLines.Add(pLine);
	}
	catch (CMemoryException* perr) // 메모리 예외 상황이 발생하였는가;
	{
		//에러 메시지 출력
		AfxMessageBox(_T("Out of memory"), MB_ICONSTOP | MB_OK); if (pLine) //만약 직선 객체를 만들었다면 삭제한다.
		{
			delete pLine;
			pLine = NULL;
		}
		// 예외 객체도 삭제한다.
		perr->Delete();
	}
	return pLine; // 뷰 객체로 하여금 직선 객체를 그리게 할 때 사용

	return nullptr;
}


void CChatClientDlg::StrToData(char* data)
{
	// TODO: 여기에 구현 코드 추가.
	CPoint curPos;
	CRect rect;
	CDC* pic = m_ctlPaint.GetWindowDC();
	struct lineData line;

	//m_rectView.GetWindowRect(rect);
	//ScreenToClient(rect);

	memcpy(&line, data, sizeof(line));

	memcpy(&m_pntOld, line.prev, sizeof(m_pntOld));
	memcpy(&curPos, line.pos, sizeof(curPos));
	memcpy(&m_penWidth, line.penwidth, sizeof(m_penWidth));
	memcpy(&m_color, line.color, sizeof(m_color));

	CClientDC dc(this);

	//펜과 브러시 속성을 변경
	CPen pen, *pOldPen;
	CBrush brush, *pOldBrush;

	// 선택한 선색으로 펜을 생성
	pen.CreatePen(PS_SOLID, m_penWidth, m_color);
	// 생성한 펜을 선택
	pOldPen = (CPen *)pic->SelectObject(&pen);

	// 직전 좌표에서 현재 좌표까지 선을 그려준다.
	pic->MoveTo(m_pntOld.x, m_pntOld.y);
	pic->LineTo(curPos.x, curPos.y);

	/*CDC* pDC = m_rectView.GetWindowDC();
	CLine *pLine = AddLine(m_pntOld, curPos, m_color, m_penWidth);
	pLine->Draw(pDC, m_color, m_penWidth);*/
}


void CChatClientDlg::DeleteContents()
{
	// TODO: 여기에 구현 코드 추가.
	/*int liCount = m_oaLines.GetSize();
	int liPos;

	if (liCount) {
		for (liPos = 0; liPos < liCount; liPos++) delete m_oaLines[liPos];
		m_oaLines.RemoveAll();
	}*/
	CDC* pic = m_ctlPaint.GetWindowDC();
	CRect rect;
	CBrush brush;

	GetWindowRect(rect);
	ScreenToClient(rect);

	brush.CreateSolidBrush(RGB(255, 255, 255));      //흰색으로 채우기
	pic->SelectObject(&brush);
	pic->FillRect(rect, &brush);
	brush.DeleteObject();
	
}





BOOL CChatClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN &&
		pMsg->wParam == VK_RETURN)
	{
		// GetFocus() == m_strSend
		// handle return pressed in edit control
		UpdateData(TRUE);
		char pTmp[256];
		CString strTmp;
		//pTmp에 전송할 데이터 입력
		memset(pTmp, '\0', 256);
		CString sendMSG;
		m_strSend.GetWindowText(sendMSG);
		strcpy(pTmp, sendMSG);
		m_strSend.SetWindowText(" ");

		//전송
		m_socCom.Send(pTmp, 256);

		//전송한 데이터도 리스트박스에 보여준다
		strTmp.Format("%s", pTmp);
		int i = m_list.GetCount();
		m_list.InsertString(i, strTmp);
		return TRUE; // this doesn't need processing anymore
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
