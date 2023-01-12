#include "UserEvent.h"

const unsigned int GAMESTATE_CHANGE = SDL_RegisterEvents(1);
const unsigned int ITEM_USED = SDL_RegisterEvents(1);
const unsigned int TIMERCHANGE = SDL_RegisterEvents(1);

void createUserEvent(unsigned int _type, unsigned int _code, void *_data1 = NULL, void *_data2 = NULL)
{
	SDL_Event event;
	SDL_zero(event);
	event.type = _type;
	event.user.code = _code;
	event.user.data1 = _data1;
	event.user.data2 = _data2;
	SDL_PushEvent(&event);
}