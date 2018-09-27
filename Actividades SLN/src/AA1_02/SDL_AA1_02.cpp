#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <exception>
#include <iostream>
#include <string>
#include "Collisions.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main(int, char*[])
{
	// --- INIT SDL ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		throw "No es pot inicialitzar SDL subsystems";

	// --- WINDOW ---
	SDL_Window *m_window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (m_window == nullptr)
		throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *m_renderer{ SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (m_renderer == nullptr)
		throw "No es pot inicialitzar SDL_Renderer";

	//-->SDL_Image 
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";

	//-->SDL_TTF
	if (TTF_Init() != 0) throw "No es pot inicialitzar SDL_ttf";
	
	//-->SDL_Mix

	// --- SPRITES ---
		//Background
	SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg") };
	if (bgTexture == nullptr) throw "Error: bgTexture init";
	SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

	//-->Animated Sprite ---

	//-->Player ---
	SDL_Texture *playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/kintoun.png") };
	if (playerTexture == nullptr) throw "Error: platerTexture init";
	SDL_Rect playerRect{ 0, 0, 350, 190 };
	SDL_Rect playerTarget{ 0, 0, 100, 100 };

	// --- TEXT ---
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 140) };
	if (font == nullptr) throw "No es pot inicialitzar SDL_ttf";
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "Anem a buscar, la bola de drac", SDL_Color{0,0,0,0}) };
	if (tmpSurf == nullptr) throw "No es pot crear SDL surface";
	SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Rect textRect{ (SCREEN_WIDTH - tmpSurf->w) / 2, 50, tmpSurf->w, tmpSurf->h };

	tmpSurf = { TTF_RenderText_Blended(font, "Anem a buscar, la bola de drac", SDL_Color{255,255,255,0}) };
	SDL_Texture *textHover{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Texture *textAux{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

	// --- AUDIO ---

	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	while (isRunning) {

		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
				break;
			case SDL_MOUSEMOTION:
				if (Collision(Vec2{ event.motion.x, event.motion.y }, textRect))
					textAux = textHover;
				else 
					textAux = textTexture;
				playerTarget.x = event.motion.x - 150;
				playerTarget.y = event.motion.y - 90;
				break;
			default:;
			}
		}

		// UPDATE
		playerRect.x += (playerTarget.x - playerRect.x) / 5;
		playerRect.y += (playerTarget.y - playerRect.y) / 5;

		// DRAW
		SDL_RenderClear(m_renderer);
		//Background
		SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect);
		//Player
		SDL_RenderCopy(m_renderer, playerTexture, nullptr, &playerRect);
		//Text
		SDL_RenderCopy(m_renderer, textTexture, nullptr, &textRect);
		SDL_RenderPresent(m_renderer);
	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_FreeSurface(tmpSurf);
	TTF_CloseFont(font);
	IMG_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	// --- QUIT ---
	SDL_Quit();

	return 0;
}
