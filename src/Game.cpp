#include <algorithm>
#include <random>
#include <ctime>
#include "Game.h"
#include "Layer.h"
#include "Map.h"
#include "Files.h"
#include "Timer.h"
#include "UserEvent.h"
using namespace std;

Game *Game::s_gameInstance = NULL;

Game::Game(string _name, unsigned int _width, unsigned int _height,  unsigned int _frameRate)
 : name(_name), width(_width), height(_height), frameRate(_frameRate), running(false), state(STARTMENU),
	window(NULL), renderer(NULL), layerInsertIndex(0U), gameMap(NULL), bgm(NULL), startTime(0U), frameCount(0U), eventStart(0U), duration(0U), puasedCount(0), started(false)
{
	rdEngine.seed(time(NULL));
	s_gameInstance = this;
}

Game::~Game()
{
	Quit();
	s_gameInstance = NULL;
}

bool Game::Init()
{
	// SDL
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return false;
	}

	// SDL_ttf
	if(!TTF_WasInit())
	{
		if(TTF_Init() < 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Init: Failed to init required font support: %s", TTF_GetError());
			return false;
		}
	}

	// SDL_mixer
	int Mix_flags = MIX_INIT_MP3;//MIX_INIT_FLAC|MIX_INIT_MOD|MIX_INIT_MP3|MIX_INIT_OGG;
	if(Mix_Init(Mix_flags) ^ Mix_flags)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Mix_Init: Failed to init required audio support: %s", Mix_GetError());
		return false;
	}
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Mix_OpenAudio: Failed to init required audio support: %s", Mix_GetError());
		return false;
	}

	// SDL_image
	int IMG_flags = IMG_INIT_JPG|IMG_INIT_PNG;
	if(IMG_Init(IMG_flags) ^ IMG_flags)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Init: Failed to init required image support: %s", IMG_GetError());
		return false;
	}

	window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, 40,
								width, height, SDL_WINDOW_SHOWN);//|SDL_WINDOW_RESIZABLE);
	if(!window)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
	if(!renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	return true;
}

void Game::Start(unsigned int _startTime)
{
	startTime = _startTime;
	frameCount = 1;
	running = true;
	bgm = loadMusic(BGM_MUSIC);
	Mix_PlayMusic(bgm, -1);
	pushLayer(createLayer(L_STARTMENU, new BackGround(START_IMAGE)));
}

void Game::HandleEvents()
{
	static bool handled = false;
	while(SDL_PollEvent(&event) or SDL_GetTicks() - startTime < frameCount * 1000 / frameRate)
	{
		handled = false;
		if(event.type == SDL_QUIT)
		{
			handled = true;
			running = false;
			return;
		}

		if(event.type == GAMESTATE_CHANGE)
		{
			handled = true;
			GameState tmp = GameState(event.user.code);
			switch(tmp)
			{
				case START:
					state = LOADING;
					popAllLayers();
					pushOverlayer(createLayer(L_LOADING, new BackGround(DONATE_IMAGE)));
					Render();
					gameMap = new Map(MAP_01, "Map01");
					eventStart = SDL_GetTicks();
					duration = 2000;
					break;
				case PAUSE:
					state = PAUSE;
					if(puasedCount < 1)
						createUserEvent(TIMERCHANGE, TIMERPAUSE, NULL, NULL);
					++puasedCount;
					break;
				case RESUME:
					--puasedCount;
					if(puasedCount == 0)
					{
						if(started)
							state = GAME;
						else
							state = STARTMENU;
						createUserEvent(TIMERCHANGE, TIMERUNPAUSE, NULL, NULL);
					}
					break;
				case END:
					state = END;
					if(*((bool *)event.user.data1))
						pushOverlayer(createLayer(L_END, new BackGround(END_WIN_IMAGE)));
					else
						pushOverlayer(createLayer(L_END, new BackGround(END_LOSE_IMAGE)));
					Render();
					break;
				case STARTMENU:
					state = STARTMENU;
					delete gameMap;
					gameMap = NULL;
					popAllLayers();
					Mix_PlayMusic(bgm, -1);
					pushLayer(createLayer(L_STARTMENU, new BackGround(START_IMAGE)));
					Render();
					
				default:
					break;
			}
		}

		if((state == PAUSE and event.type != TIMERCHANGE) or state == END)
		{
			handled = true;
			Layer *topLayer = layers.back();
			if(topLayer->isActive())
				topLayer->handleEvents(event);
		}

		for(auto it = layers.rbegin(); !handled and it != layers.rend(); ++it)
		{
			if((*it)->isActive())
				if((*it)->handleEvents(event))	// if handled
				{
					handled = true;
					break;	// for
				}
		}
		if(!handled and gameMap)
			gameMap->handleEvents(event);
		SDL_zero(event);
	}
}

