#pragma once
#include "stdafx.h"
#include "resource.h"
class MyFigure
{
private:
	POINT pt[MAX_POINTS];
	SIZE ptDelta[MAX_POINTS-1];
	HRGN hMyRegion;
	COLORREF color;
	int nFrameWidth;
	int nTopPt;
public:
	MyFigure();
	void init();
	void setCurrPt(POINT in_pt, int i);
	POINT getCurrPt(int i);
	void draw(HDC &hdc, HPEN &hPen);
	void move(POINT & curPt, POINT & prevPt, HDC &hdc);
	void initRgn(HDC &hdc);
	bool isClicked(POINT &pt);
	void setColor(COLORREF color);
	void setFrameWidth(int width);
	void setDelta(SIZE delta);
	~MyFigure();
};

