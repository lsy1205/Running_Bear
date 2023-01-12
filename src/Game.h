#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <random>
#include "includeSDL.h"
#include "Layer.h"

class Map;

enum GameState
{
	STARTMENU,
	LOADING,
	GAME,
	PAUSE,
	END,
	// for UserEvent
	START,
	RESUME,
};

class Game
{
public:
	Game(std::string, unsigned int _width = 1280, unsigned int _height = 720, unsigned int _frameRate = 60);
	~Game();

	bool Init();				// initialize the game
	void Start(unsigned int);	// start the game
	void HandleEvents();		// handle changes triggered by SDL events
	void Update();				// update all changes
	void Render();				// render to the screen
	void Quit();				// exit the game

	void pushLayer(Layer *);		// push new layer to layers
	void pushOverlayer(Layer *);	// push new overlayer to layers
	void popLayer(Layer *);			// pop layer from layers
	void popOverlayer(Layer *);		// pop overlayer from layers
	void popTopOverlayer();			// pop top pverlayer from layers
	void popAllLayers();
	inline void setGameMap(Map *_map) { gameMap = _map; }

	inline bool isRunning() const { return running; }
	inline std::string getName() const { return name; }
	inline unsigned int getWidth()  const { return width; }
	inline unsigned int getHeight() const { return height; }
	inline unsigned int getFrameRate() const { return frameRate; }
	inline SDL_Renderer *getRenderer() { return renderer; }
	inline unsigned int getLayersSize() const { return layers.size(); }

	std::mt19937_64 rdEngine;

	inline static Game &GetGame() { return *s_gameInstance; }

private:
	std::string name;
	unsigned int width;
	unsigned int height;
	unsigned int frameRate;
	bool running;
	GameState state;

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;

	std::vector<Layer*> layers;
	unsigned int layerInsertIndex;
	Map *gameMap;
	Mix_Music *bgm;

	unsigned int startTime;
	unsigned int frameCount;
	unsigned int eventStart;
	unsigned int duration;
	int puasedCount;
	bool started;

	static Game *s_gameInstance;
};

#endif	// GAME_H