#include "ObjectManage.h"
void ObjectDraw(HDC &hMem, INT nTop, HPEN &hPen, Figure **cWindow, HWND &hWnd, RECT &WndRect) {
	PatBlt(hMem, 0, 0, WndRect.right, WndRect.bottom, WHITENESS);
	for (int i = 0; i <= nTop; i++)
		cWindow[i]->draw(hMem, hPen);
	InvalidateRect(hWnd, NULL, FALSE);
}

void MyObjectDraw(HDC & hMem, INT nTop, HPEN & hPen, MyFigure * cWindow, HWND & hWnd, RECT & WndRect)
{
	PatBlt(hMem, 0, 0, WndRect.right, WndRect.bottom, WHITENESS);
	for (int i = 0; i <= nTop; i++)
		cWindow[i].draw(hMem, hPen);
	cWindow[nTop].initRgn(hMem);
	InvalidateRect(hWnd, NULL, FALSE);
}