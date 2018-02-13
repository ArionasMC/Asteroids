#include "Bullet.h"
#include "Player.h"
#include <iostream>

Bullet::Bullet(const char* textureSheet, SDL_Renderer* ren, int x, int y, bool s) : GameObject(textureSheet, ren, x, y)
{
    this->x = x;
    this->y = y;
    this->show = s;
    this->setSpeedY(-10);
    std::cout << "Bullet created! \n";
}


void Bullet::update() {
    GameObject::update();
    if(this->show) {
        this->destR.x = x;
        this->destR.y = y;
        this->destR.w = 10;
        this->destR.h = 40;
    }
}
