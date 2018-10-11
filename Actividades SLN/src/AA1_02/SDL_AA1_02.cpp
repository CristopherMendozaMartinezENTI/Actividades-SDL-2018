#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <exception>
#include <iostream>
#include <string>
#include "Collisions.h"

int main(int, char*[])
{
// --- Init ---
#pragma region InitSDL

	try {
		// --- INIT SDL ---
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			throw std::exception("No es pot inicialitzar SDL subsystems");

		// --- WINDOW ---
		SDL_Window *m_window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
		if (m_window == nullptr)
			throw std::exception("No es pot inicialitzar SDL_Window");

		// --- RENDERER ---
		SDL_Renderer *m_renderer{ SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
		if (m_renderer == nullptr)
			throw std::exception("No es pot inicialitzar SDL_Renderer");

		//-->SDL_Image 
		const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
		if (!(IMG_Init(imgFlags) & imgFlags)) throw std::exception("Error: SDL_image init");

		//-->SDL_TTF
		if (TTF_Init() != 0) throw std::exception("No es pot inicialitzar SDL_ttf");

		//-->SDL_Mix
		const Uint8 soundFlags{ MIX_INIT_MP3 };
		if (!(Mix_Init(soundFlags) & soundFlags)) throw std::exception("Error: SDL_image init");



		//-->Time
		Uint32 frameStart, frameTime;
		float sec = 0;

#pragma endregion 	

		// --- SPRITES ---
#pragma region Backgrounds

	//Main Menu Background
		SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg") };
		if (bgTexture == nullptr) throw std::exception("Error: bgTexture init");
		SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

		//In game Backgroung
		SDL_Texture* gameBgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bgCastle.jpg") };
		if (bgTexture == nullptr) throw std::exception("Error: gameBgTexture init");
		SDL_Rect gameBgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

#pragma endregion

#pragma region Cursor

		//Cursor
		SDL_Texture *cursorTexture{ IMG_LoadTexture(m_renderer, "../../res/img/kintoun.png") };
		if (cursorTexture == nullptr) throw std::exception("Error: platerTexture init");
		SDL_Rect cursorRect{ 0, 0, 350, 190 };
		SDL_Rect cursorTarget{ 0, 0, 100, 100 };

#pragma endregion

#pragma region Title Text

		//Title Text
		TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 200) };
		if (font == nullptr) throw std::exception("No es pot inicialitzar SDL_ttf");
		SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "Coins Rampage", SDL_Color{255,210,10,0}) };
		if (tmpSurf == nullptr) throw std::exception("No es pot crear SDL surface");
		SDL_Texture *titleTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_Rect titleRect{ (SCREEN_WIDTH - tmpSurf->w) / 2, 200, tmpSurf->w, tmpSurf->h };

#pragma endregion

