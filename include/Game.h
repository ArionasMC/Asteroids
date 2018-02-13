#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "MyText.h"

#ifndef GAME_H
#define GAME_H

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();

private:
	bool isRunning = false;
	SDL_Window *window;
	SDL_Renderer *renderer;
};

#endif // GAME_H
