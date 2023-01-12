#include <iostream>
#include <string>
#include "includeSDL.h"
#include "Game.h"
#include "Layer.h"
#include "Mission.h"
#include "UserEvent.h"
#include "Item.h"
#include "Map.h" 
using namespace std;

MissionTypes Mission::curMission = MissionTotal;

Mission :: Mission(string _name)
{
	total = 0;
	success = false;
}
Mission :: ~Mission() 
{ }

Mission *createMission(MissionTypes choice)
{
	
	switch (choice) 
	{
		case Mission1Type1:
			return new Mission1_1();
		case Mission1Type2:
			return new Mission1_2();
		case Mission1Type3:
			return new Mission1_3();
		case Mission2Type1:
			return new Mission2_1();
		case Mission2Type3:
			return new Mission2_3();			

	}
}
void Mission::timeup()
{
	if(!success)
	{
		Map &map = Map::getMap();
		map.addHunter(10*(3-total)); 
	}
}
Mission1_1 :: Mission1_1 ()
 : Mission("Mission 1 ")
{
	curMission = Mission1Type1;
	Map &map = Map::getMap();	
	bool SetSuccess = false;
	SDL_Point SetPos;
	while(!SetSuccess)
	{
		Game &game = Game::GetGame();
		SetPos.x = game.rdEngine()%(map.getRowNum()); 
		SetPos.y = game.rdEngine()%(map.getColNum());
		if(map.isSpace(SetPos)) 
		{
			SetSuccess = true;
		} 
	}
	
	sheet = new Sheet;
	map.placeItem(SetPos, sheet);

}
Mission1_1 :: ~Mission1_1 ()
{
	delete sheet;
	sheet = NULL;
	curMission = MissionTotal;
}
bool Mission1_1 :: handleEvents(SDL_Event &e)
{
	
	if(e.type == ITEM_USED) 
		{
			//printf("item used\n");
			if(e.user.code == SHEET)
			{
				//printf("item1-1 used\n");
				sheet = NULL;
				success = true;
				return true;
			}
		}
	else return false;
}


Mission1_2 :: Mission1_2 ()
 : Mission("Mission 1 ")
{
	curMission = Mission1Type2;
	Map &map = Map::getMap();
	
	bool SetSuccess = false;
	SDL_Point SetPos;
	while(!SetSuccess)
	{
		Game &game = Game::GetGame();
		SetPos.x = game.rdEngine()%(map.getRowNum()); 
		SetPos.y = game.rdEngine()%(map.getColNum());
		if(map.isSpace(SetPos)) 
		{
			SetSuccess = true;
		} 
	}	
	lib = new Library;
	map.placeItem(SetPos, lib);

}

Mission1_2 :: ~Mission1_2 ()
{
	delete lib;
	lib = NULL;
	curMission = MissionTotal;
}

bool Mission1_2 :: handleEvents(SDL_Event &e)
{
	
	if(e.type == ITEM_USED) 
		{
			//printf("item used\n");
			if(e.user.code == LIBRARY)
			{
				//printf("item1-2 used\n");
				lib = NULL;
				success = true;
				return true;
			}
		}
	return false;
}


Mission1_3 :: Mission1_3 ()
 : Mission("Mission 1 ")
{
	curMission = Mission1Type3;
	Map &map = Map::getMap();
	
	bool SetSuccess = false;
	SDL_Point SetPos;
	while(!SetSuccess)
	{
		Game &game = Game::GetGame();
		SetPos.x = game.rdEngine()%(map.getRowNum()); 
		SetPos.y = game.rdEngine()%(map.getColNum());
		if(map.isSpace(SetPos)) 
		{
			SetSuccess = true;
		} 
	}
	pe = new PE_Teacher;
	map.placeItem(SetPos, pe);

}

Mission1_3 :: ~Mission1_3 ()
{
	delete pe;
	pe = NULL;
	curMission = MissionTotal;
}

bool Mission1_3 :: handleEvents(SDL_Event &e)
{
	
	if(e.type == ITEM_USED) 
		{
			//printf("item used\n");
			if(e.user.code == PE_TEACHER)
			{
				//printf("item1-3 used\n");
				pe = NULL;
				success = true;
				return true;
			}
		}
	else return false;
}



