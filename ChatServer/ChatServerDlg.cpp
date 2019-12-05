
// ChatServerDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "afxdialogex.h"


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


// CChatServerDlg 대화 상자



CChatServerDlg::CChatServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATSERVER_DIALOG, pParent)
	, m_strStatus(_T(""))
	, m_test(_T(""))
	, m_restQ(0)
	, m_isErase(FALSE)
	, m_nPenWidth(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nPenWidth = 5;
	m_isErase = FALSE;

	m_colorPrev = m_color = RGB(0, 0, 0);
	gameState = 0;
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//  DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
	DDX_Text(pDX, IDC_STATIC_STATUS, m_strStatus);
	DDX_Text(pDX, IDC_STATIC_TEST, m_test);
	DDX_Control(pDX, IDC_STATIC_PAINT, m_rectView);
	//  DDX_Text(pDX, IDC_STATIC_RESTQ, m_restQ);
	//  DDX_Text(pDX, IDC_STATIC_Q, m_qWord);
	DDX_Control(pDX, IDC_BUTTON_NEXTQ, m_bNextQ);
	//  DDX_Text(pDX, IDC_STATIC_Q, m_qWord);
	DDX_Control(pDX, IDC_STATIC_Q, m_qWord);
	DDX_Control(pDX, IDC_STATIC_RESTQ, m_strRestQ);
	DDX_Control(pDX, IDC_STATIC_RESTT, m_strTime);
	DDX_Check(pDX, IDC_CHECK_ERASER, m_isErase);
	DDX_Text(pDX, IDC_EDIT_PENWIDTH, m_nPenWidth);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_colorView);
	DDX_Control(pDX, IDC_EDIT_SEND, m_strSend);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_MESSAGE(UM_ACCEPT, OnAccept)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_GSTART, &CChatServerDlg::OnBnClickedButtonGstart)
	ON_BN_CLICKED(IDC_BUTTON_NEXTQ, &CChatServerDlg::OnBnClickedButtonNextq)
	ON_BN_CLICKED(IDC_CHECK_ERASER, &CChatServerDlg::OnBnClickedCheckEraser)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &CChatServerDlg::OnClickedButtonColor)
	ON_EN_CHANGE(IDC_EDIT_PENWIDTH, &CChatServerDlg::OnEnChangeEditPenwidth)
	ON_BN_CLICKED(IDC_BUTTON_ALLCLEAR, &CChatServerDlg::OnClickedButtonAllclear)
	ON_BN_CLICKED(IDC_BUTTON1, &CChatServerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CChatServerDlg 메시지 처리기

BOOL CChatServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	//Question
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

	// TODO: 여기에 추가 초기화 작업을 추가합니다.;
	OpenDatabase();
	setRankInfo();
	m_bNextQ.ShowWindow(SW_HIDE);//숨긴다

	for(int i=0;i<USERNUM;i++)
		m_socCom[i] = NULL;
	for (int i = 0;i < USERNUM;i++)
		totalScore[i] = 0;
	userCount = 0;
	
	
	// 서버 소켓을 생성(포트번호5000)
	m_socServer.Create(5000);
	UpdateData(FALSE);
	//클라이언트의 접속을 기다린다.
	BOOL tmp = m_socServer.Listen();
	//소켓 클래스와 메인 윈도우(여기에서는 CChatServerDlg)를 연결한다.
	m_socServer.Init(this->m_hWnd);


	//다음문제 버튼을 비활성화 합니다.
	m_bNextQ.EnableWindow(FALSE);
	nameSendCheck = FALSE; //우승자의 랭킹등록 허용 : 거부로 세팅
	gameState = 0; //시작전 :0   / 진행중 :1   / 종료 : 2
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	printf("OnSysCommand 호출\n");
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

void CChatServerDlg::OnPaint()
{
	printf("OnPaint 호출\n");
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
		CDC* pic = m_rectView.GetWindowDC();
		CRect rect;
		CBrush brush;

		GetWindowRect(rect);
		ScreenToClient(rect);

		brush.CreateSolidBrush(RGB(255, 255, 255));      //흰색으로 채우기
		pic->SelectObject(&brush);
		pic->FillRect(rect, &brush);
		brush.DeleteObject();

		pic = m_colorView.GetWindowDC();				//현색 색상 알려주기
		brush.CreateSolidBrush(m_color);
		pic->SelectObject(&brush);
		pic->FillRect(rect, &brush);
		brush.DeleteObject();

		//선그리기
		int liCount = m_oaLines.GetSize();
		pic = m_rectView.GetWindowDC();
		if (liCount) {
			int liPos;
			CLine *lptLine;
			for (liPos = 0; liPos < liCount; liPos++) {
				lptLine = (CLine*)m_oaLines[liPos];
				lptLine->Draw(pic);
			}
		}
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	printf("OnQueryDragIcon 호출\n");

	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CChatServerDlg::OnAccept(WPARAM wParam, LPARAM lParam)
{
	printf("OnAccept 이벤트발생\n");
	// 클라이언트에서 접속 요청이 왔을 때
	m_strStatus = "접속 성공";

	// 통신용 소켓을 생성한 뒤
	//m_socCom[userCount] = new CSocCom;
	// 서버소켓과 통신소켓을 연결한다.
	m_socCom[userCount] = m_socServer.GetAcceptSocCom();
	m_socCom[userCount]->Init(this->m_hWnd);

	m_socCom[userCount]->Send("접속성공", 256);

	userCount++;
	UpdateData(FALSE);
	return LPARAM();//Question 교재는 TRUE로 되어있음
}


afx_msg LRESULT CChatServerDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	//접속된 곳에서 데이터가 도착했을 때
	char pTmp[256],temp[256];
	CString strTmp;
	memset(pTmp, '\0', 256);
	CSocCom* pRcv = (CSocCom*)lParam;
	
	//데이터를 pTmp에 받는다.
	for (int i = 0; i < userCount; i++)
	{
		if (BOOL b = (pRcv == m_socCom[i])) 
		{
			m_socCom[i]->Receive(pTmp, 256);
			strTmp.Format("%s", pTmp);
			strTmp.TrimRight();
			printf("받은 데이터 : %s123\n", strTmp);

			//우승자가 이니셜을 보냈을 떄의 코드
			if (nameSendCheck == TRUE && gameState ==2)
			{
				BOOL isSuccess = FALSE;
				TRY
				{
					UpdateData(TRUE);
					CString strSQL;
					strSQL.Format(
							  "delete FROM userrank where totalscore = %d"
							, RankScore[9]);
						printf("%s\n", strSQL);
						g_dbRank.ExecuteSQL(strSQL);
						printf("삭제성공 \n");
				}CATCH(CException, e)
				{
					TCHAR szCause[255];
					e->GetErrorMessage(szCause, 255);
					//AfxMessageBox(szCause, MB_ICONERROR);
					CString reason = "삭제실패 이유:";
					reason.Format("%s %s", reason, szCause);
					printf("삭제실패... \n");
					m_socCom[i]->Send(reason, 256);
					return FALSE;
				}
				END_CATCH


				TRY
				{
					UpdateData(TRUE);
					CString strSQL;
					CString strUSERNAME = strTmp;
					CString strTOTALSCORE;
					strTOTALSCORE.Format("%d", totalScore[i]);
					CString strPLAYDATE = "2019-01-16";
					//	CString strPLAYDATE = dlg->m_date;

						printf("strUSERNAME : %s\n", strUSERNAME);
						printf("strTOTALSCORE : %s\n", strTOTALSCORE);
						printf("strPLAYDATE : %s\n", strPLAYDATE);

						strSQL.Format(
							  "INSERT INTO USERRANK VALUES('%s','%s','%s')"
							, strUSERNAME, strTOTALSCORE, strPLAYDATE);
						printf("%s\n", strSQL);
						g_dbRank.ExecuteSQL(strSQL);
						printf("등록성공 \n");
						//CNotice *dlg = new CNotice("등록성공",NULL);
						//dlg->DoModal();
						m_socCom[i]->Send("success", 256);
						isSuccess = TRUE;
				}CATCH(CException, e)
				{
					TCHAR szCause[255];
					e->GetErrorMessage(szCause, 255);
					//AfxMessageBox(szCause, MB_ICONERROR);
					CString reason = "등록실패 이유:";
					reason.Format("%s %s", reason, szCause);
					printf("등록실패... \n");
					m_socCom[i]->Send(reason, 256);
					m_socCom[i]->Send("다시 이름을 입력해주세요", 256);

					/*CNotice *dlg = new CNotice(reason, NULL);
					dlg->DoModal();*/
					//if (g_dbRank.IsOpen()) g_dbRank.Close();
					return FALSE;
				}
				END_CATCH
			}

			if (gameState == 2) {
				//랭킹출력
				CString title, rankinfo;
				title = "랭킹	이름	점수	날짜	";
				CRecordset recSet(&g_dbRank);
				TRY
				{
					CString strSQL;
					CString strUSERNAME;
					CString strTOTALSCORE;
					CString strPLAYDATE;
					strSQL = "SELECT USERNAME,TOTALSCORE,PLAYDATE FROM USERRANK ORDER BY TOTALSCORE DESC";
					//strSQL = _T("SELECT USERNAME,TOTALSCORE,PLAYDATE FROM USERRANK ORDER BY TOTALSCORE DESC");
					recSet.Open(CRecordset::forwardOnly, strSQL);
					for (int i = 0;i < userCount;i++)m_socCom[i]->Send(title, 256);
					int ranknumber = 1;
					while (!recSet.IsEOF())
					{
						rankinfo.Format("%d", ranknumber);
						recSet.GetFieldValue(_T("USERNAME"), strUSERNAME);
						recSet.GetFieldValue(_T("TOTALSCORE"), strTOTALSCORE);
						recSet.GetFieldValue(_T("PLAYDATE"), strPLAYDATE);
						CString tab  = "		";
						rankinfo += tab + strUSERNAME + tab + strTOTALSCORE + tab + strPLAYDATE;
						printf("%s\n", rankinfo);

						for (int i = 0;i < userCount;i++)m_socCom[i]->Send(rankinfo, 256);
						recSet.MoveNext();
						ranknumber++;

					}
					return TRUE;


				}CATCH(CException, e)
				{
					TCHAR szCause[255];
					e->GetErrorMessage(szCause, 255);
					AfxMessageBox(szCause, MB_ICONERROR);
					if (g_dbRank.IsOpen()) g_dbRank.Close();
					return FALSE;
				}
				END_CATCH
			}
			

			for (int j = 0; j < userCount; j++)
			{
				if(j != i)
				m_socCom[j]->Send(pTmp, 256);

			}

			if (gameState == 1) {
				//정답과 문자열을 비교합니다
				printf("받은 데이터 : %s123\n", strTmp);

				printf("제시어: %s123\n", Q.word[10 - m_restQ]);
				if (strcmp(strTmp, Q.word[10 - m_restQ]) == 0)
				{
					printf("%s\n", "정답을 맞춤");
					

					//버튼을 비활성화 합니다.
					m_bNextQ.EnableWindow(FALSE);



					//점수합산을 합니다
					totalScore[i] += Q.time[10 - m_restQ];

					//타이머를 정지합니다.

					//정답을 알립니다
					int k = m_list.GetCount();
					CString temp;
					temp.Format("정답자 나옴 사용자번호: %2d", i);
					m_list.InsertString(k, temp);
					for (int j = 0; j < userCount; j++)
						if (j != i)
						{
							printf("보내는 데이터: \n%s\n", "정답자가 나왔습니다.");
							m_socCom[j]->Send("정답자가 나왔습니다.", 256);
						}
					printf("보내는 데이터: \n%s\n", "정답을 맞췄습니다.");
					m_socCom[i]->Send("정답을 맞췄습니다.", 256);
					CString score;
					score.Format("%d", totalScore[i]);
					score = "현재 SCORE : " + score;
					printf("보내는 데이터: \n%s\n", score);
					m_socCom[i]->Send(score, 256);

					//남은 문제수를 줄이고 다음문제 버튼을 활성화 합니다
					m_restQ--;

					if (m_restQ != 0) {
						//다음문제로 세팅
						m_qWord.SetWindowTextA(Q.word[10 - m_restQ]);

						DeleteContents();
						// 남은 문제수를 띄웁니다
						CString strTmp;
						strTmp.Format("%d", m_restQ);
						m_strRestQ.SetWindowTextA(strTmp);


						// 타이머를 설정합니다
						strTmp.Format("%d", Q.time[10 - m_restQ]);
						m_strTime.SetWindowTextA(strTmp);

						//남은 문제수를 갱신합니다.
						CString restQ;
						restQ.Format("%d", m_restQ);
						m_strRestQ.SetWindowTextA(restQ);

						m_bNextQ.EnableWindow(TRUE);
						return TRUE;
					}
					//남은 문제가 0개인 경우
					else 
					{
						gameState = 2;

						m_list.InsertString(k, "게임이 끝났습니다.");
						for (int j = 0; j < userCount; j++) {
							printf("보내는 데이터: \n%s\n", "게임이 끝났습니다.");
							m_socCom[j]->Send("게임이 끝났습니다.", 256);
						}
							


						//게임이 끝났을 때의 메소드를 추가합니다.
						max_idx = 0;
						for (int k = 1; k < userCount - 1; k++)
							if (totalScore[max_idx] < totalScore[k])
								max_idx = k;
						printf("보내는 데이터: \n%s\n", "우승자입니다");
						m_socCom[max_idx]->Send("우승자입니다", 256);

						if (RankScore[9] < totalScore[max_idx]) {
							printf("보내는 데이터: \n%s\n", "ranker");
							m_socCom[max_idx]->Send("ranker", 256);



						}

						for (int i = 0; i < userCount; i++) {
							if (i != max_idx) {
								printf("보내는 데이터: \n%s\n", "wait");
								m_socCom[i]->Send("wait", 256);


							}
						}
						if (totalScore[max_idx] > RankScore[9])
							nameSendCheck = TRUE;


						return TRUE;
					}
				
				}
				else {
					printf("%s\n", "정답이 아님");
				}
			}
			
		}
	}
	
	
	//리스트박스에 보여준다
	int i = m_list.GetCount();
	m_list.InsertString(i, strTmp);
	return 0;
}



void CChatServerDlg::DrawCircle(CDC* pDC, CPoint point, int radius)
{
	printf("DrawCircle 호출\n");

	// TODO: 여기에 구현 코드 추가.
	pDC->Ellipse(((int)(point.x) - radius)
		, ((int)(point.y) - radius)
		, ((int)(point.x) + radius)
		, ((int)(point.y) + radius));
}


void CChatServerDlg::DrawCircle(CDC* pDC, CBrush* brush, CPoint point, int radius)
{
	printf("DrawCircle_Brush 호출\n");

	// TODO: 여기에 구현 코드 추가.
	pDC->SelectObject(brush);
	DrawCircle(pDC, point, radius);
}




void CChatServerDlg::OnMouseMove(UINT nFlags, CPoint point)
{	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nFlags & MK_LBUTTON) //왼쪽 버튼을 누른 상태일때
	{
		struct Data data;
		struct lineData line;
		CClientDC dc(this);
		CDC *pDC = m_rectView.GetWindowDC();
		CRect rect;
		CPoint curPos;
		m_rectView.GetWindowRect(rect);
		ScreenToClient(rect);

		curPos = point - rect.TopLeft();

		CLine *pLine = AddLine(m_ptPrevPos, curPos, m_color, m_nPenWidth);
		memcpy(line.prev, &m_ptPrevPos, sizeof(m_ptPrevPos));
		memcpy(line.pos, &curPos, sizeof(curPos));
		memcpy(line.color, &m_color, sizeof(m_color));
		memcpy(line.penwidth, &m_nPenWidth, sizeof(m_nPenWidth));

		data.Opcode = 0x03;
		memcpy(data.Operand, &line, sizeof(line));

		SendData(data);//클라이언트쪽에 뿌려주기
		pLine->Draw(pDC, m_color, m_nPenWidth);
		m_ptPrevPos = curPos;

		// 현재 좌표를 저장한다.
		//m_ptPrevPos = point;
		//dc.SelectObject(pOldPen);
		//	

		//m_pntCur = point;
	}
		CDialogEx::OnMouseMove(nFlags, point);
}


void CChatServerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	printf("OnLButtonDown 이벤트발생\n");

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;

	SetCapture();
	m_rectView.GetWindowRect(rect);
	ScreenToClient(rect);
	m_ptPrevPos = point - rect.TopLeft();

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CChatServerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	printf("OnLButtonUp 이벤트발생\n");

	ReleaseCapture();
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CChatServerDlg::OnBnClickedCheckEraser()
{
	printf("OnBnClickedCheckEraser 이벤트발생\n");

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_isErase) {
		m_colorPrev = m_color;
		m_nPenWidthPrev = m_nPenWidth;
		m_color = RGB(255, 255, 255);
		m_nPenWidth = 20;
		UpdateData(FALSE);
	}
	else {
		m_color = m_colorPrev;
		m_nPenWidth = m_nPenWidthPrev;
		UpdateData(FALSE);
	}
}

void CChatServerDlg::OnClickedButtonColor()
{
	printf("OnClickedButtonColor 이벤트발생\n");

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CColorDialog dlgColor;
	if (dlgColor.DoModal() == IDOK) {
		m_color = dlgColor.GetColor();
		CDC *pic = m_colorView.GetWindowDC();
		CBrush brush;
		CRect rect;
		GetWindowRect(rect);
		ScreenToClient(rect);
		brush.CreateSolidBrush(m_color);
		pic->SelectObject(&brush);
		pic->FillRect(rect, &brush);
		brush.DeleteObject();
	}
}