#pragma region Play Button 

		//Play Button (Menu)
		tmpSurf = { TTF_RenderText_Blended(font, "Play", SDL_Color{ 255,128,0,0 }) };
		SDL_Texture *playTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_Rect playButtonRect{ (SCREEN_WIDTH - tmpSurf->w) / 2, 400, tmpSurf->w, tmpSurf->h };
		tmpSurf = { TTF_RenderText_Blended(font, "Play", SDL_Color{ 255,0,0,0 }) };
		SDL_Texture *playHover{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_Texture *playAux{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

#pragma endregion

#pragma region Sound Button

		//Sound Off Button (Menu)
		tmpSurf = { TTF_RenderText_Blended(font, "Sound Off", SDL_Color{ 255,128,0,0 }) };
		SDL_Texture *soundOffTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_Rect soundButtonRect{ (SCREEN_WIDTH - tmpSurf->w) / 2, 600, tmpSurf->w, tmpSurf->h };
		tmpSurf = { TTF_RenderText_Blended(font, "Sound Off", SDL_Color{ 255,0,0,0 }) };
		SDL_Texture *soundOffHover{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_Texture *soundOffAux{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

#pragma endregion

#pragma region Sound Button

		//Sound On Button (Menu)
		tmpSurf = { TTF_RenderText_Blended(font, "Sound On", SDL_Color{ 255,128,0,0 }) };
		SDL_Texture *soundOnTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		tmpSurf = { TTF_RenderText_Blended(font, "Sound On", SDL_Color{ 255,0,0,0 }) };
		SDL_Texture *soundOnHover{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

#pragma endregion

#pragma region Exit Button

		//Exit Button (Menu)
		tmpSurf = { TTF_RenderText_Blended(font, "Exit", SDL_Color{ 255,128,0,0 }) };
		SDL_Texture *exitTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_Rect exitButtonRect{ (SCREEN_WIDTH - tmpSurf->w) / 2, 800, tmpSurf->w, tmpSurf->h };
		tmpSurf = { TTF_RenderText_Blended(font, "Exit", SDL_Color{ 255,0,0,0 }) };
		SDL_Texture *exitHover{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_Texture *exitAux{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

#pragma endregion

#pragma region Score Boards
		//Player 1 puntuation
		TTF_Font *inGameFont{ TTF_OpenFont("../../res/ttf/Arial.ttf", 80) };
		if (inGameFont == nullptr) throw std::exception("No es pot inicialitzar SDL_ttf");
		tmpSurf = { TTF_RenderText_Blended(inGameFont, "PlayerOne: ", SDL_Color{ 0,0,0}) };
		SDL_Texture *player1ScoreTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_Rect player1ScoreRect{ 20, 30, tmpSurf->w, tmpSurf->h };

		//Player 1 puntuation
		tmpSurf = { TTF_RenderText_Blended(inGameFont, "PlayerTwo: ", SDL_Color{ 0,0,0}) };
		SDL_Texture *player2ScoreTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_Rect player2ScoreRect{ 20, 120, tmpSurf->w, tmpSurf->h };
#pragma endregion

#pragma region Coins

		SDL_Texture *coinTexture{ IMG_LoadTexture(m_renderer, "../../res/img/gold.png") };
		if (coinTexture == nullptr) throw std::exception("Error: coinTexture init");
		SDL_Rect coinRect[AMOUNT_OF_COINS];
		for (int i = 0; i < AMOUNT_OF_COINS; i++)
			coinRect[i] = SDL_Rect{ (rand() % SCREEN_WIDTH) - 50, (rand() % 700) + 300, 100,100 };


#pragma endregion

#pragma region Animated Sprites

		//-->Animated Sprite ---
			//Player1
		SDL_Texture *playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/spCastle.png") };
		if (playerTexture == nullptr) throw std::exception("Error: playerTexture init");
		int textWidth, textHeight, frameWidth1, frameHeight1;
		SDL_QueryTexture(playerTexture, NULL, NULL, &textWidth, &textHeight);
		SDL_Rect player1Rect, player1Position;
		frameWidth1 = textWidth / 12;
		frameHeight1 = textHeight / 8;
		player1Position.x = 800;
		player1Position.y = 800;
		player1Rect.x = frameWidth1 * 3;
		player1Rect.y = 0;
		player1Position.w = player1Rect.w = frameWidth1;
		player1Position.h = player1Rect.h = frameHeight1;
		player1Position.h = frameHeight1 * PLAYER_SIZE;
		player1Position.w = frameWidth1 * PLAYER_SIZE;


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
		player2Position.h = frameHeight2 * PLAYER_SIZE;
		player2Position.w = frameWidth2 * PLAYER_SIZE;

		int frameTimeSprite1 = 0;
		int frameTimeSprite2 = 0;

		//Puntuación Player 1
		SDL_Texture *scoreTexture{ IMG_LoadTexture(m_renderer, "../../res/img/num.png") };
		if (scoreTexture == nullptr) throw std::exception("Error: scoreTexture init");
		int scoreWidth, scoreHeight, frameWidthScore1Right, frameHeightScore1Right;
		SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreWidth, &scoreHeight);
		//Right
		SDL_Rect scoreRectPlayer1Right, scorePositionPlayer1Right;
		frameWidthScore1Right = scoreWidth / 10;
		frameHeightScore1Right = scoreHeight / 1;
		scorePositionPlayer1Right.x = 650;
		scorePositionPlayer1Right.y = 40;
		scoreRectPlayer1Right.x = scoreRectPlayer1Right.y = 0;
		scorePositionPlayer1Right.w = scoreRectPlayer1Right.w = frameWidthScore1Right;
		scorePositionPlayer1Right.h = scoreRectPlayer1Right.h = frameHeightScore1Right;
		int frameTimeScore1Right = 0;
		//Center
		SDL_Rect scoreRectPlayer1Center, scorePositionPlayer1Center;
		int frameWidthScore1Center, frameHeightScore1Center;
		frameWidthScore1Center = scoreWidth / 10;
		frameHeightScore1Center = scoreHeight / 1;
		scorePositionPlayer1Center.x = 550;
		scorePositionPlayer1Center.y = 40;
		scoreRectPlayer1Center.x = scoreRectPlayer1Center.y = 0;
		scorePositionPlayer1Center.w = scoreRectPlayer1Center.w = frameWidthScore1Center;
		scorePositionPlayer1Center.h = scoreRectPlayer1Center.h = frameHeightScore1Center;
		int frameTimeScore1Center = 0;
		//Left
		SDL_Rect scoreRectPlayer1Left, scorePositionPlayer1Left;
		int frameWidthScore1Left, frameHeightScore1Left;
		frameWidthScore1Left = scoreWidth / 10;
		frameHeightScore1Left = scoreHeight / 1;
		scorePositionPlayer1Left.x = 450;
		scorePositionPlayer1Left.y = 40;
		scoreRectPlayer1Left.x = scoreRectPlayer1Left.y = 0;
		scorePositionPlayer1Left.w = scoreRectPlayer1Left.w = frameWidthScore1Left;
		scorePositionPlayer1Left.h = scoreRectPlayer1Left.h = frameHeightScore1Left;
		int frameTimeScore1Left = 0;

		//Puntuación Player 2
			//Right
		SDL_Rect scoreRectPlayer2Right, scorePositionPlayer2Right;
		int frameWidthScore2Right, frameHeightScore2Right;
		frameWidthScore2Right = scoreWidth / 10;
		frameHeightScore2Right = scoreHeight / 1;
		scorePositionPlayer2Right.x = 650;
		scorePositionPlayer2Right.y = 130;
		scoreRectPlayer2Right.x = scoreRectPlayer2Right.y = 0;
		scorePositionPlayer2Right.w = scoreRectPlayer2Right.w = frameWidthScore2Right;
		scorePositionPlayer2Right.h = scoreRectPlayer2Right.h = frameHeightScore2Right;
		int frameTimeScore2Right = 0;
		//Center
		SDL_Rect scoreRectPlayer2Center, scorePositionPlayer2Center;
		int frameWidthScore2Center, frameHeightScore2Center;
		frameWidthScore2Center = scoreWidth / 10;
		frameHeightScore2Center = scoreHeight / 1;
		scorePositionPlayer2Center.x = 550;
		scorePositionPlayer2Center.y = 130;
		scoreRectPlayer2Center.x = scoreRectPlayer2Center.y = 0;
		scorePositionPlayer2Center.w = scoreRectPlayer2Center.w = frameWidthScore2Center;
		scorePositionPlayer2Center.h = scoreRectPlayer2Center.h = frameHeightScore2Center;
		int frameTimeScore2Center = 0;
		//Left
		SDL_Rect scoreRectPlayer2Left, scorePositionPlayer2Left;
		int frameWidthScore2Left, frameHeightScore2Left;
		frameWidthScore2Left = scoreWidth / 10;
		frameHeightScore2Left = scoreHeight / 1;
		scorePositionPlayer2Left.x = 450;
		scorePositionPlayer2Left.y = 130;
		scoreRectPlayer2Left.x = scoreRectPlayer2Left.y = 0;
		scorePositionPlayer2Left.w = scoreRectPlayer2Left.w = frameWidthScore2Left;
		scorePositionPlayer2Left.h = scoreRectPlayer2Left.h = frameHeightScore2Left;
		int frameTimeScore2Left = 0;

#pragma endregion

#pragma region Audio

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
			throw std::exception("Unable to open SDL_Mixer sound systems");
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
					if (event.key.keysym.sym == SDLK_w) player2.goUp = true;
					if (event.key.keysym.sym == SDLK_s) player2.goDown = true;
					if (event.key.keysym.sym == SDLK_d) player2.goRight = true;
					if (event.key.keysym.sym == SDLK_a) player2.goLeft = true;
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_UP) player1.goUp = false;
					if (event.key.keysym.sym == SDLK_DOWN) player1.goDown = false;
					if (event.key.keysym.sym == SDLK_RIGHT) player1.goRight = false;
					if (event.key.keysym.sym == SDLK_LEFT) player1.goLeft = false;
					if (event.key.keysym.sym == SDLK_w) player2.goUp = false;
					if (event.key.keysym.sym == SDLK_s) player2.goDown = false;
					if (event.key.keysym.sym == SDLK_d) player2.goRight = false;
					if (event.key.keysym.sym == SDLK_a) player2.goLeft = false;
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

			//Putting the Mouse at the center of the cursor 
			cursorTarget.x = mouseAxis.x - 150;
			cursorTarget.y = mouseAxis.y - 90;
			//Linear interpolation to make the cursor movement more smooth
			cursorRect.x += (cursorTarget.x - cursorRect.x) / 5;
			cursorRect.y += (cursorTarget.y - cursorRect.y) / 5;

#pragma region Menu: Button Colliders

			//Changing Play Button Texture
			if (Collision(mouseAxis, playButtonRect))
			{
				playAux = playHover;
				if (mouseClicked)
				{
					mouseClicked = false;
					state = IN_GAME;
					//Restart player positions and scores
					sec = 0;
					player1Position.x = 800;
					player1Position.y = 800;
					player1Rect.x = frameWidth1 * 3;
					player1Rect.y = 0;
					player2Position.x = 1100;
					player2Position.y = 800;
					player2Rect.x = player2Rect.y = 0;
					scoreRectPlayer1Right.x = 0;
					scoreRectPlayer1Center.x = 0;
					scoreRectPlayer1Left.x = 0;
					scoreRectPlayer2Right.x = 0;
					scoreRectPlayer2Left.x = 0;
					scoreRectPlayer2Left.x = 0;
				}
			}
			else
				playAux = playTexture;

			//Changing Sound Off Texture
			if (Collision(mouseAxis, soundButtonRect))
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
			if (Collision(mouseAxis, exitButtonRect))
			{
				exitAux = exitHover;
				if (mouseClicked) isRunning = false;
			}
			else exitAux = exitTexture;

#pragma endregion

#pragma region Players Animations, Movement and Score

			if (player1.goUp || player1.goDown || player1.goRight || player1.goLeft) player1OnMove = true;
			if (player2.goUp || player2.goDown || player2.goRight || player2.goLeft) player2OnMove = true;
			if (!player1.goUp && !player1.goDown && !player1.goRight && !player1.goLeft) player1OnMove = false;
			if (!player2.goUp && !player2.goDown && !player2.goRight && !player2.goLeft) player2OnMove = false;

			//Movement
			if (state == IN_GAME) {
				//Player 1 
				if (player1OnMove) {
					if (player1.goUp && player1Position.y > 300) {
						player1Position.x += 0 * MOTION_SPEED;
						player1Position.y += -1 * MOTION_SPEED;
						player1Rect.y = frameHeight1 * 3;
					}
					if (player1.goDown && player1Position.y < SCREEN_HEIGHT - 70) {
						player1Position.x += 0 * MOTION_SPEED;
						player1Position.y += 1 * MOTION_SPEED;
						player1Rect.y = frameHeight1 * 0;
					}
					if (player1.goRight && player1Position.x < SCREEN_WIDTH - 60) {
						player1Position.x += 1 * MOTION_SPEED;
						player1Position.y += 0 * MOTION_SPEED;
						player1Rect.y = frameHeight1 * 2;
					}
					if (player1.goLeft && player1Position.x > 0) {
						player1Position.x += -1 * MOTION_SPEED;
						player1Position.y += 0 * MOTION_SPEED;
						player1Rect.y = frameHeight1 * 1;
					}

					frameTimeSprite1++;
					if (FPS / frameTimeSprite1 <= 9)
					{
						frameTimeSprite1 = 0;
						player1Rect.x += frameWidth1;
						if (player1Rect.x >= (frameWidth1 * 6)) player1Rect.x = frameWidth1 * 3;
					}
				}

				//Player 2 
				if (player2OnMove) {
					if (player2.goUp && player2Position.y > 300) {
						player2Position.x += 0 * MOTION_SPEED;
						player2Position.y += -1 * MOTION_SPEED;
						player2Rect.y = frameHeight2 * 3;
					}
					if (player2.goDown && player2Position.y < SCREEN_HEIGHT - 70) {
						player2Position.x += 0 * MOTION_SPEED;
						player2Position.y += 1 * MOTION_SPEED;
						player2Rect.y = frameHeight2 * 0;
					}
					if (player2.goRight&& player2Position.x < SCREEN_WIDTH - 60) {
						player2Position.x += 1 * MOTION_SPEED;
						player2Position.y += 0 * MOTION_SPEED;
						player2Rect.y = frameHeight2 * 2;
					}
					if (player2.goLeft && player2Position.x > 0) {
						player2Position.x += -1 * MOTION_SPEED;
						player2Position.y += 0 * MOTION_SPEED;
						player2Rect.y = frameHeight2 * 1;
					}
					frameTimeSprite2++;
					if (FPS / frameTimeSprite2 <= 9)
					{
						frameTimeSprite2 = 0;
						player2Rect.x += frameWidth2;
						if (player2Rect.x >= (frameWidth2 * 3)) player2Rect.x = 0;
					}
				}
			}

			//Ganará siempre el jugador 1 en el caso de que ambos cojan el mismo saco de oro al mismo tiempo
			for (int i = 0; i < AMOUNT_OF_COINS; i++)
			{
				if (rectCollision(player1Position, coinRect[i])) {
					coinRect[i].x = (rand() % SCREEN_WIDTH) - 50;
					coinRect[i].y = (rand() % 700) + 300;
					player1.score++;
					player1.getCoins = true;
				}
				else if (rectCollision(player2Position, coinRect[i])) {
					coinRect[i].x = (rand() % SCREEN_WIDTH) - 50;
					coinRect[i].y = (rand() % 700) + 300;
					player2.score++;
					player2.getCoins = true;
				}
			}

			if (player1.getCoins) {
				frameTimeScore1Right++;
				if (FPS / frameTimeScore1Right <= 9) {
					player1.getCoins = false;
					frameTimeScore1Right = 0;
					scoreRectPlayer1Right.x += frameWidthScore1Right;
					if (scoreRectPlayer1Right.x >= (scoreWidth)) {
						scoreRectPlayer1Right.x = 0;
						scoreRectPlayer1Center.x += frameWidthScore1Center;
						if (scoreRectPlayer1Center.x >= (scoreWidth)) {
							scoreRectPlayer1Center.x = 0;
							scoreRectPlayer1Right.x = 0;
							scoreRectPlayer1Left.x += frameWidthScore1Left;
							if (scoreRectPlayer1Left.x >= (scoreWidth)) scoreRectPlayer1Left.x = 0;
						}
					}
				}
			}

			if (player2.getCoins) {
				frameTimeScore2Right++;
				if (FPS / frameTimeScore2Right <= 9) {
					player2.getCoins = false;
					frameTimeScore2Right = 0;
					scoreRectPlayer2Right.x += frameWidthScore2Right;
					if (scoreRectPlayer2Right.x >= (scoreWidth)) {
						scoreRectPlayer2Right.x = 0;
						scoreRectPlayer2Center.x += frameWidthScore2Center;
						if (scoreRectPlayer2Center.x >= (scoreWidth)) {
							scoreRectPlayer2Center.x = 0;
							scoreRectPlayer2Right.x = 0;
							scoreRectPlayer2Left.x += frameWidthScore2Left;
							if (scoreRectPlayer2Left.x >= (scoreWidth)) scoreRectPlayer2Left.x = 0;
						}
					}
				}
			}

#pragma endregion 

#pragma region Time

			//Time 
			sec += DELAY_TIME;
			if (sec >= MAX_TIME) state = MENU;

			float timeLeft = (MAX_TIME - sec) / 1000;
			int size = sizeof(exactTime);
			SDL_snprintf(exactTime, size, "%f", timeLeft);

			for (int i = 0; i < size; i++) {
				if (exactTime[i] == '.') exactTime[i] == ':';
			}

			tmpSurf = { TTF_RenderText_Blended(inGameFont, exactTime, SDL_Color{ 255,0,0,0 }) };
			if (tmpSurf == nullptr) throw std::exception("No es pot crear SDL surface");
			SDL_Texture *timeTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
			SDL_Rect timeRect{ 1750, 65, tmpSurf->w, tmpSurf->h };

#pragma endregion 

			// --- DRAW ---
			SDL_RenderClear(m_renderer);

#pragma region Game State Machine

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
			case IN_GAME:
				//Background
				SDL_RenderCopy(m_renderer, gameBgTexture, nullptr, &gameBgRect);
				//Animated Player1 Sprite
				SDL_RenderCopy(m_renderer, playerTexture, &player1Rect, &player1Position);
				//Animated Player2 Sprite
				SDL_RenderCopy(m_renderer, playerTexture, &player2Rect, &player2Position);
				//Coins
				for (int i = 0; i < AMOUNT_OF_COINS; i++) {
					SDL_RenderCopy(m_renderer, coinTexture, nullptr, &coinRect[i]);
					//Time
					SDL_RenderCopy(m_renderer, timeTexture, nullptr, &timeRect);
					//Score Board
					SDL_RenderCopy(m_renderer, player1ScoreTexture, nullptr, &player1ScoreRect);
					SDL_RenderCopy(m_renderer, player2ScoreTexture, nullptr, &player2ScoreRect);
					SDL_RenderCopy(m_renderer, scoreTexture, &scoreRectPlayer1Right, &scorePositionPlayer1Right);
					SDL_RenderCopy(m_renderer, scoreTexture, &scoreRectPlayer1Center, &scorePositionPlayer1Center);
					SDL_RenderCopy(m_renderer, scoreTexture, &scoreRectPlayer1Left, &scorePositionPlayer1Left);
					SDL_RenderCopy(m_renderer, scoreTexture, &scoreRectPlayer2Right, &scorePositionPlayer2Right);
					SDL_RenderCopy(m_renderer, scoreTexture, &scoreRectPlayer2Center, &scorePositionPlayer2Center);
					SDL_RenderCopy(m_renderer, scoreTexture, &scoreRectPlayer2Left, &scorePositionPlayer2Left);
				}
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
		SDL_DestroyTexture(playerTexture);
		SDL_DestroyTexture(player1ScoreTexture);
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

	}

	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
 