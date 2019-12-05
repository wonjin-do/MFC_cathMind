#pragma once



// CSocServer 명령 대상

class CSocServer : public CSocket
{
public:
	HWND m_hWnd; // 메인 윈도우 핸들
	CSocServer();
	virtual ~CSocServer();
	void Init(HWND hWnd);
	virtual void OnAccept(int nErrorCode);
	CSocCom m_socCom[2];
	CSocCom* GetAcceptSocCom();
	CSocCom* pRcv;
	int userCount;
};


