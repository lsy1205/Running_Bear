#include <queue>
#include <iostream>
#include <cmath>
#include<iostream>
#include "includeSDL.h"
#include "Hunter.h"
#include "Game.h"
#include "Map.h"
#include "UserEvent.h"
using namespace std;

SDL_Rect Hunter_Clip[TOTAL];

ostream &operator<<(ostream &output, const Hunter &h)
{
	output << h.directPos.x << "," << h.directPos.y << '\n';
	return output;
}

Hunter::Hunter(SDL_Point MapPos, SDL_Point PixelPos) :
	BasicObject("Hunter"), arrive(false), SetSuccess(false), Discovered(false),
	Animation_Frame(4), Run(3), Walk(2), updateRate(20), frame(0), map(&Map::getMap())
{
	Hvelocity = Walk;
	direction = DOWN_1;
	check = true;
	initHunter_Clips();
	texture = loadImage(HUNTER_IMAGE);
	rectOnTexture = Hunter_Clip[direction+frame/updateRate];
	rectOnScreen.w = Map::getPixelWidth();
	rectOnScreen.h = Map::getPixelHeight();
	setMapPos(MapPos);
	setPixelPos(PixelPos);
	directPos = HunterPixelPos;
}

void Hunter::setMapPos(SDL_Point &MapPos)
{
	HunterMapPos = MapPos;
}

void Hunter::setPixelPos(SDL_Point &PixelPos)
{
	HunterCenterPixel = PixelPos;
	HunterPixelPos.x = HunterCenterPixel.x - rectOnScreen.w/2;
	HunterPixelPos.y = HunterCenterPixel.y - rectOnScreen.h/2;
	rectOnScreen.x = HunterPixelPos.x;
	rectOnScreen.y = HunterPixelPos.y;
}


Hunter::~Hunter()
{
	
}

bool Hunter::handleEvents(SDL_Event &e)
{
	return false;
}

bool Hunter::update()
{
	frame++;

	HunterCenterPixel.x = HunterPixelPos.x + rectOnScreen.w/2;
	HunterCenterPixel.y = HunterPixelPos.y + rectOnScreen.h/2;
	HunterMapPos = map->pixelPosTomapPos(HunterCenterPixel);
	RunnerMapPos = map->getRunnerMapPos();
	if(RunnerVisible())
	{
		check = false;
		Hvelocity = Run;
		Place = RunnerMapPos;
		if(!Discovered || Arrive(directPos))
		{
			if(!Discovered)  Discovered = true;
			directPos = map->mapPosTopixelPos(Place);
			Chase(HunterMapPos, Place);
			NextPixel = map->mapPosTopixelPos(HunterMapPos);
		}
	}
	else 
	{
		Discovered = false;
		
		if(!check)
			{
				check = true;
				directPos = map->mapPosTopixelPos(Place);
				Chase(HunterMapPos, Place);
				NextPixel = map->mapPosTopixelPos(HunterMapPos);
			}
		if(Arrive(directPos))
		{
			Hvelocity = Walk;
			directPos = ~(*this);
			Chase(HunterMapPos, map->pixelPosTomapPos(directPos));
			NextPixel = map->mapPosTopixelPos(HunterMapPos);		
		}
	}

	

	Move();
	if(frame/updateRate == Animation_Frame){frame=0;}
	rectOnTexture = Hunter_Clip[direction+frame/updateRate];
	rectOnScreen.x = HunterPixelPos.x;
	rectOnScreen.y = HunterPixelPos.y;
	return true;
}

bool Hunter::RunnerVisible()
{
	double a, b, deltaX, deltaY, findX, findY;
	SDL_Point findpos;
	visible = true;
	a = RunnerMapPos.x - HunterMapPos.x;
	b = RunnerMapPos.y - HunterMapPos.y;
	deltaX = a / sqrt(a*a+b*b);
	deltaY = b / sqrt(a*a+b*b);
	findpos = HunterMapPos;
	findX = HunterMapPos.x;
	findY = HunterMapPos.y;
	while(findpos.x != RunnerMapPos.x || findpos.y != RunnerMapPos.y)
	{
		findX += deltaX;
		findY += deltaY;
		findpos.x = round(findX);
		findpos.y = round(findY);
		if(map->isWall(findpos))
		{
			visible = false;
			break;
		}
	}
	return visible;
}


bool Hunter::Arrive(SDL_Point destination)
{
	arrive = false;
	if(abs(HunterPixelPos.x - destination.x) < Hvelocity && abs(HunterPixelPos.y - destination.y) < Hvelocity)
	{
		arrive = true;
	}
	return arrive;
}

