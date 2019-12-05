﻿
// ChatClient.h: PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


// CChatClientApp:
// 이 클래스의 구현에 대해서는 ChatClient.cpp을(를) 참조하세요.
//

class CChatClientApp : public CWinApp
{
public:
	CChatClientApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	

// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CChatClientApp theApp;

struct Data {
	char Opcode;
	char Operand[255];
};

struct lineData {
	char prev[8];
	char pos[8];
	char color[4];
	char penwidth[4];
};