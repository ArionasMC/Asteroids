#include "Level.h"
#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
#include "Asteroid.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>

using namespace std;

Player* player;

vector<Asteroid*> asteroids;
vector<Bullet*> bullets(5);
vector<GameObject*> hearts;

int freeHeart = 0;
int freeIndex = 0;

bool bulletsMessageShow = false;
int bulletsMessageDelay = 120;
MyText* bulletsMessage;

MyText* gameOverText;
bool gameOver = false;

int aSize;
//int asteroidsToCome;
int asteroidsRemaining;

bool reloading = false;
MyText* reloadMessage;
int reloadDelay = 180;

int spawn = 0;
//int freeSpawn = 0;
MyText* levelText;
MyText* restartText;
MyText* bulletsRemaining;
MyText* scoreText;
MyText* hiscoreText;
MyText* returnText;

TTF_Font* font25;
TTF_Font* font50;

ifstream hiscoreRead;
ofstream hiscoreWrite;

int hiscore;
bool didHiscore = false;

bool openedFile = false;

Level::Level(int level, SDL_Surface* screen, SDL_Renderer* renderer, int playerHearts, int startScore) {
    this->level = level;
    this->screen = screen;
    this->renderer = renderer;
    gameOver = false;
    freeHeart = 0;
    freeIndex = 0;
    spawn = 0;

    cout << "Initializing fonts...\n";
    font25 = TTF_OpenFont("OpenSans-Regular.ttf", 25);
    font50 = TTF_OpenFont("OpenSans-Regular.ttf", 50);

    cout << "Initializing Player... \n";
	player = new Player("assets/images/spaceship.png", renderer, 10, 750);
	player->setHearts(playerHearts);
	freeIndex = 0;

	cout << "Player initialized successfully! \n";

	hearts.reserve(player->getHearts());
	hearts.resize(player->getHearts());
	int hSize = hearts.size();
	cout << "Hearts size: " << hSize << endl;

	for(int j = 0; j < hSize; j++) {
        hearts[j] = new GameObject("assets/images/heart.png", renderer, 5+(j*32), 40);
        hearts[j]->setSimpleTexture(true);
        hearts[j]->setShow(true);
	}
	freeHeart = hSize - 1;

    asteroids.reserve(level*5);
    asteroids.resize(level*5);

    bullets.reserve(5);
    bullets.resize(5);

    aSize = asteroids.size();
    //asteroidsToCome = aSize;
    asteroidsRemaining = aSize;

    srand(time(NULL));
    for(int i = 0; i < aSize; i++) {
        asteroids[i] = new Asteroid("assets/images/asteroid2.png", renderer, 5 + (rand() % 560), 20);
        asteroids[i]->setShow(false);
    }

    cout << "AsteroidVector size: " << asteroids.size() << endl;
    cout << "BulletVector size: " << bullets.size() << endl;

    bulletsMessage = new MyText("No more bullets!", font25, screen->w / 3, 120, make_color(255, 0, 0), renderer);
    gameOverText = new MyText("GAME OVER!", font50, 10, 100, make_color(255,255,255), renderer);
    string reloadS = "Reloading...("+to_string(reloadDelay)+")";
    reloadMessage = new MyText(reloadS.c_str(), font25, screen->w / 3, 140, make_color(255, 0, 0), renderer);
    string lvlText = "Level: "+to_string(this->level);
    levelText = new MyText(lvlText.c_str(), font25, 480, 5, make_color(255,255,255), renderer);
    restartText = new MyText("Press 'Enter' to start a new game!", font25, 10, 170, make_color(255,255,255), renderer);
    string bRem = "BulletsRemaining: "+to_string(player->getBulletsRemaining());
    bulletsRemaining = new MyText(bRem.c_str(), font25, 5, 5, make_color(255,255,255), renderer);
    string sText = "Score: "+to_string(this->score);
    scoreText = new MyText(sText.c_str(), font25, 480, 30, make_color(255,255,255), renderer);
    hiscoreText = new MyText("new highscore you score !", font25, 10, 200, make_color(255,255,255), renderer);
    returnText = new MyText("Press 'Esc' to return to main screen", font25, 10, 250, make_color(255,255,255), renderer);

    this->score = startScore;
    bulletsMessageShow = false;
    bulletsMessageDelay = 120;
    reloading = false;
    reloadDelay = 180;
    openedFile = false;

    hiscore = score;
    didHiscore = false;

    returnMainscreen = false;

    cout << "Initialization completed! \n";
}

void Level::setLevel(int level) {
    this->level = level;
}

int Level::getLevel() {
    return this->level;
}

