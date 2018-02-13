#ifndef PLAYER_H
#define PLAYER_H

#include <GameObject.h>
#include <Bullet.h>
#include <iostream>
#include <vector>
class Bullet;

class Player : public GameObject
{
    public:
        Player(const char* textureSheet, SDL_Renderer* ren, int x, int y);

        void setHearts(int h);
        int getHearts();
        void update();
        void keyMove(const Uint8* keyState, SDL_Renderer* ren);
        bool sdlEvent(SDL_Event e);
        int getBulletsRemaining();
        void setBulletsRemaining(int bullets);
        void decreaseBulletsRemaining();
        void setCanShoot(bool shoot);

    private:
        int hearts = 3;
        int bulletsRemaining = 5;
        bool canShoot = true;
};

#endif // PLAYER_H

