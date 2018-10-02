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
	return vec.x > rect.x && vec.y > rect.y && vec.x < rect.w + rect.x && vec.y < rect.h + rect.y;
}