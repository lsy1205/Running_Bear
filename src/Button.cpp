#include "includeSDL.h"
#include "Game.h"
#include "Button.h"
#include "UserEvent.h"
#include "Layer.h"
#include "Files.h"
#include <cmath>
using namespace std;

SDL_Rect Button::buttons[TOTAL_BUTTONS] = 
{
	SDL_Rect({ 495, 380, 290, 290}), //STARTS
	SDL_Rect({1085,  50, 145, 145}), //INTRO1
	SDL_Rect({ 615, 100,  30,  30}), //LEAVEINTRO
	SDL_Rect({ 980,  40, 100, 100}), //MISSION
	SDL_Rect({ 615, 100,  30,  30}), //LEAVEMISSION
	SDL_Rect({1130,  40, 100, 100}), //PAUSES
	SDL_Rect({ 370, 290, 145, 145}), //LEAVE
	SDL_Rect({ 570, 290, 145, 145}), //RESUMES
	SDL_Rect({ 770, 290, 145, 145}), //INTRO2
	SDL_Rect({ 615, 100,  30,  30}), //LEAVEINTRO2
	SDL_Rect({ 980, 420, 200, 200})  //OK
};

double operator^(SDL_Point center, SDL_Point mouse)
{
	double ans;
	ans = pow((pow(center.x-mouse.x, 2) + pow(center.y-mouse.y, 2)), 0.5);
	return ans;
	
}

Button::Button(button_type tmp) :
	BasicObject("Button"), type(tmp), UpdateReturnType(false), EventReturnType(false), Last(false)
{
	Center.x = buttons[tmp].x + buttons[tmp].w/2;
	Center.y = buttons[tmp].y + buttons[tmp].h/2;
	rectOnScreen = buttons[tmp];
	switch(tmp)
	{
		case STARTS:
			name = "Starts";
			texture = loadImage(START_BUTTON_IMAGE);
			break;
		case INTRO1:
			name = "Intro1";
			texture = loadImage(INTRO_BUTTON_IMAGE);
			break;
		case LEAVEINTRO:
			name = "LeaveIntro";
			texture = loadImage(LEAVE_BUTTON_IMAGE);
			break;
		case MISSION:
			name = "Mission";
			setRenderEnable(false);
			texture = loadImage(MISSION_BUTTON_IMAGE);
			break;
		case LEAVEMISSION:
			name = "LeaveMission";
			texture = loadImage(LEAVE_BUTTON_IMAGE);
			break;
		case PAUSES:
			name = "Pause";
			texture = loadImage(PAUSE_BUTTON_IMAGE);
			break;
		case LEAVE:
			name = "Leave";
			texture = loadImage(RESTART_BUTTON_IMAGE);
			break;
		case RESUMES:
			name = "Resumes";
			texture = loadImage(RESUME_BUTTON_IMAGE);
			break;
		case INTRO2:
			name = "Intro2";
			texture = loadImage(INTRO_BUTTON_IMAGE);
			break;
	    case LEAVEINTRO2:
	    	name = "LeaveIntro2";
			texture = loadImage(LEAVE_BUTTON_IMAGE);
			break;
		case OK:
			name = "OK";
			texture = loadImage(OK_BUTTON_IMAGE);
			break;
		default:
			texture = loadImage(SAMPLE_IMAGE);
	}
	Inside = false;
}

Button::~Button()
{
	
}

