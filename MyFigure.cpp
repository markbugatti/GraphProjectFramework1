#include "MyFigure.h"

MyFigure::MyFigure()
{
}

void MyFigure::init()
{
	for (int i = 0; i < nTopPt; i++) {
		pt[i] = { 0 };
		ptDelta[i] = { 0 };
	}
	if (hMyRegion) DeleteObject(hMyRegion);
	color = 0;
	nFrameWidth = 0;
	nTopPt = 0;
}

void MyFigure::setCurrPt(POINT in_pt, int i)
{
	pt[i] = in_pt;
	if(i > 0) { 
		ptDelta[i].cx = pt[0].x - in_pt.x;
		ptDelta[i].cy = pt[0].y - in_pt.y;
	}
	nTopPt++;
}

POINT MyFigure::getCurrPt(int i)
{
	return POINT(pt[i]);
}

void MyFigure::initRgn(HDC &hdc)
{
	/* creating path */ 
	BeginPath(hdc);
	Polyline(hdc, pt, nTopPt);
	CloseFigure(hdc);
	EndPath(hdc);
	/**/
	/* convert to Region*/
	if (hMyRegion) DeleteObject(hMyRegion);
	hMyRegion = PathToRegion(hdc);
	/**/
}

void MyFigure::draw(HDC & hdc, HPEN & hPen)
{
	if (hPen) DeleteObject(hPen);
	hPen = CreatePen(PS_SOLID, nFrameWidth, color);
	SelectObject(hdc, hPen);
	PaintRgn(hdc, hMyRegion);
	Polyline(hdc, pt, nTopPt);
}


void MyFigure::move(POINT &curPt, POINT & prevPt, HDC &hdc)
{
	pt[0].x = curPt.x + ptDelta[0].cx;
	pt[0].y = curPt.y + ptDelta[0].cy;
	for (int i = 1; i < nTopPt; i++)
	{
		pt[i].x = pt[0].x - ptDelta[i].cx;
		pt[i].y = pt[0].y - ptDelta[i].cy;
	}
}

bool MyFigure::isClicked(POINT &pt)
{
	return PtInRegion(hMyRegion, pt.x, pt.y);
}

void MyFigure::setColor(COLORREF color)
{
	this->color = color;
}

void MyFigure::setFrameWidth(int width)
{
	this->nFrameWidth = width;
}

void MyFigure::setDelta(SIZE delta)
{
	this->ptDelta[0] = delta;
}


MyFigure::~MyFigure()
{
}
