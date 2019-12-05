
// ChatServerDlg.h: 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Line.h"
#define USERNUM 2
#define PLAYDATE "2019-01-16"
//제시어 집합 구조체
typedef struct Question {
	//char word[10][10];
	CString word[10];
	int time[10];
}Question;


// CChatServerDlg 대화 상자
class CChatServerDlg : public CDialogEx
{
	// 생성입니다.
public:
	CChatServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	//메인 프로그램에서 쓸 소켓 선언
	CSocServer m_socServer; // 서버용 소켓
	CSocCom* m_socCom[USERNUM]; //통신용 소켓
	int userCount;
	int totalScore[USERNUM];

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
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	//afx_msg void OnClose();
	//afx_msg void OnButton2();
//	afx_msg void OnButtonSend();
	//}}AFX_MSG
	afx_msg LRESULT OnAccept(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list;
//	CString m_strSend;
	CString m_strStatus;
	CSocCom *pRcv;
protected:

public:
	CString m_test;
	void DrawCircle(CDC* pDC, CPoint point, int radius);
	void DrawCircle(CDC* pDC, CBrush* brush, CPoint point, int radius);

	//선을 그리기위한 변수 
	int m_nPenWidth;            //현재의 펜굵기 저장
	int m_nPenWidthPrev;         //지우개를 눌렀을때 이전의 펜굵기 저장
	COLORREF m_color;            //현재의 색상을 저장
	COLORREF m_colorPrev;         //지우개를 눌렀을때 이전의 색상 저장
	CObArray m_oaLines;            //직선객체를 저장,관리하기위한 변수 
	CPoint m_ptPrevPos;            //직전의 좌표 저장

	//다이얼로그의 컨트롤 변수 
	CStatic m_rectView;         //그림을 그릴 메인 Picture Control
	CStatic m_colorView;      //현재읭 색상을 나타네는 Picture Control
	BOOL m_isErase;         //현재 그리기도구중 지우개를 선택하였는지 아닌지

	//제시어 구조체 선언
	Question Q;
	
	afx_msg void OnBnClickedButtonGstart();
//	int m_restQ;
	afx_msg void OnBnClickedButtonNextq();
	BOOL OpenDatabase();
	void ExitInstance();
//	CString m_qWord;
	CButton m_bNextQ;
//	CString m_qWord;
	CStatic m_qWord;
	CStatic m_strRestQ;
	int m_restQ;
	CStatic m_strTime;
	CDatabase g_dbRank;
public:
	//사용자 정의 함수들

	//직선을 m_oaLines에 추가하기위한 함수
	CLine* AddLine(CPoint ptFrom, CPoint ptTo, COLORREF color, int nPenWidth);

	void DeleteContents();                           //내용물을 초기화하기 위한 함수 
	void SendData(struct Data &data);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckEraser();
	afx_msg void OnClickedButtonColor();
	afx_msg void OnEnChangeEditPenwidth();
	afx_msg void OnClickedButtonAllclear();

	int max_idx;
	int RankScore[10];
	int getScore(int order);
	BOOL nameSendCheck;
//	BOOL gameEndCheck;
	CRecordset recSet;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeEditSend();
	afx_msg void OnStnClickedStaticQ();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit m_strSend;
//	BOOL gameStartCheck;
	int gameState;
	void setRankInfo();
	afx_msg void OnBnClickedButton1();
};