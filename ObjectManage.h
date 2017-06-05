#pragma once
#include "MyFigure.h"
#include "Figure.h"

void ObjectDraw(HDC &hMem, INT nTop, HPEN &hPen, Figure **cWindow, HWND &hWnd, RECT &WndRect);
void MyObjectDraw(HDC &hMem, INT nTop, HPEN &hPen, MyFigure *cWindow, HWND &hWnd, RECT &WndRect);

template <class TypeClass>
void ObjectInit(TypeClass *tmp, int &nTopFigure)
{
	tmp->setFrameWidth(nEdtPenWidth);
	tmp->setColor(PenColor);
	tmp->setX(10);
	tmp->setY(10);
	tmp->setHeight(nEdtHeight);
	tmp->setWidth(nEdtWidth);
	tmp->setFrameWidth(nEdtPenWidth);
	Window[nTopFigure] = tmp;
}