#include "Asteroid.h"
#include <iostream>

Asteroid::Asteroid(const char* textureSheet, SDL_Renderer* ren, double x, double y) : GameObject(textureSheet, ren, x, y)
{
    this->show = false;
    this->setSpeedY(0.5);
    this->setRotation(2.0);
    this->destR.w = 32;
    this->destR.h = 32;
    std::cout << "Asteroid created at " << x <<"! \n";
}

void Asteroid::update() {

    GameObject::update();
    if(this->show) {
        this->destR.x = this->x;
        this->destR.y = this->y;
        this->destR.w = 32;
        this->destR.h = 32;
    }

}
