#include "Figure.h"

Figure::Figure()
{
}

void Figure::setColor(COLORREF color)
{
	this->color = color;
}

COLORREF Figure::getColor()
{
	return COLORREF(this->color);
}

void Figure::setFrameWidth(int width)
{
	this->nFrameWidth = width;
}

int Figure::getFrameWidth()
{
	return this->nFrameWidth;
}

void Figure::setHeight(int height)
{
	this->height = height;
}

void Figure::setWidth(int width)
{
	this->width = width;
}

void Figure::setX(int x)
{
	this->pos.left = x;
}

void Figure::setY(int y)
{
	this->pos.top = y;
}

int Figure::getX()
{
	return pos.left;
}

int Figure::getY()
{
	return pos.top;
}

Figure::~Figure()
{
}