void Level::update() {
    const Uint8* keyState = SDL_GetKeyboardState( NULL );

    //cout << "Here! ";

    if(!gameOver) {
        //cout << "Here? \n";
        spawn++;
        if(spawn >= 40) {
            /*
            if(freeSpawn < aSize) {
                if(asteroids[freeSpawn] != NULL) {
                    asteroids[freeSpawn]->setShow(true);
                    freeSpawn++;
                    spawn = 0;
                }
            }
            */
            for(int i = 0; i < aSize; i++) {
                if(asteroids[i] != NULL) {
                    if(!asteroids[i]->isShowing()) {
                        asteroids[i]->setShow(true);
                        spawn = 0;
                        break;
                    }
                }
            }
        }

        player->update();
        player->keyMove(keyState, renderer);
        //cout << "h2 \n";
        int s = bullets.size();
        //cout << "h3 s: " << s << endl;
        //cout << s << endl;
        for(int i = 0; i < s; i++) {
            //cout << "null \n";
            //if(bullets[i] == NULL) cout << "h3.5 NULL!!! \n";

            if(bullets[i] != NULL) {
                //cout << "h4 not null \n";
                //cout << "Not null \n";
                bullets[i]->update();
                //cout << "h5 updating bullet! \n";
                //cout << "Bullet i: x-> " << b[i]->getX() << endl;
                if(bullets[i]->getY() < 0) {

                    bullets.erase(bullets.begin()+i);
                    bullets.reserve(5);
                    bullets.resize(5);
                    cout << "Bullet destroyed on bounds! \n";

                }
            } else {
             //   cout << "null 1 \n";
            }
        }


        int s2 = asteroids.size();
        for(int j = 0; j < aSize; j++) {
            if(asteroids[j] != NULL) {
                asteroids[j]->update();
                if(asteroids[j]->getY() >= screen->h) {
                    cout << "Asteroid hit ground! \n";

                    asteroids.erase(asteroids.begin()+j);
                    asteroids.reserve(aSize);
                    asteroids.resize(aSize);
                    asteroidsRemaining--;
                    player->setHearts(player->getHearts()-1);
                    if(freeHeart >= 0) {
                        hearts[freeHeart]->setShow(false);
                        freeHeart--;
                    }

                }
            }
        }



        // check collisions between asteroids and bullets
        for(int k = 0; k < s; k++) {
            for(int q = 0; q < s2; q++) {
                //    if(a[q] == NULL) cout << "Nulled asteroid! \n";
                //    if(b[k] == NULL) cout << "Nulled bullet! \n";
                if(asteroids[q] != NULL &&  bullets[k] != NULL) {
                  //      if(a[q] == NULL || p->getBullet(k) == NULL) std::cout << "NULL \n";
                    if(asteroids[q]->isShowing()) {
                        if(checkCollision(asteroids[q]->getRect(), bullets[k]->getRect())) {
                        //    std::cout << "k: " << k << std::endl;
                        //    std::cout << "q: " << q << std::endl;
                           // std::cout player->render();<< "Collision detected! \n";
                           cout << "Collision detected! \n";
                            //Asteroid* as1 = a[q];
                            //Bullet* bl1 = b[k];

                            //a[q]->setShow(false);
                            //p->getBullet(k)->setShow(false);

                            //delete a[q];
                            asteroids.erase(asteroids.begin()+q);
                            asteroidsRemaining--;
                            //cout << "Deleting " << a.begin()+q << " asteroid! \n";
                            //a.reserve(a.size());
                            cout << "AsteroidVector new_size: " << asteroids.size() << endl;

                            //delete b[k];
                            bullets.erase(bullets.begin()+k);
                            //b.reserve(b.size());
                            cout << "BulletVector new_size: " << bullets.size() << endl;

                            //delete as1;
                            //delete bl1;

                            std::cout << "Collision finished! \n";
                          //  delete a[q];
                          //  delete p->getBullet(k);
                          //  a[q] = NULL;
                          //  p->setBullet(k, NULL);
                         //   a[q]->setShow(false); a
                         //   p->getBullet(k)->setShow(false);

                            bullets.reserve(5);
                            bullets.resize(5);
                            asteroids.reserve(aSize);
                            asteroids.resize(aSize);

                            this->score += 1;

                        }
                    }
                }
            }
        }

        // check collision between player and asteroids
        for(int i = 0; i < s2; i++) {
            if(asteroids[i] != NULL) {
                if(asteroids[i]->isShowing()) {
               // if(asteroids[i]->getRect().w == 32 && asteroids[i]->getRect().h == 32) { // This is to fix a rect bug of some asteroids
                    if(checkCollision(asteroids[i]->getRect(), player->getRect())) {
                        cout << endl;
                        cout << "Player hit by Asteroid! \n";
                        //cout << "PlayerX/Y: " << player->getX() << ", " << player->getY() << endl;
                        cout << "PlayerRect: {"<< player->getRect().x<<", "<<player->getRect().y<<", "<<player->getRect().w<<", "<<player->getRect().h<<"} \n";
                        //cout << "AsteroidX/Y: " << asteroids[i]->getX() << ", " << asteroids[i]->getY() << endl;
                        cout << "AsteroidRect: {"<< asteroids[i]->getRect().x<<", "<<asteroids[i]->getRect().y<<", "<<asteroids[i]->getRect().w<<", "<<asteroids[i]->getRect().h<<"} \n";
                        cout << endl;
                        /*
                        a.erase(a.begin()+i);
                        a.reserve(5);
                        a.resize(5);
                        decreaseHearts();
                        */

                        asteroids.erase(asteroids.begin()+i);
                        asteroidsRemaining--;
                        cout << "AsteroidVector new_size: " << asteroids.size() << endl;

                        asteroids.reserve(aSize);
                        asteroids.resize(aSize);

                        if(player->getHearts() > 0) {
                            player->setHearts(player->getHearts()-1);
                        }

                        if(freeHeart >= 0) {
                            hearts[freeHeart]->setShow(false);
                            cout << "Removing heart... \n";
                            freeHeart--;
                        }

                        cout << "Collision finished! \n";

                    }

               // }
                }
            }
        }



        /*
        if(p->getBulletsRemaining() == 0) {
            int ns = b.size();
            for(int j = 0; j < ns; j++) {
              file << (b[j]==NULL) << "\n";
            }
        }
        */
        if(bulletsMessageShow) {

            if(bulletsMessageDelay > 0) {
                bulletsMessageDelay--;
                if(bulletsMessage->getTextRect()->y > 100) {
                    bulletsMessage->getTextRect()->y -= 1;
                }

            } else {
                bulletsMessageShow = false;
                bulletsMessageDelay = 120;
                bulletsMessage->getTextRect()->y = 120;
            }

        }

        int hSize = hearts.size();
        for(int h = 0; h < hSize; h++) {
            hearts[h]->update();
        }

        if(player->getHearts() <= 0) {
            gameOver = true;
            if(!openedFile) {
                hiscoreRead.open("saves/hiscore.score");
                int x;
                hiscoreRead >> x;
                hiscoreRead.close();
                hiscore = x;
                if(score > x) {
                    cout << endl;
                    cout << "New Highscore! \n";
                    cout << endl;
                    hiscoreWrite.open("saves/hiscore.score");
                    hiscoreWrite << score;
                    hiscoreWrite.close();
                    hiscore = score;
                    didHiscore = true;
                }
                openedFile = true;
            }
        }

        if(reloading) {
            if(reloadDelay > 0) {
                reloadDelay--;
                string t = "Reloading...("+to_string(reloadDelay)+")";
                reloadMessage->setText(t.c_str());
                reloadMessage->setColor(make_color(255,0,0));
            } else {
                reloading = false;
                reloadDelay = 180;
                player->setBulletsRemaining(5);
                freeIndex = 0;
            }
        }

        if(player->getHearts() > 0 && asteroidsRemaining == 0) {
            this->nextLevel = true;
        }

        string bRem = "BulletsRemaining: "+to_string(player->getBulletsRemaining());
        bulletsRemaining->setText(bRem.c_str());
        bulletsRemaining->setColor(make_color(255,255,255));

        string sText = "Score: "+to_string(this->score);
        scoreText->setText(sText.c_str());
        scoreText->setColor(make_color(255,255,255));
    }


}

