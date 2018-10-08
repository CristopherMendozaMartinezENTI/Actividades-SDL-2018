#pragma once

int speedMovement = 2;

struct Vec2
{
	int x, y;
};

struct Rect
{
	int x, y, w, h;
	Rect(SDL_Rect rect) :x{ rect.x }, y{ rect.y }, w{ rect.w }, h{ rect.h } {};
};

struct Player
{
	//Directions
	bool goUp, goDown, goLeft, goRight;
	Player() :goUp{ false }, goDown{ false }, goRight{ false }, goLeft{ false } {};
};
