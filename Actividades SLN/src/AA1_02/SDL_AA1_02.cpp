#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <exception>
#include <iostream>
#include <string>
#include "Collisions.h"
#include <SDL_mixer.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

enum gameStates {
	MENU,
	INGAME,
	MAX
};

int main(int, char*[])
{
// --- Init ---
#pragma region InitSDL

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
	const Uint8 soundFlags{ MIX_INIT_MP3 };
	if (!(IMG_Init(soundFlags) & soundFlags)) throw "Error: SDL_image init";

#pragma endregion 	

// --- SPRITES ---
#pragma region Backgrounds

	//Main Menu Background
	SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg") };
	if (bgTexture == nullptr) throw "Error: bgTexture init";
	SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

	//In game Backgroung
	SDL_Texture* gameBgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bgCastle.jpg") };
	if (bgTexture == nullptr) throw "Error: bgTexture init";
	SDL_Rect gameBgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

#pragma endregion

#pragma region Cursor

	//Cursor
	SDL_Texture *playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/kintoun.png") };
	if (playerTexture == nullptr) throw "Error: platerTexture init";
	SDL_Rect playerRect{ 0, 0, 350, 190 };
	SDL_Rect playerTarget{ 0, 0, 100, 100 };
#pragma endregion

#pragma endregion

#pragma region Title Text

	//Title Text
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 80) };
	if (font == nullptr) throw "No es pot inicialitzar SDL_ttf";
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "Anem a buscar, la bola de drac", SDL_Color{255,128,0,0}) };
	if (tmpSurf == nullptr) throw "No es pot crear SDL surface";
	SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Rect textRect{ (SCREEN_WIDTH - tmpSurf->w) / 2, 50, tmpSurf->w, tmpSurf->h };

#pragma endregion

#pragma region Play Button 

	//Play Button (Menu)
	tmpSurf = { TTF_RenderText_Blended(font, "Play", SDL_Color{ 255,128,0,0 }) };
	SDL_Texture *playTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Rect playButtonRect{ (SCREEN_WIDTH - tmpSurf->w) / 2, 500, tmpSurf->w, tmpSurf->h };
	tmpSurf = { TTF_RenderText_Blended(font, "Play", SDL_Color{ 255,255,255,0 }) };
	SDL_Texture *playHover{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Texture *playAux{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

#pragma endregion

#pragma region Sound Button

	//Sound Off Button (Menu)
	tmpSurf= { TTF_RenderText_Blended(font, "Sound Off", SDL_Color{ 255,128,0,0 }) };
	SDL_Texture *soundOffTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Rect soundButtonRect{ (SCREEN_WIDTH - tmpSurf->w) / 2, 700, tmpSurf->w, tmpSurf->h };
	tmpSurf = { TTF_RenderText_Blended(font, "Sound Off", SDL_Color{ 255,255,255,0 }) };
	SDL_Texture *soundOffHover{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Texture *soundOffAux{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

#pragma endregion

#pragma region Sound Button

	//Sound On Button (Menu)
	tmpSurf = { TTF_RenderText_Blended(font, "Sound On", SDL_Color{ 255,128,0,0 }) };
	SDL_Texture *soundOnTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	tmpSurf = { TTF_RenderText_Blended(font, "Sound On", SDL_Color{ 255,255,255,0 }) };
	SDL_Texture *soundOnHover{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

#pragma endregion

#pragma region Exit Button

	//Exit Button (Menu)
	tmpSurf = { TTF_RenderText_Blended(font, "Exit", SDL_Color{ 255,128,0,0 }) };
	SDL_Texture *exitTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Rect exitButtonRect{ (SCREEN_WIDTH - tmpSurf->w) / 2, 900, tmpSurf->w, tmpSurf->h };
	tmpSurf = { TTF_RenderText_Blended(font, "Exit", SDL_Color{ 255,255,255,0 }) };
	SDL_Texture *exitHover{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Texture *exitAux{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

#pragma endregion

//-->Animated Sprite ---

#pragma region Audio

	//Title Music
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	Mix_Music *music = Mix_LoadMUS("../../res/au/mainTheme.mp3");
	if (music == NULL) return false;
	Mix_PlayMusic(music, 100);

#pragma endregion 

// --- GAME LOOP ---
	SDL_Event event;
	gameStates state = MENU;
	bool isRunning = true;
	bool mouseClicked = false;
	bool playMenuMusic = false;
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
				playerTarget.x = event.motion.x - 150;
				playerTarget.y = event.motion.y - 90;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button = SDL_BUTTON_LEFT)
					mouseClicked = true;
				break;
			default:;
			}
		}

// --- UPDATE ---

	//Linear interpolation to make the cursor movement more smooth
	playerRect.x += (playerTarget.x - playerRect.x) / 5;
	playerRect.y += (playerTarget.y - playerRect.y) / 5;

#pragma region Menu: Button Colliders

		//Changing Play Button Texture
		if (Collision(Vec2{ event.motion.x, event.motion.y }, playButtonRect)) {
			playAux = playHover;
			if (mouseClicked)
			{
				mouseClicked = false;
				state = INGAME;
			}
		}
		else 
			playAux = playTexture;

		//Changing Sound Off Texture
		if (Collision(Vec2{ event.motion.x, event.motion.y }, soundButtonRect))
		{
			if (mouseClicked)
			{
				mouseClicked = false;
				playMenuMusic = !playMenuMusic;
				if(playMenuMusic) Mix_HaltMusic();
				else Mix_PlayMusic(music, 100);

			}
			if(playMenuMusic) soundOffAux = soundOnHover;
			else soundOffAux = soundOffHover;
		}
		else 
		{
			if(playMenuMusic) soundOffAux = soundOnTexture;
			else soundOffAux = soundOffTexture;
		}
		
		//Changing Exit Button Texture
		if (Collision(Vec2{ event.motion.x, event.motion.y }, exitButtonRect)) {
	        exitAux = exitHover;
			if(mouseClicked) isRunning = false;
		}
		else exitAux = exitTexture;

#pragma endregion

// --- DRAW ---
	switch (state) 
	{
		case MENU:
			SDL_RenderClear(m_renderer);
			//Background
			SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect);
			//Player
			SDL_RenderCopy(m_renderer, playerTexture, nullptr, &playerRect);
			//Text
			SDL_RenderCopy(m_renderer, textTexture, nullptr, &textRect);
			//Play Button
			SDL_RenderCopy(m_renderer, playAux, nullptr, &playButtonRect);
			//Sound Botton
			SDL_RenderCopy(m_renderer, soundOffAux, nullptr, &soundButtonRect);
			//Exit
			SDL_RenderCopy(m_renderer, exitAux, nullptr, &exitButtonRect);
			break;
		case INGAME:
			SDL_RenderCopy(m_renderer, gameBgTexture, nullptr, &gameBgRect);
			break;
		default:
			break;
	}
		//Hide Mouse 
		SDL_ShowCursor(SDL_DISABLE);
		//Update the screen
		SDL_RenderPresent(m_renderer);
}

// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(soundOffTexture);
	SDL_DestroyTexture(soundOnTexture);
	SDL_DestroyTexture(exitTexture);
	SDL_FreeSurface(tmpSurf);
	TTF_CloseFont(font);
	IMG_Quit();
	TTF_Quit();
	//Mix_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

// --- QUIT ---
	SDL_Quit();

	return 0;
}