string hiText;
void Level::render() {
    //cout << "Rendering... \n";
    int hSize = hearts.size();
    for(int h = 0; h < hSize; h++) {
        hearts[h]->render();
    }



    int s = bullets.size();
    //cout << "size2: " << s <<endl;
    for(int i = 0; i < s; i++) {
        if(bullets[i] != NULL) {
            bullets[i]->render();
        //    cout << "Rendering bullet number " << i << "! VectorSize: " << s << "\n";
        }else {
        //    cout << "null 2 \n";
        }
    }

    //s = asteroids.size();
    for(int j = 0; j < aSize; j++) {
        if(asteroids[j] != NULL) {
            asteroids[j]->render();
        }
    }

    player->render();
    //SDL_Color cl = {255,255,255};
    //drawText("Hi!", 30, cl, 10, 10);

    //drawText(screen, "Test Text!", 100, 10, 10, make_color(255,255,255), make_color(0,0,0));
    //drawText();


    //string t = "BulletsRemaining: " + to_string(player->getBulletsRemaining());
    //MyText* text = new MyText(t.c_str(), 25, 5, 5, make_color(255,255,255), renderer);
    drawText(bulletsRemaining);

    if(bulletsMessageShow) {
        drawText(bulletsMessage);
    }

    if(reloading) {
        drawText(reloadMessage);
    }

    drawText(levelText);

    if(gameOver) {
        drawText(gameOverText);
        drawText(restartText);
        if(!didHiscore) hiText = "Highscore: "+to_string(hiscore)+". Your score: "+to_string(score)+"!";
        else hiText = "NEW HIGHSCORE! Your score: "+to_string(hiscore)+"!";
        hiscoreText->setColor(make_color(255,255,255));
        hiscoreText->setText(hiText.c_str());
        drawText(hiscoreText);
        drawText(returnText);
    }

    drawText(scoreText);
}

