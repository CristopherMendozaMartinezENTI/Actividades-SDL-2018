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
	int score;
	bool goUp, goDown, goLeft, goRight, iddle;
	Player() :score{ 0 }, goUp { false }, goDown{ false }, goRight{ false }, goLeft{ false }, iddle{ false } {};
};
