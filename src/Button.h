#ifndef BUTTON_H
#define BUTTON_H
#include <string>
#include "includeSDL.h"
#include "BasicObject.h"
#include "Mission.h"
enum button_type
{
	STARTS, INTRO1, LEAVEINTRO, MISSION, LEAVEMISSION, PAUSES, LEAVE, RESUMES, INTRO2, LEAVEINTRO2, OK, TOTAL_BUTTONS
};

class Button : public BasicObject
{
	bool Inside;
	bool EventReturnType;
	bool UpdateReturnType;
	bool Last;
	bool changed;
	SDL_Point Center;
	MissionTypes current;
	button_type type;
public:
	static SDL_Rect buttons[TOTAL_BUTTONS];
	bool handleEvents(SDL_Event &e);
	bool update();
	Button(button_type tmp);
	~Button();
	
};

double operator ^(SDL_Point, SDL_Point);

#endif
