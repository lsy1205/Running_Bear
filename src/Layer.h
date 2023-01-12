#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include <string>
#include "includeSDL.h"
#include "BasicObject.h"
#include "Files.h"

enum Layers
{
	L_STARTMENU,
	L_STATUS,
	L_INTRO,
	L_MISSION,
	L_PAUSE,
	L_END,
	L_LOADING,
	L_MAP_GROUND,
	L_MAP_FRONT,
	L_CHARACTER,
};

class Layer
{
public:
	Layer(std::string _name = "", int _texWidth = -1, int _texHeight = -1);
	virtual ~Layer();

	void free();							// destroy all textures and surfaces
	virtual bool handleEvents(SDL_Event &);	// handle events on current layer
	virtual void update();					// update changes on current layer
	virtual void render();					// render elements to mainTexture
	inline void setActive(bool _active) { active = _active; }
	inline void setRectViewPos(SDL_Point &_pos) { rectViewPort.x = _pos.x; rectViewPort.y = _pos.y; }

	void pushElement(BasicObject *);
	void popElement(BasicObject *);
	Layer &operator+(BasicObject *);
	Layer &operator-(BasicObject *);

	inline std::string getName() const { return name; }
	inline bool isActive() const { return active; }
	inline SDL_Texture *getTexture() { return mainTexture; }
	inline SDL_Rect *getRectViewPortPtr() { return &rectViewPort; }
	inline unsigned int getElementsSize() const { return elements.size(); }

protected:
	std::string name;
	bool active;
	bool changed;
	int renderCount;

	SDL_Texture* mainTexture;
	SDL_Rect rectViewPort;
	std::vector<BasicObject*> elements;
};

class BackGround : public BasicObject
{
public:
	BackGround(Images, SDL_Rect = SDL_Rect({0, 0, -1, -1}));
	~BackGround();
	
};

Layer *createLayer(Layers index, BackGround *background = NULL);

#endif	// LAYER_H
