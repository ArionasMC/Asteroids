#include "Player.h"
#include "TextureManager.h"
#include "Bullet.h"
#include <iostream>
#include <vector>

Player::Player(const char* textureSheet, SDL_Renderer* ren, int x, int y) : GameObject(textureSheet, ren, x, y)
{
    this->show = true;
}



void Player::setHearts(int h) {
    this->hearts = h;
}

int Player::getHearts() {
    return this->hearts;
}

void Player::update() {
    GameObject::update();
    if(this->show) {
        this->destR.x = this->x;
        this->destR.y = this->y;
        this->destR.w = 32;
        this->destR.h = 16;
        if((this->x + this->destR.w) >= 600) {
            this->x = 1;
        }
        if(this->x <= 0) {
            this->x = 600-(this->x + this->destR.w);
        }
    }
}

void Player::keyMove(const Uint8* keyState, SDL_Renderer* ren) {
    if(keyState[SDL_SCANCODE_RIGHT]) {
        this->x += 5;
    } else if(keyState[SDL_SCANCODE_LEFT]) {
        this->x -= 5;
    }
}

bool Player::sdlEvent(SDL_Event e) {
    if(e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) {
        case SDLK_SPACE:
            if(canShoot) {
                return true;
            }

            break;
        default:
            break;

        }
    }
    return false;
}

int Player::getBulletsRemaining() {
    return bulletsRemaining;
}

void Player::setBulletsRemaining(int bullets) {
    bulletsRemaining = bullets;
}

void Player::decreaseBulletsRemaining() {
    if(bulletsRemaining > 0) {
        bulletsRemaining--;
    }
}

void Player::setCanShoot(bool shoot) {
    canShoot = shoot;
}

