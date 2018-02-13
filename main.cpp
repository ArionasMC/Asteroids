#include "SDL.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
using namespace std;

bool fileExists(const std::string& file);

Game *game = NULL;
const char* version = "v0.3";
const char* name = "Asteroids ";

int main(int argc, char *argv[])
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    string title = string(name)+string(version);

	game = new Game();
	game->init(title.c_str(), 600, 800, false);

	// Creating hiscore file
	if(!fileExists("saves/hiscore.score")) {
        ofstream file("saves/hiscore.score");
        file << 0;
        file.close();
	}

	while (game->running())
	{
	    frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if(frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
		}


	}

	game->clean();
	return 0;
}

bool fileExists(const std::string& file) {
    struct stat buf;
    return (stat(file.c_str(), &buf) == 0);
}
