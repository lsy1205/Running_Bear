#include <fstream>
#include <sstream>
#include "Map.h"
#include "Game.h"
#include "BasicObject.h"
#include "Timer.h"
#include "UserEvent.h"
using namespace std;

#ifdef __APPLE__
int operator~(SDL_Point pos)
{
	return pos.y * Map::getPixelWidth() + pos.x;
}
#endif

const int Map::sc_pixelWidth = 70, Map::sc_pixelHeight = 70;
Map *Map::s_mapInstance = NULL;

Map::Map(Maps index, string _name)
 : name(_name), colNum(0), rowNum(0), width(0), height(0), mapPixelPos({0, 0}), started(false),
	L_ground(NULL), L_character(NULL), L_front(NULL), groundTexture(NULL), frontTexture(NULL)
{
	s_mapInstance = this;
	loadMap(index);

	buildMap();

	addRunner();
	addHunter(10);
	addItems(30);
}

Map::~Map()
{
	free();
	s_mapInstance = NULL;
}

void Map::loadMap(Maps index)
{
	ifstream* inMapFile = openMapFile(index);
	char c;
	string s;
	stringstream ss;

	getline(*inMapFile, s);
	ss.str(s);
	ss.clear();
	ss >> rowNum >> c;
	ss >> colNum >> c;

	int n;
	for(int i = 0; i < rowNum; ++i)
	{
		if(!getline(*inMapFile, s))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load map %d", index);
			rowNum = 0;
			colNum = 0;
			return;
		}
		ss.str(s);
		ss.clear();
		vector<int> tmpV;
		for(int j = 0; j < colNum; ++j)
		{
			if(!getline(ss, s, ','))
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load map %d", index);
				rowNum = 0;
				colNum = 0;
				return;
			}
			if(s.length())
				n = stoi(s);
			else
				n = 0;
			tmpV.push_back(n);
		}
		map.push_back(tmpV);
	}
	(*inMapFile).close();
	delete inMapFile;

	width = colNum * sc_pixelWidth;
	height = rowNum * sc_pixelHeight;
}

void Map::addRunner()
{
	Game &game = Game::GetGame();
	SDL_Point tmpMapPos, tmpPixelPos;
	do
	{
		tmpMapPos.x = game.rdEngine() % rowNum;
		tmpMapPos.y = game.rdEngine() % colNum;
	}while(!isSpace(tmpMapPos));
	tmpPixelPos = mapPosTopixelPos(tmpMapPos);
	tmpPixelPos.x += sc_pixelWidth / 2;
	tmpPixelPos.y += sc_pixelHeight / 2;
	runnerMapPos = tmpMapPos;
	runner = new Runner(tmpMapPos, tmpPixelPos);
	L_character->pushElement(runner);
}

void Map::addHunter(int num)
{
	SDL_Point tmpMapPos, tmpPixelPos;
	Game &game = Game::GetGame();
	for(int i = 0; i < num; ++i)
	{
		do
		{
			tmpMapPos.x = game.rdEngine() % rowNum;
			tmpMapPos.y = game.rdEngine() % colNum;
		}while(!isSpace(tmpMapPos) or tmpMapPos == runnerMapPos);
		tmpPixelPos = mapPosTopixelPos(tmpMapPos);
		tmpPixelPos.x += sc_pixelWidth / 2;
		tmpPixelPos.y += sc_pixelHeight / 2;
		hunters.push_back(new Hunter(tmpMapPos, tmpPixelPos));
		L_character->pushElement(hunters.back());
		huntersMapPos.push_back(tmpMapPos);
	}
}

