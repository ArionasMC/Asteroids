#ifndef BULLET_H
#define BULLET_H

#include <GameObject.h>
#include <Player.h>


class Bullet : public GameObject
{
    public:
        Bullet(const char* textureSheet, SDL_Renderer* ren, int x, int y, bool s);
        void update();
};

#endif // BULLET_H

