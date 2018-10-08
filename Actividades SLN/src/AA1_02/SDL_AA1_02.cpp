#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <exception>
#include <iostream>
#include <string>
#include "Collisions.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define FPS  60
#define DELAY_TIME 1000.0f / FPS
#define MAX_TIME 60000

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
	if (!(Mix_Init(soundFlags) & soundFlags)) throw "Error: SDL_image init";

	//-->Time
	Uint32 frameStart, frameTime;
	float sec = 0;

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
	SDL_Texture *cursorTexture{ IMG_LoadTexture(m_renderer, "../../res/img/kintoun.png") };
	if (cursorTexture == nullptr) throw "Error: platerTexture init";
	SDL_Rect cursorRect{ 0, 0, 350, 190 };
	SDL_Rect cursorTarget{ 0, 0, 100, 100 };
#pragma endregion

#pragma region Title Text

	//Title Text
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 80) };
	if (font == nullptr) throw "No es pot inicialitzar SDL_ttf";
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "Coins Rampage", SDL_Color{255,128,0,0}) };
	if (tmpSurf == nullptr) throw "No es pot crear SDL surface";
	SDL_Texture *titleTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Rect titleRect{ (SCREEN_WIDTH - tmpSurf->w) / 2, 50, tmpSurf->w, tmpSurf->h };

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

#pragma region Score Boards
	//Player 1 puntuation
	tmpSurf = { TTF_RenderText_Blended(font, "PlayerOne: ", SDL_Color{ 0,0,0}) };
	SDL_Texture *player1ScoreTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Rect player1ScoreRect{ 50, 50, tmpSurf->w, tmpSurf->h };

	//Player 1 puntuation
	tmpSurf = { TTF_RenderText_Blended(font, "PlayerTwo: ", SDL_Color{ 0,0,0}) };
	SDL_Texture *player2ScoreTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	SDL_Rect player2ScoreRect{ 50, 110, tmpSurf->w, tmpSurf->h };
#pragma endregion

#pragma region Animated Sprites
//-->Animated Sprite ---
	//Player1
	SDL_Texture *playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/spCastle.png") };
	int textWidth, textHeight, frameWidth1, frameHeight1;
	SDL_QueryTexture(playerTexture, NULL, NULL, &textWidth, &textHeight);
	SDL_Rect player1Rect, player1Position;
	frameWidth1 = textWidth / 12;
	frameHeight1 = textHeight / 8;
	player1Position.x = 800;
	player1Position.y = 800;
	player1Rect.x = frameWidth1*3;
	player1Rect.y = 0;
	player1Position.w = player1Rect.w = frameWidth1;
	player1Position.h = player1Rect.h = frameHeight1;

	//Player2
	SDL_Rect player2Rect, player2Position;
	int frameWidth2, frameHeight2;
	frameWidth2 = textWidth / 12;
	frameHeight2 = textHeight / 8;
	player2Position.x = 1100;
	player2Position.y = 800;
	player2Rect.x = player2Rect.y = 0;
	player2Position.w = player2Rect.w = frameWidth2;
	player2Position.h = player2Rect.h = frameHeight2;

	int frameTimeSprite1 = 0;
	int frameTimeSprite2 = 0;


#pragma endregion

#pragma region Audio

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		throw "Unable to open SDL_Mixer sound systems";
	}

	//Title Music
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	Mix_Music *menuMusic = Mix_LoadMUS("../../res/au/mainThemeCatala.mp3");
	if (menuMusic == NULL) return false;
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	Mix_PlayMusic(menuMusic, -1);

#pragma endregion 