void CChatServerDlg::OnEnChangeEditPenwidth()
{
	printf("OnEnChangeEditPenwidth 이벤트발생\n");

	UpdateData(TRUE);
}

void CChatServerDlg::OnClickedButtonAllclear()
{
	printf("OnClickedButtonAllclear 이벤트발생\n");

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DeleteContents();
	for (int j = 0; j < userCount; j++)
		m_socCom[j]->Send("그림을 초기화합니다.", 256);
}




CLine * CChatServerDlg::AddLine(CPoint ptFrom, CPoint ptTo, COLORREF color, int nPenWidth)
{
	printf("AddLine 호출\n");

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
}

void CChatServerDlg::DeleteContents()
{
	printf("DeleteContents 호출\n");

	int liCount = m_oaLines.GetSize();
	int liPos;

	if (liCount) {
		for (liPos = 0; liPos < liCount; liPos++) delete m_oaLines[liPos];
		m_oaLines.RemoveAll();
	}
	OnPaint();
}

void CChatServerDlg::SendData(struct Data &data)
{
	// TODO: 여기에 구현 코드 추가.
	printf("SendData 호출\n");

	CString strData;
	char temp[256];
	memcpy(temp, &data, sizeof(temp));
	for (int i = 0; i < userCount; i++)
	{
		m_socCom[i]->Send(temp, sizeof(temp));
	}
}


