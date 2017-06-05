#pragma once
#include "Figure.h"

class Triangles
	: public Figure
{
private:
	HRGN hTriangleRgn;
	POINT ptTriangle[3];
	void initRgn(HDC &hdc);
public:
	Triangles();
	void draw(HDC & hdc, HPEN &hPen);
	bool isClicked(POINT &pt);
	~Triangles();
};