// --- GAME LOOP ---
	SDL_Event event;
	gameStates state = MENU;
	Vec2 mouseAxis;
	Player player1;
	Player player2;
	char exactTime[5];
	bool player1OnMove = false;
	bool player2OnMove = false;
	bool isRunning = true;
	bool mouseClicked = false;
	bool playMenuMusic = false;

	while (isRunning) {
		frameStart = SDL_GetTicks();
		// HANDLE EVENTS
		while (SDL_PollEvent(&event))
		{
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;

				if (event.key.keysym.sym == SDLK_UP) player1.goUp = true;
				if (event.key.keysym.sym == SDLK_DOWN) player1.goDown = true;
				if (event.key.keysym.sym == SDLK_RIGHT) player1.goRight = true;
				if (event.key.keysym.sym == SDLK_LEFT) player1.goLeft = true;
				if (player1.goUp || player1.goDown || player1.goRight || player1.goLeft) player1OnMove = true;

				if (event.key.keysym.sym == SDLK_w) player2.goUp = true;
				if (event.key.keysym.sym == SDLK_s) player2.goDown = true;
				if (event.key.keysym.sym == SDLK_d) player2.goRight = true;
				if (event.key.keysym.sym == SDLK_a) player2.goLeft = true;
				if (player2.goUp || player2.goDown || player2.goRight || player2.goLeft) player2OnMove = true;

				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_UP) player1.goUp = false;
				if (event.key.keysym.sym == SDLK_DOWN) player1.goDown = false;
				if (event.key.keysym.sym == SDLK_RIGHT) player1.goRight = false;
				if (event.key.keysym.sym == SDLK_LEFT) player1.goLeft = false;
				if (!player1.goUp || !player1.goDown || !player1.goRight || !player1.goLeft) player1OnMove = false;

				if (event.key.keysym.sym == SDLK_w) player2.goUp = false;
				if (event.key.keysym.sym == SDLK_s) player2.goDown = false;
				if (event.key.keysym.sym == SDLK_d) player2.goRight = false;
				if (event.key.keysym.sym == SDLK_a) player2.goLeft = false;
				if (!player2.goUp || !player2.goDown || !player2.goRight || !player2.goLeft) player2OnMove = false;
				break;
			case SDL_MOUSEMOTION:
				mouseAxis.x = event.motion.x;
				mouseAxis.y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button = SDL_BUTTON_LEFT)
					mouseClicked = true;
				break;
			default:;
			}
		}

		// --- UPDATE --
			//Hide Mouse 
		SDL_ShowCursor(SDL_DISABLE);

