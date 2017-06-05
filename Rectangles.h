#pragma once
#include "Figure.h"

class Rectangles
	: public Figure
{
public:	
	Rectangles();
	void draw(HDC & hdc, HPEN &hPen);
	bool isClicked(POINT &pt);
	~Rectangles();
};