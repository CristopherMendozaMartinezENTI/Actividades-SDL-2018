#pragma once
#include <SDL.h>
#include "Types.h"

bool Collision(Vec2 vec, SDL_Rect rect)
{
	return vec.x < rect.x || vec.y < rect.y || vec.x < rect.x + rect.w || vec.y < rect.y + rect.h;
}