void Game::Update()
{
	if(state == PAUSE or state == LOADING or state == END)
	{
		Layer *topLayer = layers.back();
		if(topLayer->isActive())
			topLayer->update();
		if(state == LOADING)
		{
			if( SDL_GetTicks() > eventStart + duration)
			{
				started = true;
				popTopOverlayer();
				state = GAME;
				while(SDL_PollEvent(&event)){}
			}
		}
		return;
	}

	for(auto it = layers.begin(); it != layers.end(); ++it)
	{
		if((*it)->isActive())
			(*it)->update();
	}
	if(gameMap)
	{
		gameMap->update();
	}
}

void Game::Render()
{
	if(state == PAUSE or state == LOADING  or state == END)
	{
		Layer *topLayer = layers.back();
		if(topLayer->isActive())
			topLayer->render();
	}
	else
	{
		for(auto it = layers.begin(); it != layers.end(); ++it)
		{
			(*it)->render();
		}
	}
	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	for(auto it = layers.begin(); it != layers.end(); ++it)
	{
		if((*it)->isActive())
			SDL_RenderCopy(renderer, (*it)->getTexture(), (*it)->getRectViewPortPtr(), NULL);
	}
	SDL_RenderPresent(renderer);
	++frameCount;
	//printf("%d\n", frameCount);	// debug
}

void Game::Quit()
{
	delete gameMap;
	gameMap = NULL;

	for(auto it = layers.begin(); it != layers.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
	layers.clear();
	if(renderer)
	{
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if(window)
	{
		SDL_DestroyWindow(window);
		window = NULL;
	}
	freeAllFiles();
	IMG_Quit();
	Mix_HaltMusic();
	Mix_FreeMusic(bgm);
	bgm = NULL;
	for(int i = Mix_QuerySpec(NULL, NULL, NULL); i > 0; --i)
		Mix_CloseAudio();
	while(Mix_Init(0))
		Mix_Quit();
	if(TTF_WasInit())
		TTF_Quit();
	SDL_Quit();
}

void Game::pushLayer(Layer *_layer)
{
	layers.insert(layers.begin() + layerInsertIndex, _layer);
	++layerInsertIndex;
}

void Game::pushOverlayer(Layer *_overlayer)
{
	layers.push_back(_overlayer);
}

void Game::popLayer(Layer *_layer)
{
	auto iter = find(layers.begin(), layers.begin() + layerInsertIndex, _layer);
	if (iter != layers.begin() + layerInsertIndex)
	{
		layers.erase(iter);
		--layerInsertIndex;
	}
}

void Game::popOverlayer(Layer *_overlayer)
{
	auto iter = find(layers.begin() + layerInsertIndex, layers.end(), _overlayer);
	if (iter != layers.end())
	{
		layers.erase(iter);
	}
}

void Game::popTopOverlayer()
{
	Layer *overlayer = layers.back();
	auto iter = find(layers.begin() + layerInsertIndex, layers.end(), overlayer);
	if (iter != layers.end())
	{
		layers.erase(iter);
		delete *iter;
	}
}

void Game::popAllLayers()
{
	for(auto it = layers.begin(); it != layers.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
	layers.clear();
	layerInsertIndex = 0;
}
