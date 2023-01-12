#include <iostream>
#include <cstdlib>
#include <string>
#include "includeSDL.h"
#include "Game.h"
using namespace std;

string PROJECTNAME = "RunningBareBear";

int mode = 1;
int main(int argc, char* argv[])
{
	atexit(SDL_Quit);	// for safe exit

	Game game(PROJECTNAME, 1280, 720, 60);
	if(!game.Init())
	{
		cout << "Game init failed." << endl;
		return -1;
	}
	//cout << "Game started!" << endl;

	game.Start(SDL_GetTicks());
	while(game.isRunning())
	{
		game.HandleEvents();
		game.Update();
		game.Render();
	}
	game.Quit();
	//cout << "Game ended!" << endl;

	return 0;
}
