#pragma once

struct Vec2
{
	int x, y;
};

struct Rect
{
	int x, y, w, h;
	Rect(SDL_Rect rect) :x{ rect.x }, y{ rect.y }, w{ rect.w }, h{ rect.h } {};
};

