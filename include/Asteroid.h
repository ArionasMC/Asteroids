#ifndef ASTEROID_H
#define ASTEROID_H

#include <GameObject.h>


class Asteroid : public GameObject
{
    public:
        Asteroid(const char* textureSheet, SDL_Renderer* ren, double x, double y);
        void update();
};

#endif // ASTEROID_H
