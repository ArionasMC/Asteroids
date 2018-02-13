#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "MyText.h"
#include <iostream>

#ifndef LEVEL_H
#define LEVEL_H

class Level
{
    public:
        Level(int level, SDL_Surface* screen, SDL_Renderer* renderer, int playerHearts, int startScore);
        void setLevel(int level);
        int getLevel();
        void update();
        void render();
        void handleEvents(SDL_Event event);
        bool checkCollision(SDL_Rect asteroids, SDL_Rect bullets);
        void setNextLevel(bool nextLevel);
        bool isNextLevel();
        void drawText(MyText* text);
        SDL_Color make_color(Uint8 r, Uint8 g, Uint8 b);
        int getLastPlayerHearts();
        bool isGameOver();
        int getScore();
        bool isToReturnToMainscreen();


    private:
        int level = 1;
        int score = 0;
        bool nextLevel = false;
        bool returnMainscreen = false;
        SDL_Surface* screen;
        SDL_Renderer* renderer;
};

#endif // LEVEL_H
