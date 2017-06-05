#pragma once
#include "Figure.h"

class Ellipses
	: public Figure
{
private:
	HRGN hEllipseRgn;
	void initRgn();
public:
	Ellipses();
	void draw(HDC & hdc, HPEN &hPen);
	bool isClicked(POINT &pt);
	~Ellipses();
};