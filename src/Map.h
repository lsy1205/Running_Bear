#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <map>
#include "includeSDL.h"
#include "Layer.h"
#include "Files.h"
#include "Item.h"
#include "Hunter.h"
#include "Runner.h"


enum ObjOnMap
{
	SPACE,
	WALL,
	ITEM,
};

class Map
{
public:
	Map(Maps index, std::string _name = "");
	~Map();

	void loadMap(Maps index);
	void addRunner();
	void addHunter(int);
	void addItems(int);
	void free();
	void start();
	bool handleEvents(SDL_Event &);	// return true if handled
	bool update();

	inline bool isSpace(SDL_Point mapPos) const { return map[mapPos.y][mapPos.x] == SPACE; }
	inline bool isWall(SDL_Point mapPos)  const { return map[mapPos.y][mapPos.x] == WALL; }
	inline bool isItem(SDL_Point mapPos)  const { return map[mapPos.y][mapPos.x] == ITEM; }
	bool placeItem(SDL_Point &, Item *);	// mapPos
	Item *pickItem(SDL_Point &);			// mapPos
	ItemList peekItem(SDL_Point &);			// mapPos

	SDL_Point pixelPosTomapPos(SDL_Point);
	SDL_Point mapPosTopixelPos(SDL_Point);

	inline std::vector<SDL_Point> getHuntersMapPos() const { return huntersMapPos; }
	inline SDL_Point getRunnerMapPos() const { return runnerMapPos; }

	inline int getRowNum() const { return rowNum; }
	inline int getColNum() const { return colNum; }
	inline int getWidth()  const { return width; }
	inline int getHeight() const { return height; }
	inline static int getPixelWidth()  { return sc_pixelWidth; }
	inline static int getPixelHeight() { return sc_pixelHeight; }

	inline static Map &getMap() { return *s_mapInstance;}
private:
	void buildMap();

	std::string name;
	int colNum, rowNum;
	std::vector<std::vector<int>> map;
	int width, height;
	SDL_Point mapPixelPos;
	bool started;
	// for map on Window
	#ifdef _WIN32
	struct SDL_PointComp{ bool operator()(const SDL_Point , const SDL_Point ); };
	std::map<SDL_Point, Item *, SDL_PointComp> items;
	#endif
	// for map on Mac
	#ifdef __APPLE__
	std::map<int, Item *> items;
	#endif

	Runner *runner;
	std::vector<Hunter *> hunters;
	std::vector<SDL_Point> huntersMapPos;
	SDL_Point runnerMapPos;

	Layer *L_ground;
	Layer *L_character;
	Layer *L_front;	// overlayer
	SDL_Texture *groundTexture;
	SDL_Texture *frontTexture;

	static const int sc_pixelWidth, sc_pixelHeight;
	static Map *s_mapInstance;
};

#endif	// MAP_H