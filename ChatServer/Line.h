#pragma once

// CLine command target

class CLine : public CObject
{
	DECLARE_SERIAL(CLine)
public:
	CLine();
	virtual ~CLine();
private:
	CPoint m_ptFrom;
	CPoint m_ptTo;
public:
	COLORREF m_colorLine;
	int m_nLineWidth;
	CLine(CPoint ptTo, CPoint ptFrom, COLORREF lineColor, int nLineWidth);
	void Draw(CDC* pDC);
	void Draw(CDC* pDC, COLORREF LineColor, int nLineWidth);
	void Serialize(CArchive& ar);
};


