#pragma once
#include "stdafx.h"
class Figure
{
public:
	Figure();
	void setColor(COLORREF color);
	COLORREF getColor();
	void setFrameWidth(int width);
	int getFrameWidth();
	void setHeight(int height);
	void setWidth(int width);
	void setX(int x);
	void setY(int y);
	int getX();
	int getY();
	virtual void draw(HDC & hdc, HPEN &hPen) = 0;
	virtual bool isClicked(POINT &pt) = 0;
	virtual ~Figure();
protected:
	int height, width;
	int nFrameWidth;
	COLORREF color;
	RECT pos;

};