#pragma region Menu: Button Colliders

		//Changing Play Button Texture
		if (Collision(mouseAxis, Rect(playButtonRect)))
		{
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
		if (Collision(mouseAxis, Rect(soundButtonRect)))
		{
			if (mouseClicked)
			{
				mouseClicked = false;
				playMenuMusic = !playMenuMusic;
				if (playMenuMusic) Mix_PauseMusic();
				else Mix_PlayMusic(menuMusic, 100);

			}
			if (playMenuMusic) soundOffAux = soundOnHover;
			else soundOffAux = soundOffHover;
		}
		else
		{
			if (playMenuMusic) soundOffAux = soundOnTexture;
			else soundOffAux = soundOffTexture;
		}

		//Changing Exit Button Texture
		if (Collision(mouseAxis, Rect(exitButtonRect))) {
			exitAux = exitHover;
			if (mouseClicked) isRunning = false;
		}
		else exitAux = exitTexture;

		//Time 
		sec += DELAY_TIME;
		if (sec >= MAX_TIME) state = MENU;

		float timeLeft = (MAX_TIME - sec) / 1000;
		int size = sizeof(exactTime);
		SDL_snprintf(exactTime, size, "%f", timeLeft);

		for (int i = 0; i < size; i++) {
			if (exactTime[i] == '.') exactTime[i] == ':';
		}

		TTF_Font *timeFont{ TTF_OpenFont("../../res/ttf/SuperMario256.ttf", 80) };
		if (font == nullptr) throw "No es pot inicialitzar SDL_ttf";
		SDL_Surface *tmpSurf{ TTF_RenderText_Blended(timeFont, exactTime, SDL_Color{255,0,0,0}) };
		if (tmpSurf == nullptr) throw "No es pot crear SDL surface";
		SDL_Texture *timeTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_Rect timeRect{ 1700, 70, tmpSurf->w, tmpSurf->h };


#pragma endregion

		//Putting the Mouse at the center of the cursor 
		cursorTarget.x = mouseAxis.x - 150;
		cursorTarget.y = mouseAxis.y - 90;
		//Linear interpolation to make the cursor movement more smooth
		cursorRect.x += (cursorTarget.x - cursorRect.x) / 5;
		cursorRect.y += (cursorTarget.y - cursorRect.y) / 5;

		//Player Movement
		if (state == INGAME) {
			//Player 1 
			if (player1.goUp) {
				player1Position.x += 0 * speedMovement;
				player1Position.y += -1 * speedMovement;
				player1Rect.y = frameHeight1 * 3;
			}
			if (player1.goDown) {
				player1Position.x += 0 * speedMovement;
				player1Position.y += 1 * speedMovement;
				player1Rect.y = frameHeight1 * 0;
			}
			if (player1.goRight) {
				player1Position.x += 1 * speedMovement;
				player1Position.y += 0 * speedMovement;
				player1Rect.y = frameHeight1 * 2;
			}
			if (player1.goLeft) {
				player1Position.x += -1 * speedMovement;
				player1Position.y += 0 * speedMovement;
				player1Rect.y = frameHeight1 * 1;
			}

			//Player 2 
			if (player2.goUp) {
				player2Position.x += 0 * speedMovement;
				player2Position.y += -1 * speedMovement;
				player2Rect.y = frameHeight2 * 3;
			}
			if (player2.goDown) {
				player2Position.x += 0 * speedMovement;
				player2Position.y += 1 * speedMovement;
				player2Rect.y = frameHeight2 * 0;
			}
			if (player2.goRight) {
				player2Position.x += 1 * speedMovement;
				player2Position.y += 0 * speedMovement;
				player2Rect.y = frameHeight2 * 2;
			}
			if (player2.goLeft) {
				player2Position.x += -1 * speedMovement;
				player2Position.y += 0 * speedMovement;
				player2Rect.y = frameHeight2 * 1;
			}
		}

		//Player Sprite Movement
	    frameTimeSprite1++;
		if (FPS / frameTimeSprite1 <= 9 && player1OnMove)
		{
			frameTimeSprite1 = 0;
		    player1Rect.x += frameWidth1;
			if (player1Rect.x >= (frameWidth1*6)) player1Rect.x = frameWidth1*3;
		}

		frameTimeSprite2++;
		if (FPS / frameTimeSprite2 <= 9 && player2OnMove)
		{
			frameTimeSprite2 = 0;
			player2Rect.x += frameWidth2;
			if (player2Rect.x >= (frameWidth2*3)) player2Rect.x = 0;
		}

		// --- DRAW ---
		SDL_RenderClear(m_renderer);

#pragma region State Machine

		switch (state)
		{
		case MENU:
			//Background
			SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect);
			//Cursor
			SDL_RenderCopy(m_renderer, cursorTexture, nullptr, &cursorRect);
			//Title
			SDL_RenderCopy(m_renderer, titleTexture, nullptr, &titleRect);
			//Play Button
			SDL_RenderCopy(m_renderer, playAux, nullptr, &playButtonRect);
			//Sound Botton
			SDL_RenderCopy(m_renderer, soundOffAux, nullptr, &soundButtonRect);
			//Exit
			SDL_RenderCopy(m_renderer, exitAux, nullptr, &exitButtonRect);
			break;
		case INGAME:
			//Background
			SDL_RenderCopy(m_renderer, gameBgTexture, nullptr, &gameBgRect);
			//Animated Player1 Sprite
			SDL_RenderCopy(m_renderer, playerTexture, &player1Rect, &player1Position);
			//Animated Player2 Sprite
			SDL_RenderCopy(m_renderer, playerTexture, &player2Rect, &player2Position);
			//Time
			SDL_RenderCopy(m_renderer, timeTexture, nullptr, &timeRect);
			//Score Board
			SDL_RenderCopy(m_renderer, player1ScoreTexture, nullptr, &player1ScoreRect);
			SDL_RenderCopy(m_renderer, player2ScoreTexture, nullptr, &player2ScoreRect);
			break;
		default:
			break;
		}

#pragma endregion 

		//Update the screen
		SDL_RenderPresent(m_renderer);

		//Frame Control
		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < DELAY_TIME)
			SDL_Delay((int)(DELAY_TIME - frameTime));
	}

#pragma region Close SDL

		// --- DESTROY ---
		SDL_DestroyTexture(bgTexture);
		SDL_DestroyTexture(cursorTexture);
		SDL_DestroyTexture(titleTexture);
		SDL_DestroyTexture(soundOffTexture);
		SDL_DestroyTexture(soundOnTexture);
		SDL_DestroyTexture(exitTexture);
		SDL_FreeSurface(tmpSurf);
		TTF_CloseFont(font);
		Mix_CloseAudio();
		IMG_Quit();
		TTF_Quit();
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		Mix_Quit();

#pragma endregion 

	return 0;
}