void Map::addItems(int num)
{
	SDL_Point tmpMapPos;
	ItemList tmpItemList;
	Game &game = Game::GetGame();
	for(int i = 0; i < num; ++i)
	{
		do
		{
			tmpMapPos.x = game.rdEngine() % (rowNum-2) + 1;
			tmpMapPos.y = game.rdEngine() % (colNum-2) + 1;
		}while(!isSpace(tmpMapPos));

		switch(game.rdEngine()%3)
		{
			case 0:
				tmpItemList = STAR;
				break;
			case 1:
				tmpItemList = POTION;
				break;
			case 2:
				tmpItemList = MEAT;
				break;
		}
		placeItem(tmpMapPos, createItem(tmpItemList));
	}
}

void Map::free()
{
	SDL_DestroyTexture(groundTexture);
	groundTexture = NULL;
	SDL_DestroyTexture(frontTexture);
	frontTexture = NULL;

	Game &game = Game::GetGame();
	game.popOverlayer(L_front);
	delete L_front;
	L_front = NULL;
	game.popLayer(L_character);
	delete L_character;
	L_character = NULL;
	game.popLayer(L_ground);
	delete L_ground;
	L_ground = NULL;
	items.clear();		// already deleted by L_character
	hunters.clear();	// already deleted by L_character
	runner = NULL;		// already deleted by L_character
	game.setGameMap(NULL);
}

void Map::start()
{
	Game &game = Game::GetGame();
	game.pushLayer(L_ground);
	game.pushLayer(L_character);
	game.pushOverlayer(L_front);
	game.pushOverlayer(createLayer(L_STATUS, new BackGround(STATUS_IMAGE)));
	createUserEvent(TIMERCHANGE, TIMERSTART, NULL, NULL);
	started = true;
}

bool Map::handleEvents(SDL_Event &event)
{
	return false;
}

bool Map::update()
{
	if(!started)
	{
		start();
	}
	SDL_Point tmpPixelPos;
	Game &game = Game::GetGame();
	int scrWidth = game.getWidth(), scrHeight = game.getHeight();
	//cout << *runner;
	tmpPixelPos = runner->getPixelPos();
	runnerMapPos = pixelPosTomapPos(tmpPixelPos);
	mapPixelPos = SDL_Point({scrWidth/2 - tmpPixelPos.x, scrHeight/2 - tmpPixelPos.y});
	for(long long unsigned int i = 0; i < hunters.size(); ++i)
	{
		//cout << *hunters[i];
		huntersMapPos[i] = pixelPosTomapPos(hunters[i]->getPixelPos());
		if(huntersMapPos[i] == runnerMapPos)
			createUserEvent(GAMESTATE_CHANGE, END, new bool(false), NULL);
	}
	
	if(mapPixelPos.x > 0)
		mapPixelPos.x = 0;
	else if(mapPixelPos.x < scrWidth - width)
		mapPixelPos.x =scrWidth - width;
	if(mapPixelPos.y > 0)
		mapPixelPos.y = 0;
	else if(mapPixelPos.y < scrHeight - height)
		mapPixelPos.y = scrHeight - height;
	SDL_Point tmp = {-mapPixelPos.x, -mapPixelPos.y};
	L_ground->setRectViewPos(tmp);
	L_character->setRectViewPos(tmp);
	L_front->setRectViewPos(tmp);
	return true;
}

bool Map::placeItem(SDL_Point &pos, Item *item)	// mapPos
{
	if(!isSpace(pos) or item == NULL)
		return false;
	else
	{
		map[pos.y][pos.x] = ITEM;
		SDL_Point tmpPixelPos = mapPosTopixelPos(pos);
		tmpPixelPos.x += sc_pixelWidth / 2;
		tmpPixelPos.y += sc_pixelHeight / 2;
		item->setPixelPos(tmpPixelPos);
		#ifdef _WIN32
		items.insert({pos, item});
		#endif
		#ifdef __APPLE__
		items.insert({~pos, item});
		#endif
		L_ground->pushElement(item);
		return true;
	}
}