SDL_Rect rect;
void Level::handleEvents(SDL_Event event) {
    switch (event.type)
	{
    case SDL_KEYDOWN:
        //p->sdlEvent(event, b);
        if(player->sdlEvent(event)) {
            if(!gameOver) {
                /*
                b[p->getCurrentBullet()]->setShow(true);
                b[p->getCurrentBullet()]->setX(p->getX());
                b[p->getCurrentBullet()]->setY(p->getY());
                */
                //cout << "BulletsRemainingPre: " << p->getBulletsRemaining() << endl;
                if(player->getBulletsRemaining() > 0) {
                    cout << "Adding bullet at index " << freeIndex << endl;

                    rect.x = player->getX() + (player->getRect().w/2);
                    rect.y = player->getY();
                    rect.w = 10;
                    rect.h = 40;

                    bullets[freeIndex] = new Bullet("assets/images/bullet.png", renderer, player->getX() + (player->getRect().w / 2) - rect.w / 2, player->getY() - rect.h / 2, true);
                    //bullets[freeIndex]->setX(bullets[freeIndex]->getX() - bullets[freeIndex]->getRect().w/2);
                    //bullets[freeIndex]->setY(bullets[freeIndex]->getY() + player->getRect().h/2);
                    //b[freeIndex]->setX(p->getX());
                    //b[freeIndex]->setY(p->getY());
                    cout << "Bullet X: " << bullets[freeIndex]->getX() << ", Y: " << bullets[freeIndex]->getY() << endl;
                    if(bullets[freeIndex]->isShowing()) cout << "Showing \n";
                    //b[freeIndex]->setShow(true);
                    if(bullets[freeIndex] == NULL) cout << "Bullet did not created! \n";
                    freeIndex++;
                    player->decreaseBulletsRemaining();
                    cout << "FreeIndex: " << freeIndex << endl;
                    cout << "BulletsRemaining: " << player->getBulletsRemaining() << endl;
                } else {
                    bulletsMessageShow = true;
                    bulletsMessageDelay = 120;
                    bulletsMessage->getTextRect()->y = 120;
                }
            }

        }
        switch(event.key.keysym.sym) {

        case SDLK_r:
            if(player->getBulletsRemaining() == 0) {
                if(reloading == false) {
                    reloading = true;
                    reloadDelay = 180;
                }
            }

            break;
        case SDLK_ESCAPE:
            if(gameOver && !returnMainscreen) {
                returnMainscreen = true;
            }
            break;
        default:
            break;

        }
        break;
	default:
		break;
	}

	//cout << "Events handled! \n";
}

bool Level::checkCollision(SDL_Rect asteroids, SDL_Rect bullets) {
    if(SDL_HasIntersection(&asteroids, &bullets)) {
        return true;
    } else {
        return false;
    }
    /*
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = asteroids.x;
    rightA = asteroids.x + asteroids.w;
    topA = asteroids.y;
    bottomA = asteroids.y + asteroids.h;

    //Calculate the sides of rect B
    leftB = bullets.x;
    rightB = bullets.x + bullets.w;
    topB = bullets.y;
    bottomB = bullets.y + bullets.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
    */
}

bool Level::isNextLevel() {
    return this->nextLevel;
}

void Level::setNextLevel(bool nextLevel) {
    this->nextLevel = nextLevel;
}

void Level::drawText(MyText* text) {
    SDL_RenderCopy(renderer, text->getText(), NULL, text->getTextRect());
}

SDL_Color Level::make_color(Uint8 r, Uint8 g, Uint8 b) {
    SDL_Color c = { r, g, b };
    return c;
}

int Level::getLastPlayerHearts() {
    cout << "Hearts returned: " << player->getHearts() << endl;
    return player->getHearts();
}

bool Level::isGameOver() {
    return gameOver;
}

int Level::getScore() {
    return score;
}

bool Level::isToReturnToMainscreen() {
    return returnMainscreen;
}