void CChatServerDlg::OnBnClickedButtonGstart()
{
	printf("OnBnClickedButtonGstart 이벤트\n");

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//게임을 시작합니다
	gameState = 1;
	//게임시작 메시지 전송
	CString strTmp;
	char pTmp[256] = "게임을 시작합니다. 참여유저 수 : ";
	char usercount[10];
	sprintf(usercount, "%d", userCount);
	if (userCount == 0)
	{
		strTmp.Format("%s", "참여할 유저가 없습니다.");
		int i = m_list.GetCount();
		m_list.InsertString(i, strTmp);
		return ;
	}
	strcat(pTmp, usercount);
	strTmp.Format("%s", pTmp);
	
	int i = m_list.GetCount();
	m_list.InsertString(i, strTmp);

	for (int j = 0; j < userCount; j++)
		m_socCom[j]->Send(pTmp, 256);

	//제시어를 초기화 합니다.
	m_restQ = 3;

	Q.word[0] = "시험";
	Q.time[0] = 15;
	Q.word[1] = "오동나무";
	Q.time[1] = 20;
	Q.word[2] = "우거지국";
	Q.time[2] = 14;
	Q.word[3] = "장갑차";
	Q.time[3] = 15;
	Q.word[4] = "가격표";
	Q.time[4] = 16;
	Q.word[5] = "가로수";
	Q.time[5] = 17;
	Q.word[6] = "공중전화";
	Q.time[6] = 28;
	Q.word[7] = "백수";
	Q.time[7] = 22;
	Q.word[8] = "김치";
	Q.time[8] = 29;
	Q.word[9] = "샴푸";
	Q.time[9] = 30;


	//다음 문제 버튼을 활성화 합니다.
	
	m_bNextQ.EnableWindow(FALSE);
	m_bNextQ.ShowWindow(SW_SHOW);//숨긴다

	CString str;
	str.Format("%s", Q.word[10 - m_restQ]);
	m_qWord.SetWindowTextA(str);



}


