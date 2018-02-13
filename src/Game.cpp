#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "MyButton.h"
#include "Level.h"
Game::Game()
{}

Game::~Game()
{}

Level* level;
int l = 1;
int lastHearts;
int cScore = 0;

bool startLevel = false;
bool playPushed = false;

GameObject* asteroidsTitle;
MyButton* buttonPlay;
MyButton* buttonShop;
MyButton* buttonCredits;

TTF_Font* font;
MyText* versionText;
MyText* madeByText;

//MyButton* rectsShow;

SDL_Surface* screen;
void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);

		screen = SDL_GetWindowSurface(window);
		/*
		SDL_Surface* icon = IMG_Load("assets/images/asteroid2.png");
		//SDL_Texture* iconTexture = TextureManager::LoadTexture("assets/images/asteroid2.png", renderer);
		//SDL_Surface* icon =
		if(icon == NULL) {
            std::cout << "Nulled icon! " << SDL_GetError() << std::endl;
		}
        SDL_SetWindowIcon(window, icon);
        */

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		}

		isRunning = true;
	}

	if ( TTF_Init( ) == -1 )
    {
        std::cout << "Initialization of TTF failed : " << SDL_GetError() << std::endl;
    }

	level = new Level(l, screen, renderer, 3, cScore);
	lastHearts = 3;
    std::cout << SDL_GetError() << std::endl;
    std::cout << "Level " << level->getLevel() << " created! \n";

    asteroidsTitle = new GameObject("assets/images/asteroids.png", renderer, screen->w / 6, 50);
    asteroidsTitle->setSimpleTexture(true);
    asteroidsTitle->setShow(true);
    asteroidsTitle->setDimensions(400, 100);

    buttonPlay = new MyButton("assets/images/button_play.png", renderer, screen->w / 3, 250, 200, 54);
    buttonShop = new MyButton("assets/images/button_shop.png", renderer, screen->w / 3, 350, 200, 54);
    buttonCredits = new MyButton("assets/images/button_credits.png", renderer, screen->w / 3, 450, 200, 54);

    font = TTF_OpenFont("OpenSans-Regular.ttf", 25);
    versionText = new MyText("Version: 0.3", font, 10, 720, {255,255,255}, renderer);
    madeByText = new MyText("Made by Nikolas Spyropoulos", font, 10, 750, {255,255,255}, renderer);

    //rectsShow = new MyButton("assets/images/red.png", renderer, 500, 750, 100, 36);
}

void Game::handleEvents()
{
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
    case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {

        case SDLK_RETURN:
            if(level->isGameOver()) {
                delete level;
                l = 1;
                cScore = 0;
                level = new Level(l, screen, renderer, 3, cScore);
                lastHearts = 3;
            }

            break;
        default:
            break;

        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if(!startLevel) {
            int x, y;
            SDL_GetMouseState( &x, &y );
            if(buttonPlay->isTriggered(x, y)) {
                std::cout << x << " " << y << std::endl;
                startLevel = true;
            }
        }
        break;
	default:
		break;
	}

	if(startLevel) {
        level->handleEvents(event);
	}
}

void Game::update()
{
    if(startLevel) {
        level->update();
        if(level->isNextLevel()) {
            lastHearts = level->getLastPlayerHearts();
            cScore = level->getScore();
            std::cout << std::endl;
            std::cout << "Starting next level! \n";
            std::cout << "New Player hearts: " << lastHearts << std::endl;
            std::cout << std::endl;
            delete level;
            l++;
            level = new Level(l, screen, renderer, lastHearts, cScore);
            level->setNextLevel(false);
        }
        if(level->isToReturnToMainscreen()) {
            delete level;
            l = 1;
            cScore = 0;
            level = new Level(l, screen, renderer, 3, cScore);
            startLevel = false;
        }
    } else {
        asteroidsTitle->update();

        int x, y;
        SDL_GetMouseState( &x, &y );
        buttonPlay->isTriggered(x, y);
        buttonPlay->update("assets/images/button_play.png", "assets/images/button_play_pushed.png");
        buttonShop->isTriggered(x, y);
        buttonShop->update("assets/images/button_shop.png", "assets/images/button_shop_pushed.png");
        buttonCredits->isTriggered(x, y);
        buttonCredits->update("assets/images/button_credits.png", "assets/images/button_credits_pushed.png");
        //rectsShow->isTriggered(x, y);
        //rectsShow->update("assets/images/red.png", "assets/images/red_pushed.png");

        /*
        if(buttonPlay->isTriggered(x, y)) {
            if(!playPushed) {
                buttonPlay->setTexture("assets/images/button_play_pushed.png");
                buttonPlay->setX(buttonPlay->getX()+1);
                buttonPlay->setY(buttonPlay->getY()+1);
                buttonPlay->setDimensions(200, 53);
                playPushed = true;
            }
        } else {
            if(playPushed) {
                buttonPlay->setTexture("assets/images/button_play.png");
                buttonPlay->setX(buttonPlay->getX()-1);
                buttonPlay->setY(buttonPlay->getY()-1);
                buttonPlay->setDimensions(200, 54);
                playPushed = false;
            }
        }
        */

    }


}

void Game::render()
{
	SDL_RenderClear(renderer);
	if(startLevel) {
        level->render();
	} else {
        asteroidsTitle->render();
        buttonPlay->render();
        buttonShop->render();
        buttonCredits->render();
        //rectsShow->render();
        level->drawText(versionText);
        level->drawText(madeByText);
	}
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}