bool Button::handleEvents(SDL_Event &e)
{
	EventReturnType = false;
	int X,Y;
	SDL_GetMouseState( &X, &Y );
	SDL_Point mouse;
	mouse.x = X;
	mouse.y = Y;
	switch(type)
	{
		case STARTS:
			if(e.type == SDL_MOUSEMOTION)
			{
				EventReturnType = true;
				Inside = false;
				if((Center ^ mouse) <= buttons[STARTS].w/2 )
				{
					Inside = true;
				}
			}
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				
				if(Inside)
				{
					EventReturnType = true;
					createUserEvent(GAMESTATE_CHANGE, START, NULL, NULL); 
				}
			}
			break;
		case INTRO1:
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				
				Inside = false;
				if((Center ^ mouse) <= buttons[INTRO1].w/2 )
				{
					EventReturnType = true;
					Inside = true;
				}
				if(Inside)
				{
					Game &game = Game::GetGame();
					game.pushOverlayer(createLayer(L_INTRO, new BackGround(INTRO_IMAGE)));
					createUserEvent(GAMESTATE_CHANGE, PAUSE, NULL, NULL);
				}
			}
			break;
		case LEAVEINTRO:
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				
				Inside = false;
				if((Center ^ mouse) <= buttons[LEAVEINTRO].w/2 )
				{
					EventReturnType = true;
					Inside = true;
				}
				if(Inside)
				{
					Game &game = Game::GetGame();
					game.popTopOverlayer();
					createUserEvent(GAMESTATE_CHANGE, RESUME, NULL, NULL);
				}
			}
			break;
		case MISSION:
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				
				Inside = false;
				if((Center ^ mouse) <= buttons[INTRO1].w/2 )
				{
					EventReturnType = true;
					Inside = true;
				}
				if(Inside)
				{
					Game &game = Game::GetGame();
					if(current == MissionTotal)
						break;

					if(current == Mission1Type1)
						game.pushOverlayer(createLayer(L_MISSION, new BackGround(MISSION_1_1_IMAGE )));
					else if(current == Mission1Type2)
						game.pushOverlayer(createLayer(L_MISSION, new BackGround(MISSION_1_2_IMAGE )));
						
					else if(current == Mission1Type3)
						game.pushOverlayer(createLayer(L_MISSION, new BackGround(MISSION_1_3_IMAGE )));
						
					else if(current == Mission2Type1)
						game.pushOverlayer(createLayer(L_MISSION, new BackGround(MISSION_2_1_IMAGE )));
						
					else if(current == Mission2Type3)
						game.pushOverlayer(createLayer(L_MISSION, new BackGround(MISSION_2_2_IMAGE )));
				}
			}
			break;
		case LEAVEMISSION:
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				
				Inside = false;
				if((Center ^ mouse) <= buttons[LEAVEMISSION].w/2 )
				{
					EventReturnType = true;
					Inside = true;
				}
				if(Inside)
				{
					Game &game = Game::GetGame();
					game.popTopOverlayer();
				}
			}
			break;
		case PAUSES:
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				
				Inside = false;
				if((Center ^ mouse) <= buttons[PAUSE].w/2 )
				{
					EventReturnType = true;
					Inside = true;
				}
				if(Inside)
				{
					Game &game = Game::GetGame();
					game.pushOverlayer(createLayer(L_PAUSE, new BackGround(BOARD_IMAGE, SDL_Rect({300,200,680,320}))));
					createUserEvent(GAMESTATE_CHANGE, PAUSE, NULL, NULL);
				}
			}
			break;
		case LEAVE:
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				
				Inside = false;
				if((Center ^ mouse) <= buttons[LEAVE].w/2 )
				{
					EventReturnType = true;
					Inside = true;
				}
				if(Inside)
				{
					createUserEvent(GAMESTATE_CHANGE, STARTMENU, NULL, NULL);
				}
			}
			break;
		case RESUMES:
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				
				
				Inside = false;
				if((Center ^ mouse) <= buttons[RESUMES].w/2 )
				{
					EventReturnType = true;
					Inside = true;
				}
				if(Inside)
				{
					Game &game = Game::GetGame();
					game.popTopOverlayer();
					createUserEvent(GAMESTATE_CHANGE, RESUME, NULL, NULL);
				}
			}
			break;
		case INTRO2:
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				
				Inside = false;
				if((Center ^ mouse) <= buttons[INTRO2].w/2 )
				{
					EventReturnType = true;
					Inside = true;
				}
				if(Inside)
				{
					Game &game = Game::GetGame();
					game.pushOverlayer(createLayer(L_INTRO, new BackGround(INTRO_IMAGE )));
					createUserEvent(GAMESTATE_CHANGE, PAUSE, NULL, NULL);
				}
			}
			break;
		case LEAVEINTRO2:
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				
				Inside = false;
				if((Center ^ mouse) <= buttons[LEAVEINTRO2].w/2 )
				{
					EventReturnType = true;
					Inside = true;
				}
				if(Inside)
				{
					Game &game = Game::GetGame();
					game.popTopOverlayer();
					createUserEvent(GAMESTATE_CHANGE, RESUME, NULL, NULL);
				}
			}
			break;
		case OK:
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				
				Inside = false;
				if((Center ^ mouse) <= buttons[OK].w/2 )
				{
					EventReturnType = true;
					Inside = true;
				}
				if(Inside)
				{
					createUserEvent(GAMESTATE_CHANGE, STARTMENU, NULL, NULL);
				}
			}
			break;
			
	}
	return EventReturnType;
}

bool Button::update()
{
	if(type == STARTS && Inside)
		rectOnTexture = SDL_Rect( {512, 0, 512, 512} );
	else if (type == STARTS && !Inside)
		rectOnTexture = SDL_Rect( {0, 0, 512, 512} );
	else
		rectOnTexture = SDL_Rect( {0, 0, 214, 215} );

	if(type == MISSION)
	{
		current = Mission::getMission();
		if(isRenderEnable() and current == MissionTotal)
		{
			setRenderEnable(false);
			return true;
		}
		else if(!isRenderEnable() and current != MissionTotal)
		{
			setRenderEnable(true);
			return true;
		}
	}

	if(Last != Inside)
		UpdateReturnType = true;
	else
		UpdateReturnType = false;
	Last = Inside;
	return UpdateReturnType;

}

