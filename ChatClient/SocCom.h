#pragma once

// CSocCom 명령 대상

class CSocCom : public CSocket
{
public:
	HWND m_hWnd; // 메인 윈도우 핸들
	CSocCom();
	virtual ~CSocCom();
	void Init(HWND hWnd);
	virtual void OnReceive(int nErrorCode);
};


