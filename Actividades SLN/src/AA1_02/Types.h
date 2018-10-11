#pragma once
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define FPS  60
#define DELAY_TIME 1000.0f / FPS
#define MAX_TIME 60000
#define AMOUNT_OF_COINS 10
#define PLAYER_SIZE 2
#define MOTION_SPEED 5

enum gameStates {
	MENU,
	IN_GAME,
};

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