void Hunter::Move()
{
	if(Arrive(NextPixel))
	{
		NextPixel = map->mapPosTopixelPos(go.front());
		go.pop();
	}	
	else
	{
		if(HunterPixelPos.x != NextPixel.x)
		{
			if(abs(HunterPixelPos.x - NextPixel.x) < Hvelocity)
			{
				HunterPixelPos.x = NextPixel.x;
				/*
				if(HunterPixelPos.x - NextPixel.x > 0)
				{
					direction = LEFT_1;
				}
				else
				{
					direction = RIGHT_1;
				}*/
			}
			else if(HunterPixelPos.x < NextPixel.x)
			{
				HunterPixelPos.x += Hvelocity;
				direction = RIGHT_1;
			}
			else if(HunterPixelPos.x > NextPixel.x)
			{
				HunterPixelPos.x -= Hvelocity;
				direction = LEFT_1;
			}
		}
		else
		{
			if(abs(HunterPixelPos.y - NextPixel.y) < Hvelocity)
			{
				HunterPixelPos.y = NextPixel.y;
				/*
				if(HunterPixelPos.y - NextPixel.y > 0)
				{
					direction = UP_1;
				}
				else
				{
					direction = DOWN_1;
				}*/
			}
			else if(HunterPixelPos.y < NextPixel.y)
			{
				HunterPixelPos.y += Hvelocity;
				direction = DOWN_1;
			}
			else if(HunterPixelPos.y > NextPixel.y)
			{
				HunterPixelPos.y -= Hvelocity;
				direction = UP_1;
			}
		}
	}
}

SDL_Point Hunter::operator~()	
{
	SetSuccess = false;
	SDL_Point SetPos;
	while(!SetSuccess)
	{
		Game &game = Game::GetGame();
		SetPos.x = game.rdEngine() % map->getRowNum();
		SetPos.y = game.rdEngine() % map->getColNum();
		if(map->isSpace(SetPos)) 
		{
			SetSuccess = true;
			SDL_Point tmp;
			tmp = map->mapPosTopixelPos(SetPos);
			return tmp;
		} 
	}
}


void Hunter::Chase(SDL_Point HunterMapPos, SDL_Point directpos)
{
bool visited[map->getRowNum()][map->getColNum()];
	for(int i=0; i<map->getRowNum(); i++)
	for(int j=0; j<map->getColNum(); j++)
	visited[i][j] = false;
	
	class Node
	{
	public:
		int step;
		std::vector<SDL_Point> way;
		Node(){} 
		~Node(){}
		Node(const Node &x){
			step = x.step;
			for(auto it = x.way.begin(); it != x.way.end(); ++it)
			{
				way.push_back(*it);
			}
		}
	};
	
	std::queue<Node> q; 
	Node init; 
	init.step = 0;
	init.way.push_back(HunterMapPos);  
	visited[HunterMapPos.x][HunterMapPos.y] = true;
	q.push(init);
	while(!q.empty())
	{
		Node curNode = q.front(); 
		q.pop();
		SDL_Point curP = curNode.way.back();
		//right
		SDL_Point nextP1;
		nextP1.x = curP.x + 1;
		nextP1.y = curP.y;
		if(!(map->isWall(nextP1)) && !(visited[nextP1.x][nextP1.y]))
		{
			visited[nextP1.x][nextP1.y] = true;
			Node tmp;
			tmp.step = curNode.step + 1;
			for(int i=0; i<tmp.step; i++)
			{
				tmp.way.push_back(curNode.way[i]);
			}
			tmp.way.push_back(nextP1);
			
			if(nextP1.x == directpos.x && nextP1.y == directpos.y)
			{
				while(!go.empty())
				{
					go.pop();
				}
				for(int i=0; i<=tmp.step; i++)
				{
					go.push(tmp.way[i]);
				}
				break;
			}
			else
			{
				q.push(tmp);
			}
		}
		//left
		SDL_Point nextP2;
		nextP2.x = curP.x - 1;
		nextP2.y = curP.y;
		if(!(map->isWall(nextP2)) && !(visited[nextP2.x][nextP2.y]))
		{
			visited[nextP2.x][nextP2.y] = true;
			Node tmp;
			tmp.step = curNode.step + 1;
			for(int i=0; i<tmp.step; i++)
			{
				tmp.way.push_back(curNode.way[i]);
			}
			tmp.way.push_back(nextP2);
			
			if(nextP2.x == directpos.x && nextP2.y == directpos.y)
			{
				while(!go.empty())
				{
					go.pop();
				}
				for(int i=0; i<=tmp.step; i++)
				{
					go.push(tmp.way[i]);
				}
				break;
			}
			else
			{
				q.push(tmp);
			}
		}
		//down
		SDL_Point nextP3;
		nextP3.x = curP.x;
		nextP3.y = curP.y + 1;
		if(!(map->isWall(nextP3)) && !(visited[nextP3.x][nextP3.y]))
		{
			visited[nextP3.x][nextP3.y] = true;
			Node tmp;
			tmp.step = curNode.step + 1;
			for(int i=0; i<tmp.step; i++)
			{
				tmp.way.push_back(curNode.way[i]);
			}
			tmp.way.push_back(nextP3);
			
			if(nextP3.x == directpos.x && nextP3.y == directpos.y)
			{
				while(!go.empty())
				{
					go.pop();
				}
				for(int i=0; i<=tmp.step; i++)
				{
					go.push(tmp.way[i]);
				}
				break;
			}
			else
			{
				q.push(tmp);
			}
		}
		//up
		SDL_Point nextP4;
		nextP4.x = curP.x;
		nextP4.y = curP.y - 1;
		if(!(map->isWall(nextP4)) && !(visited[nextP4.x][nextP4.y]))
		{
			visited[nextP4.x][nextP4.y] = true;
			Node tmp;
			tmp.step = curNode.step + 1;
			for(int i=0; i<tmp.step; i++)
			{
				tmp.way.push_back(curNode.way[i]);
			}
			tmp.way.push_back(nextP4);
			
			if(nextP4.x == directpos.x && nextP4.y == directpos.y)
			{
				while(!go.empty())
				{
					go.pop();
				}
				for(int i=0; i<=tmp.step; i++)
				{
					go.push(tmp.way[i]);
				}
				break;
			}
			else
			{
				q.push(tmp);
			}
		}
	}
}


