#pragma once

int speedMovement = 5;

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
	bool getCoins;
	bool goUp, goDown, goLeft, goRight;
	Player() :score{ 0 }, getCoins{ false }, goUp { false }, goDown{ false }, goRight{ false }, goLeft{ false } {};
};