void CChatServerDlg::OnBnClickedButtonNextq()
{
	printf("OnBnClickedButtonNextq 이벤트\n");

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 제시어를 띄웁니다
	m_qWord.SetWindowTextA(Q.word[10 - m_restQ]);

	DeleteContents();

	// 남은 문제수를 띄웁니다
	CString strTmp;
	strTmp.Format("%d", m_restQ);
	m_strRestQ.SetWindowTextA(strTmp);
	

	// 타이머를 설정합니다
	strTmp.Format("%d", Q.time[10 - m_restQ]);
	m_strTime.SetWindowTextA(strTmp);

	//버튼을 비활성화 합니다.
	m_bNextQ.EnableWindow(FALSE);
	
}


BOOL CChatServerDlg::OpenDatabase()
{

	TRY
	{
		g_dbRank.OpenEx(_T("DSN=MysqlODBC;UID=root;PWD=1234"),CDatabase::noOdbcDialog);
	}CATCH(CException, e)
	{
		m_test = "데이터베이스개방 실패";
		UpdateData(FALSE);
		TCHAR szCause[255];
		e->GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause, MB_ICONERROR);
		if (g_dbRank.IsOpen()) g_dbRank.Close();
		return FALSE;
	}
	END_CATCH
		m_test = "데이터베이스개방 성공";
	UpdateData(FALSE);
	return TRUE;
}

