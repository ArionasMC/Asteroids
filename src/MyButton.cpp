#include "MyButton.h"

MyButton::MyButton(const char* textureSheet, SDL_Renderer* ren, int x, int y, int w, int h) : GameObject(textureSheet, ren, x, y)
{
   this->x = x;
   this->y = y;
   this->width = w;
   this->height = h;
   this->show = true;
   this->simpleTexture = true;
}

bool MyButton::isTriggered(int mouseX, int mouseY) {

    if(mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
        pushedState = true;
        return true;
    }
    pushedState = false;
    return false;

}

void MyButton::update(const char* normal, const char* pushed) {
    GameObject::update();
    if(this->show) {

        if(pushedState) {
            if(!changed) {
                setTexture(pushed);
                x = x + 1;
                y = y + 1;
                changed = true;
            }
        } else {
            if(changed) {
                setTexture(normal);
                x = x - 1;
                y = y - 1;
                changed = false;
            }
        }

    }
}
