#include "Rectangles.h"

Rectangles::Rectangles()
{
}

void Rectangles::draw(HDC &hdc, HPEN &hPen)
{
	if (hPen) DeleteObject(hPen);
	hPen = CreatePen(PS_SOLID, nFrameWidth, color);
	SelectObject(hdc, hPen);
	this->pos.right = pos.left + width;
	this->pos.bottom = pos.top + height;
	Rectangle(hdc, pos.left, pos.top, pos.right, pos.bottom);
}

bool Rectangles::isClicked(POINT & pt)
{
	return PtInRect(&pos, pt);
}

Rectangles::~Rectangles()
{
}