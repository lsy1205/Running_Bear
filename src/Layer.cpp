#include <iostream>
#include <algorithm>
#include "Layer.h"
#include "Game.h"
#include "Files.h"
#include "Button.h"
#include "Timer.h"
#include "Map.h"
using namespace std;

Layer::Layer(string _name, int _texWidth, int _texHeight)
 : name(_name), active(true), changed(true), renderCount(0), mainTexture(NULL), rectViewPort({0, 0, 0, 0})
{
	Game &game = Game::GetGame();
	if(_texWidth < 0 or _texHeight < 0)
	{
		_texWidth = game.getWidth();
		_texHeight = game.getHeight();
	}
	mainTexture = SDL_CreateTexture(game.getRenderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, _texWidth, _texHeight);
	if(!mainTexture)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture for layer %s: %s", name.c_str(), SDL_GetError());
	}
	else
	{
		SDL_SetTextureBlendMode(mainTexture, SDL_BLENDMODE_BLEND);
	}
	rectViewPort.w = game.getWidth();
	rectViewPort.h = game.getHeight();
}

Layer::~Layer()
{
	free();
}

void Layer::free()
{
	for(auto it = elements.begin(); it != elements.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
	elements.clear();
	if(mainTexture)
	{
		SDL_DestroyTexture(mainTexture);
		mainTexture = NULL;
	}
}

bool Layer::handleEvents(SDL_Event &e)
{
	for(auto it = elements.rbegin(); it != elements.rend(); ++it)
	{
		if((*it)->isEventEnable())
			if((*it)->handleEvents(e))	// if handled
			{
				changed = true;
				return true;
			}
	}
	return false;
}

void Layer::update()
{
	for(auto it = elements.begin(); it != elements.end(); ++it)
	{
		//cout << *(*it);
		if((*it)->isUpdateEnable())
			if((*it)->update())
				changed = true;
	}
}

void Layer::render()
{
	if(changed)
	{
		++renderCount;
		SDL_Renderer *renderer = Game::GetGame().getRenderer();
		SDL_SetRenderTarget(renderer, mainTexture);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);
		for(auto it = elements.begin(); it != elements.end(); ++it)
		{
			if((*it)->isRenderEnable() and (*it)->getTexture() and (*it)->getRectOnTexturePtr() and (*it)->getRectOnScreenPtr())
				SDL_RenderCopy(renderer, (*it)->getTexture(), (*it)->getRectOnTexturePtr(), (*it)->getRectOnScreenPtr());
		}
	}
	if(renderCount > 1)
	{
		changed = false;
		renderCount = 0;
	}
}

void Layer::pushElement(BasicObject *_element)
{
	changed = true;
	elements.push_back(_element);
}

void Layer::popElement(BasicObject *_element)
{
	changed = true;
	auto iter = find(elements.begin(), elements.end(), _element);
	if (iter != elements.end())
	{
		elements.erase(iter);
	}
}

Layer &Layer::operator+(BasicObject *_element)
{
	changed = true;
	elements.push_back(_element);
	return *this;
}

Layer &Layer::operator-(BasicObject *_element)
{
	changed = true;
	auto iter = find(elements.begin(), elements.end(), _element);
	if (iter != elements.end())
	{
		elements.erase(iter);
	}
	return *this;
}

BackGround::BackGround(Images index, SDL_Rect _rectOnScreen)
 : BasicObject(Files::P_Images[index])
{
	texture = loadImage(index);
	SDL_QueryTexture(texture, NULL, NULL, &rectOnTexture.w, &rectOnTexture.h);

	if(_rectOnScreen.w > 0 and _rectOnScreen.h > 0)
		rectOnScreen = _rectOnScreen;
	else
	{
		rectOnScreen.w = Game::GetGame().getWidth();
		rectOnScreen.h = Game::GetGame().getHeight();
	}
}

BackGround::~BackGround()
{}

Layer *createLayer(Layers index, BackGround *_bg)
{
	Layer *lay = NULL;
	switch(index)
	{
		case L_STARTMENU:
			lay = new Layer("StartMenu");
			if(_bg)
				(*lay) + _bg;
			(*lay) + new Button(STARTS) + new Button(INTRO1);
			break;
		case L_STATUS:
			lay = new Layer("Status");
			if(_bg)
				(*lay) + _bg;
			(*lay) + new Button(PAUSES) + new Button(MISSION) + new Timer() + new Backpack() + new StrengthBar();
			break;
		case L_INTRO:
			lay = new Layer("Intro");
			if(_bg)
				(*lay) + _bg;
			(*lay) + new Button(LEAVEINTRO);
			break;
		case L_MISSION:
			lay = new Layer("Mission");
			if(_bg)
				(*lay) + _bg;
			(*lay) + new Button(LEAVEMISSION);
			break;
		case L_PAUSE:
			lay = new Layer("Pause");
			if(_bg)
				(*lay) + _bg;
			(*lay) + new Button(RESUMES) + new Button(INTRO2) + new Button(LEAVE);
			break;
		case L_END:
			lay = new Layer("End");
			if(_bg)
				(*lay) + _bg;
			(*lay) + new Button(OK);
			break;
		case L_LOADING:
			lay = new Layer("Loading");
			if(_bg)
				(*lay) + _bg;
			break;
		case L_MAP_GROUND:
			lay = new Layer("MapGround", Map::getMap().getWidth(), Map::getMap().getHeight());
			if(_bg)
				(*lay) + _bg;
			break;
		case L_MAP_FRONT:
			lay = new Layer("MapFront", Map::getMap().getWidth(), Map::getMap().getHeight());
			if(_bg)
				(*lay) + _bg;
			break;
		case L_CHARACTER:
			lay = new Layer("Character", Map::getMap().getWidth(), Map::getMap().getHeight());
			break;
		default:
			break;
	}
	return lay;
}