Item *Map::pickItem(SDL_Point &pos)		// mapPos
{
	if(!isItem(pos))
		return NULL;
	else
	{
		#ifdef _WIN32
		Item *tmp = items[pos];
		items.erase(pos);
		#endif
		#ifdef __APPLE__
		Item *tmp = items[~pos];
		items.erase(~pos);
		#endif
		map[pos.y][pos.x] = SPACE;
		L_ground->popElement(tmp);
		return tmp;
	}
}

ItemList Map::peekItem(SDL_Point &pos)	// mapPos
{
	if(!isItem(pos))
		return BAD_ITEM;
	else
	{
		#ifdef _WIN32
		return items[pos]->getItemType();
		#endif
		#ifdef __APPLE__
		return items[~pos]->getItemType();
		#endif
	}
}

SDL_Point Map::pixelPosTomapPos(SDL_Point pixelPos)
{
	return SDL_Point({pixelPos.x / sc_pixelWidth, pixelPos.y / sc_pixelHeight});
}

SDL_Point Map::mapPosTopixelPos(SDL_Point mapPos)
{
	return SDL_Point({mapPos.x * sc_pixelWidth, mapPos.y * sc_pixelHeight});
}

void Map::buildMap()
{
	Game &game = Game::GetGame();
	SDL_Renderer *renderer = game.getRenderer();
	groundTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
	frontTexture  = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetTextureBlendMode(groundTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(frontTexture, SDL_BLENDMODE_BLEND);

	SDL_SetRenderTarget(renderer, groundTexture);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, frontTexture);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);

	SDL_SetRenderTarget(renderer, groundTexture);
	SDL_RenderCopy(renderer, loadImage(BACKGROUND_IMAGE), NULL, NULL);

	SDL_Texture *wallTexture = loadImage(WALL_IMAGE);
	SDL_Rect top = {0, 0, 550, 300};
	SDL_Rect base = {0, 300, 550, 550};
	SDL_Rect target;
	SDL_Point tmpPixelPos;
	for(int i = 0; i < rowNum; ++i)
	{
		for(int j = 0; j < colNum; ++j)
		{
			if(isWall(SDL_Point({j, i})))
			{
				tmpPixelPos = mapPosTopixelPos(SDL_Point({j, i}));
				target = {tmpPixelPos.x, tmpPixelPos.y, sc_pixelWidth, sc_pixelHeight};
				SDL_SetRenderTarget(renderer, groundTexture);
				SDL_RenderCopy(renderer, wallTexture, &base, &target);
				if(i > 0)
				{
					tmpPixelPos = mapPosTopixelPos(SDL_Point({j, i-1}));
					target = {tmpPixelPos.x, tmpPixelPos.y + sc_pixelHeight/2, sc_pixelWidth, sc_pixelHeight/2};
					SDL_SetRenderTarget(renderer, groundTexture);
					SDL_SetRenderTarget(renderer, frontTexture);
					SDL_RenderCopy(renderer, wallTexture, &top, &target);
				}
			}
		}
	}

	SDL_Rect tmpRect = {0, 0, width, height};
	L_ground = createLayer(L_MAP_GROUND, NULL);
	BasicObject* ground = new BasicObject("Ground", false, true, true);
	ground->setTexture(groundTexture);
	ground->setRectOnTexture(tmpRect);
	ground->setRectOnScreen(tmpRect);
	L_ground->pushElement(ground);

	L_front = createLayer(L_MAP_FRONT, NULL);
	BasicObject* front = new BasicObject("Front", false, true, true);
	front->setTexture(frontTexture);
	front->setRectOnTexture(tmpRect);
	front->setRectOnScreen(tmpRect);
	L_front->pushElement(front);

	L_character = createLayer(L_CHARACTER, NULL);
}

#ifdef _WIN32
bool Map::SDL_PointComp::operator()(const SDL_Point lhs, const SDL_Point rhs)
{
	if(lhs.x != rhs.x)
		return lhs.x < rhs.x;
	else
		return lhs.y < rhs.y;
}
#endif
