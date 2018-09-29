#pragma once
#include <SDL.h>
#include "Types.h"

Rect getRectComponents(SDL_Rect rect) {
	Rect myRect;
	myRect.x = rect.x;
	myRect.y = rect.y;
	myRect.height = rect.h;
    myRect.width = rect.w;
	return myRect;
}

bool Collision(Vec2 vec, SDL_Rect rect)
{
	return ((vec.x - (rect.x + rect.w / 2) < (rect.w / 2)) && ((vec.y - (rect.y + rect.h) / 2) < (rect.h / 2)));
}