Mission2_1 :: Mission2_1 ()
 : Mission("Mission 2 ")
{
	total = 0;
	curMission = Mission2Type1;
	Map &map = Map::getMap();
	
	bool SetSuccess1 = false;
	SDL_Point SetPos1;
	while(!SetSuccess1)
	{
		Game &game = Game::GetGame();
		SetPos1.x = game.rdEngine()%(map.getRowNum()); 
		SetPos1.y = game.rdEngine()%(map.getColNum());
		if(map.isSpace(SetPos1)) 
		{
			SetSuccess1 = true;
		} 
	}
	ta1 = new TeacherAssistant1;
	map.placeItem(SetPos1, ta1);
	
	bool SetSuccess2 = false;
	SDL_Point SetPos2;
	while(!SetSuccess2)
	{
		Game &game = Game::GetGame();
		SetPos2.x = game.rdEngine()%(map.getRowNum()); 
		SetPos2.y = game.rdEngine()%(map.getColNum());
		if(map.isSpace(SetPos2)) 
		{
			SetSuccess2 = true;
		} 
	}	
	ta2 = new TeacherAssistant2;
	map.placeItem(SetPos2, ta2);
	
	bool SetSuccess3 = false;
	SDL_Point SetPos3;
	while(!SetSuccess3)
	{
		Game &game = Game::GetGame();
		SetPos3.x = game.rdEngine()%(map.getRowNum()); 
		SetPos3.y = game.rdEngine()%(map.getColNum());
		if(map.isSpace(SetPos3)) 
		{
			SetSuccess3 = true;
		} 
	}	
	ta3 = new TeacherAssistant3;
	map.placeItem(SetPos3, ta3);

}

Mission2_1 :: ~Mission2_1 ()
{
	delete ta1;
	ta1 = NULL;
	delete ta2;
	ta2 = NULL;
	delete ta3;
	ta3 = NULL;
	curMission = MissionTotal;
}

bool Mission2_1 :: handleEvents(SDL_Event &e)
{
	
	if(e.type == ITEM_USED) 
		{
			if(e.user.code == TA1)
			{
				ta1 = NULL;
				total++;
			}
			if(e.user.code == TA2)
			{
				ta2 = NULL;
				total++;
			}
			if(e.user.code == TA3)
			{
				ta3 = NULL;
				total++;
			}
		}
	if(total == 3)
	{
		success = true;
	}
	return true;
}

Mission2_3 :: Mission2_3 ()
 : Mission("Mission 2 ")
{
	total = 0;
	curMission = Mission2Type3;
	Map &map = Map::getMap();
	
	bool SetSuccess1 = false;
	SDL_Point SetPos1;
	while(!SetSuccess1)
	{
		Game &game = Game::GetGame();
		SetPos1.x = game.rdEngine()%(map.getRowNum()); 
		SetPos1.y = game.rdEngine()%(map.getColNum());
		if(map.isSpace(SetPos1)) 
		{
			SetSuccess1 = true;
		} 
	}
	
	web1 = new WebWork1;
	map.placeItem(SetPos1, web1);
	
	bool SetSuccess2 = false;
	SDL_Point SetPos2;
	while(!SetSuccess2)
	{
		Game &game = Game::GetGame();
		SetPos2.x = game.rdEngine()%(map.getRowNum()); 
		SetPos2.y = game.rdEngine()%(map.getColNum());
		if(map.isSpace(SetPos2)) 
		{
			SetSuccess2 = true;
		} 
	}
	
	web2 = new WebWork2;
	map.placeItem(SetPos1, web2);
	
	bool SetSuccess3 = false;
	SDL_Point SetPos3;
	while(!SetSuccess3)
	{
		Game &game = Game::GetGame();
		SetPos3.x = game.rdEngine()%(map.getRowNum()); 
		SetPos3.y = game.rdEngine()%(map.getColNum());
		if(map.isSpace(SetPos3)) 
		{
			SetSuccess3 = true;
		} 
	}
	
	web3 = new WebWork3;
	map.placeItem(SetPos1, web3);

}

Mission2_3 :: ~Mission2_3 ()
{
	delete web1;
	web1 = NULL;
	delete web2;
	web2 = NULL;
	delete web3;
	web3 = NULL;
	curMission = MissionTotal;
}

bool Mission2_3 :: handleEvents(SDL_Event &e)
{
	
	if(e.type == ITEM_USED) 
		{
			if(e.user.code == WEBWORK1)
			{
				web1 = NULL;
				total++;
			}
			if(e.user.code == WEBWORK2)
			{
				web2 = NULL;
			    total++;
			}
			if(e.user.code == WEBWORK3)
			{
				web3 = NULL;
			    total++;
			}
		}
	if(total == 3)
	{
		success = true;
	}
	return true;
}

