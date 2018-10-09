#pragma once
#include <SDL.h>
#include "Types.h"

bool Collision(Vec2 vec, Rect rect)
{
	return vec.x > rect.x && vec.y > rect.y && vec.x < rect.w + rect.x && vec.y < rect.h + rect.y;
}

bool rectCollision(Rect rect1, Rect rect2) 
{
	return (Collision(Vec2{ rect1.x, rect1.y }, rect2) 
		|| Collision(Vec2{ rect1.x + rect1.w, rect1.y }, rect2) 
		|| Collision(Vec2{ rect1.x,rect1.y + rect1.h }, rect2)
		|| Collision(Vec2{ rect1.x + rect1.w, rect1.y + rect1.h }, rect2));
}








