#include "Ellipses.h"



void Ellipses::initRgn()
{
	if (hEllipseRgn) DeleteObject(hEllipseRgn);
	hEllipseRgn = CreateEllipticRgnIndirect(&this->pos);
}

Ellipses::Ellipses()
{
}

void Ellipses::draw(HDC & hdc, HPEN & hPen)
{
	if (hPen) DeleteObject(hPen);
	hPen = CreatePen(PS_SOLID, nFrameWidth, color);
	SelectObject(hdc, hPen);
	this->pos.right = pos.left + width;
	this->pos.bottom = pos.top + height;
	Ellipse(hdc, pos.left, pos.top, pos.right, pos.bottom);
}

bool Ellipses::isClicked(POINT &pt)
{
	this->initRgn();
	return PtInRegion(hEllipseRgn, pt.x, pt.y);
}

Ellipses::~Ellipses()
{
	if (hEllipseRgn) DeleteObject(hEllipseRgn);
}
