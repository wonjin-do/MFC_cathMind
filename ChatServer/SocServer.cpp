// SocServer.cpp: 구현 파일
//

#include "stdafx.h"
#include "ChatServer.h"
#include "SocServer.h"


// CSocServer

CSocServer::CSocServer()
{
}

CSocServer::~CSocServer()
{
}


// CSocServer 멤버 함수


void CSocServer::Init(HWND hWnd)
{
	// TODO: 여기에 구현 코드 추가.
	//메인 윈도우 포인터 받기
	m_hWnd = hWnd;
	userCount = 0;
}


void CSocServer::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//클라이언트가 접속하면 메인 윈도우에 접속 메시지(UM_ACCEPT)를 보낸다
	Accept(m_socCom[userCount]); //Accept 함수를 호출
	userCount++; 
	SendMessage(m_hWnd, UM_ACCEPT, 0, 0); //메인윈도우에 당 함수가 호출됨을 알림
	CSocket::OnAccept(nErrorCode);
}


CSocCom* CSocServer::GetAcceptSocCom()
{
	// TODO: 여기에 구현 코드 추가.
	// 통신 소켓을 리턴한다.
	// 반환되는 통신 소켓은 클라이언트와 연결된다.
	return &m_socCom[userCount-1];
}


