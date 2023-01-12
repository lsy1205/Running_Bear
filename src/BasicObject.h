#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include <iostream>
#include <string>
#include "includeSDL.h"

class BasicObject
{

	friend std::ostream &operator<<(std::ostream &, const BasicObject &);

public:
	BasicObject(std::string _name = "", bool _evEn = true, bool _upEn = true, bool _rdEn = true);
	BasicObject(BasicObject &);
	virtual ~BasicObject();

	inline void setEventEnable(bool _evEn)  { eventEnable  = _evEn; }
	inline void setUpdateEnable(bool _upEn) { updateEnable = _upEn; }
	inline void setRenderEnable(bool _rdEn) { renderEnable = _rdEn; }

	virtual bool handleEvents(SDL_Event &);	// return true if handled
	virtual bool update();

	inline void setTexture(SDL_Texture *tex) { texture = tex; }
	inline void setRectOnScreen(SDL_Rect &rect)  { rectOnScreen  = rect; }
	inline void setRectOnTexture(SDL_Rect &rect) { rectOnTexture = rect; }

	inline std::string getName() const { return name; }
	inline bool isEventEnable()  const { return eventEnable; }
	inline bool isUpdateEnable() const { return updateEnable; }
	inline bool isRenderEnable() const { return renderEnable; }
	inline SDL_Texture *getTexture() const { return texture; }
	inline const SDL_Rect *getRectOnScreenPtr()  const { return &rectOnScreen; }
	inline const SDL_Rect *getRectOnTexturePtr() const { return &rectOnTexture; }

protected:
	std::string name;
	bool eventEnable;
	bool updateEnable;
	bool renderEnable;

	SDL_Texture *texture;
	SDL_Rect rectOnScreen;
	SDL_Rect rectOnTexture;
};

std::ostream &operator<<(std::ostream &, const BasicObject &);

#endif	// BASICOBJECT_H
