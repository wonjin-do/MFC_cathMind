// Line.cpp : implementation file
//

#include "stdafx.h"
#include "Line.h"

#define Red RGB(255,0,0)
#define Blue RGB(0,0,255)
#define Green RGB(0,255,0)
IMPLEMENT_SERIAL(CLine, CObject, 1)

// CLine

CLine::CLine()
{
}

CLine::~CLine()
{
}


// CLine member functions


CLine::CLine(CPoint ptTo, CPoint ptFrom, COLORREF lineColor, int nLineWidth = 3)
{
	m_ptFrom = ptFrom;
	m_ptTo = ptTo;
	m_colorLine = lineColor;
	m_nLineWidth = nLineWidth;
}


void CLine::Draw(CDC* pDC)
{
	CPen pen, *oldpen;
	pen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	oldpen = pDC->SelectObject(&pen);
	pDC->MoveTo(m_ptFrom);
	pDC->LineTo(m_ptTo);
	pen.DeleteObject();
}

void CLine::Draw(CDC* pDC, COLORREF LineColor, int nLineWidth)
{
	CPen pen, *oldpen;
	pen.CreatePen(PS_SOLID, nLineWidth, LineColor);
	oldpen = pDC->SelectObject(&pen);
	pDC->MoveTo(m_ptFrom);
	pDC->LineTo(m_ptTo);
	pen.DeleteObject();
}


void CLine::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);

	if (ar.IsStoring())
		ar << m_ptFrom << m_ptTo << m_colorLine << m_nLineWidth;
	else {
		ar >> m_ptFrom >> m_ptTo >> m_colorLine << m_nLineWidth;
	}
}
