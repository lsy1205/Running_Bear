#ifndef HUNTER_H
#define HUNTER_H
#include<iostream>
#include <vector>
#include <string>
#include <queue>
#include "includeSDL.h"
#include "BasicObject.h"

class Map;

enum frames
{
	UP_1, UP_2, UP_3, UP_4, DOWN_1, DOWN_2, DOWN_3, DOWN_4, RIGHT_1, RIGHT_2, RIGHT_3, RIGHT_4, LEFT_1, LEFT_2, LEFT_3, LEFT_4, TOTAL
};

class Hunter : public BasicObject
{
	friend std::ostream & operator<<(std::ostream &, const Hunter &);
	
	SDL_Point HunterMapPos;
	SDL_Point RunnerMapPos; 
	SDL_Point HunterPixelPos;
	SDL_Point HunterCenterPixel;
	SDL_Point directPos;
	SDL_Point NextPixel;
	SDL_Point Place;
	double Hvelocity;
	int frame;
	int updateRate;
	const int Run;
	const int Walk;
	const int Animation_Frame;
	bool SetSuccess;
	bool Discovered;
	bool arrive;
	bool visible;
	bool check;
	std::queue<SDL_Point> go;
	frames direction;
	Map *map;
	
	
public:
	Hunter(SDL_Point, SDL_Point);
	~Hunter();
	bool handleEvents(SDL_Event &e);
	bool update();
	SDL_Point operator~();
	SDL_Point getMapPos()const {return HunterMapPos;}
	virtual SDL_Point getPixelPos()const {return HunterCenterPixel;}
	bool RunnerVisible();
	bool Arrive(SDL_Point);
	void setPixelPos(SDL_Point &);
	void setMapPos(SDL_Point &);
	void Chase(SDL_Point, SDL_Point);
	void Move();
	void initHunter_Clips();
};
std::ostream & operator<<(std::ostream &, const Hunter &);
#endif