void Hunter::initHunter_Clips()               
{
    //Up
    Hunter_Clip[UP_1].x = 32;
    Hunter_Clip[UP_1].y = 96;
    Hunter_Clip[UP_1].w = 32;
    Hunter_Clip[UP_1].h = 32;

    Hunter_Clip[UP_2].x = 00;
    Hunter_Clip[UP_2].y = 96;
    Hunter_Clip[UP_2].w = 32;
    Hunter_Clip[UP_2].h = 32;

    Hunter_Clip[UP_3].x = 32;
    Hunter_Clip[UP_3].y = 96;
    Hunter_Clip[UP_3].w = 32;
    Hunter_Clip[UP_3].h = 32;
    
    Hunter_Clip[UP_4].x = 64;
    Hunter_Clip[UP_4].y = 96;
    Hunter_Clip[UP_4].w = 32;
    Hunter_Clip[UP_4].h = 32;

    // Down
    Hunter_Clip[DOWN_1].x = 32;
    Hunter_Clip[DOWN_1].y = 00;
    Hunter_Clip[DOWN_1].w = 32;
    Hunter_Clip[DOWN_1].h = 32;

    Hunter_Clip[DOWN_2].x = 00;
    Hunter_Clip[DOWN_2].y = 00;
    Hunter_Clip[DOWN_2].w = 32;
    Hunter_Clip[DOWN_2].h = 32;

    Hunter_Clip[DOWN_3].x = 32;
    Hunter_Clip[DOWN_3].y = 00;
    Hunter_Clip[DOWN_3].w = 32;
    Hunter_Clip[DOWN_3].h = 32;
    
    Hunter_Clip[DOWN_4].x = 64;
    Hunter_Clip[DOWN_4].y = 00;
    Hunter_Clip[DOWN_4].w = 32;
    Hunter_Clip[DOWN_4].h = 32;

    //Right
    Hunter_Clip[RIGHT_1].x = 32;
    Hunter_Clip[RIGHT_1].y = 64;
    Hunter_Clip[RIGHT_1].w = 32;
    Hunter_Clip[RIGHT_1].h = 32;

    Hunter_Clip[RIGHT_2].x = 00;
    Hunter_Clip[RIGHT_2].y = 64;
    Hunter_Clip[RIGHT_2].w = 32;
    Hunter_Clip[RIGHT_2].h = 32;

    Hunter_Clip[RIGHT_3].x = 32;
    Hunter_Clip[RIGHT_3].y = 64;
    Hunter_Clip[RIGHT_3].w = 32;
    Hunter_Clip[RIGHT_3].h = 32;
    
    Hunter_Clip[RIGHT_4].x = 64;
    Hunter_Clip[RIGHT_4].y = 64;
    Hunter_Clip[RIGHT_4].w = 32;
    Hunter_Clip[RIGHT_4].h = 32;

    //Left
    Hunter_Clip[LEFT_1].x = 32;
    Hunter_Clip[LEFT_1].y = 32;
    Hunter_Clip[LEFT_1].w = 32;
    Hunter_Clip[LEFT_1].h = 32;

    Hunter_Clip[LEFT_2].x = 00;
    Hunter_Clip[LEFT_2].y = 32;
    Hunter_Clip[LEFT_2].w = 32;
    Hunter_Clip[LEFT_2].h = 32;

    Hunter_Clip[LEFT_3].x = 32;
    Hunter_Clip[LEFT_3].y = 32;
    Hunter_Clip[LEFT_3].w = 32;
    Hunter_Clip[LEFT_3].h = 32;
    
    Hunter_Clip[LEFT_4].x = 64;
    Hunter_Clip[LEFT_4].y = 32;
    Hunter_Clip[LEFT_4].w = 32;
    Hunter_Clip[LEFT_4].h = 32;
}
