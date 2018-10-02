#pragma once
#include <SDL.h>
#include "Types.h"

bool Collision(Vec2 vec, Rect rect)
{
	return vec.x > rect.x && vec.y > rect.y && vec.x < rect.w + rect.x && vec.y < rect.h + rect.y;
}
