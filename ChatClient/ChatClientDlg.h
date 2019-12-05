
// ChatClientDlg.h: 헤더 파일
//
#pragma warning(disable : 4996)
#pragma once
#include "Line.h"


// CChatClientDlg 대화 상자
class CChatClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CChatClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT_DIALOG };
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

//	afx_msg void OnButtonSend();
	//}}AFX_MSG
	afx_msg LPARAM OnAccept(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list;
//	CString m_strSend;
	CString m_strIP;
	CSocCom m_socCom; //통신용 소켓
	void OnButtonConnect();
	afx_msg void OnBnClickedButtonConnect();

	void DataAnalysis(char* data);

	//선을 그리기위한 변수 
	CPoint m_pntOld, m_pntCur; //그리기 좌표
	CStatic m_rectView;			//그림을 그릴 메인 Picture Control
	CStatic m_ctlPaint;
	int m_penWidth;
	COLORREF m_color;

	CObArray m_oaLines;
	CLine* AddLine(CPoint ptFrom, CPoint ptTo, COLORREF color, int nPenWidth);
	void StrToData(char* data);
	void DeleteContents();
	CEdit m_strSend;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL isRankSend;
	CButton m_btnConnect;
};