void CChatServerDlg::ExitInstance()
{
	if (g_dbRank.IsOpen())
	{
		g_dbRank.Close();
	}

}



void CChatServerDlg::setRankInfo()
{
	// TODO: 여기에 구현 코드 추가.
	CString strSQL;
	CString strTOTALSCORE;
	CRecordset recSet(&g_dbRank);
	int i = 0;
	TRY
	{

		strSQL = "SELECT USERNAME,TOTALSCORE,PLAYDATE FROM USERRANK ORDER BY TOTALSCORE DESC";
		printf("%s\n", strSQL);
		recSet.Open(CRecordset::forwardOnly, strSQL);
		int num = 0;

		while (!recSet.IsEOF())
		{
			recSet.GetFieldValue("TOTALSCORE", strTOTALSCORE);
			RankScore[i] = atoi(strTOTALSCORE);
			i++;
			recSet.MoveNext();
		}
		printf("%s", "랭커점수들");
		for(int i=0;i<10;i++)
			printf("%d", RankScore[i]);


		
	}CATCH(CException, e)
	{
		TCHAR szCause[255];
		e->GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause, MB_ICONERROR);
		if (g_dbRank.IsOpen()) g_dbRank.Close();
	}
	END_CATCH

	

}



void CChatServerDlg::OnBnClickedButton1()
{
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
	for (int i = 0;i < userCount;i++)
		m_socCom[i]->Send(pTmp, 256);

	//전송한 데이터도 리스트박스에 보여준다
	strTmp.Format("%s", pTmp);
	int i = m_list.GetCount();
	m_list.InsertString(i, strTmp);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
