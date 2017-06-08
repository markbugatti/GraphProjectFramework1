#include "Triangles.h"

Triangles::Triangles()
{
}

void Triangles::initRgn(HDC &hdc)
{
	ptTriangle[0].x = pos.left;
	ptTriangle[0].y = pos.bottom = pos.top + height;
	ptTriangle[1].x = pos.left + (width / 2);
	ptTriangle[1].y = pos.top;
	ptTriangle[2].x = pos.right = pos.left + width;
	ptTriangle[2].y = pos.bottom;
	BeginPath(hdc);
	Polyline(hdc, ptTriangle, 3);
	CloseFigure(hdc);
	EndPath(hdc);
	if (hTriangleRgn) DeleteObject(hTriangleRgn);
	hTriangleRgn = PathToRegion(hdc);
}

void Triangles::draw(HDC & hdc, HPEN & hPen)
{
	if (hPen) DeleteObject(hPen);
	hPen = CreatePen(PS_SOLID, nFrameWidth, color);
	SelectObject(hdc, hPen);
	initRgn(hdc);
	MoveToEx(hdc, ptTriangle[0].x, ptTriangle[0].y, NULL);
	LineTo(hdc, ptTriangle[1].x, ptTriangle[1].y);
	LineTo(hdc, ptTriangle[2].x, ptTriangle[2].y);
	LineTo(hdc, ptTriangle[0].x, ptTriangle[0].y);
	PaintRgn(hdc, hTriangleRgn);
}

bool Triangles::isClicked(POINT & pt)
{
	return PtInRegion(hTriangleRgn, pt.x, pt.y);
}

Triangles::~Triangles()
{
}
