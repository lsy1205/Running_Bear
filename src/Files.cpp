#include "Files.h"
#include "Game.h"
using namespace std;

namespace Files
{
	string P_Images[IMAGE_COUNT]	= { "./media/image/image.txt",
										"./media/image/donate.png",
										"./media/image/startimage.png",
										"./media/image/background.jpg",
										"./media/image/Status.png",
										"./media/image/fail.png",
										"./media/image/success.png",
										"./media/image/Board.png",
										"./media/image/Hunter_32_32.png",
										"./media/image/Runner_56_56.png",
										"./media/image/StartButton_512_512.png",
										"./media/image/Intro.png",
										"./media/image/Pause.png",
										"./media/image/Mission.png",
										"./media/image/Resume.png",
										"./media/image/Leave.png",
										"./media/image/Restart.png",
										"./media/image/Ok.png",
										"./media/image/Introduction.png",
										"./media/image/SheetMission.png",
										"./media/image/LibraryMission.png",
										"./media/image/PEMission.png",
										"./media/image/DebugMission.png",
										"./media/image/WebworkMission.png",
										"./media/image/table.png",
										"./media/image/timerboard.png", //TIMER_BACKGROUND
										"./media/image/star.png", //STAR
										"./media/image/Item.png", //POTION
										"./media/image/Item.png", //MEAT
										"./media/image/sheet.png", //SHEET
										"./media/image/Library.png", //LIBRARY
										"./media/image/PE_teacher.png", //PE_TEACHER
										"./media/image/Miku.png", //TA1
										"./media/image/TA2.png", //TA2
										"./media/image/teacher.png", //TA3
										"./media/image/dt.png", //WEBWORK1
										"./media/image/parenthesis.png", //WEBWORK2
										"./media/image/webwork.png", //WEBWORK3
										"./media/image/wall.png",
										"./media/image/StrengthBar.png",
										};

	//string P_Fonts[FONT_COUNT]	= { "../media/font/fonts.txt",};

	string P_Musics[MUSIC_COUNT]	= { "./media/music/music.txt",
										"./media/music/cozy_BGM.mp3",
										"./media/music/runner_sprint.mp3",
										};

	string P_Maps[MAP_COUNT]		= { "./media/map/map.txt",
										"./media/map/map01.csv",
										"./media/map/map02.csv",
										"./media/map/map03.csv",
										"./media/map/map04.csv",
										};

	SDL_Texture *loadedImage[IMAGE_COUNT] = {NULL};
	//TTF_Font *loadedFont[FONT_COUNT] = {NULL};
	Mix_Music *loadedMusic[MUSIC_COUNT] = {NULL};
}

SDL_Texture *loadImage(Images index)
{
	if(Files::loadedImage[index])
		return Files::loadedImage[index];

	SDL_Texture *texture = NULL;
	SDL_Surface *surface = IMG_Load(Files::P_Images[index].c_str());
	if(!surface)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Load: Failed to load image %s: %s", Files::P_Images[index].c_str(), IMG_GetError());
		return NULL;
	}
	texture = SDL_CreateTextureFromSurface(Game::GetGame().getRenderer(), surface);
	SDL_FreeSurface(surface);
	if(!texture)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture for image %s: %s", Files::P_Images[index].c_str(), SDL_GetError());
		return NULL;
	}
	Files::loadedImage[index] = texture;
	return texture;
}
/*
TTF_Font *loadFont(Fonts index, int ptSize)
{
	if(Files::loadedFont[index])
		return Files::loadedFont[index];

	TTF_Font *font = TTF_OpenFont(Files::P_Fonts[index].c_str(), ptSize);
	if(!font)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_OpenFont: Failed to load font %s: %s", Files::P_Fonts[index].c_str(), TTF_GetError());
		return NULL;
	}
	Files::Files::loadedFont[index] = font;
	return font;
}
*/
Mix_Music *loadMusic(Musics index)
{
	if(Files::loadedMusic[index])
		return Files::loadedMusic[index];

	Mix_Music *music = Mix_LoadMUS(Files::P_Musics[index].c_str());
	if(!music)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Mix_loadMus: Failed to load music %s: %s", Files::P_Musics[index].c_str(), Mix_GetError());
		return NULL;
	}
	Files::loadedMusic[index] = music;
	return music;
}

ifstream *openMapFile(Maps index)
{
	return new ifstream(Files::P_Maps[index], ios_base::in);
}

void freeImage(Images index)
{
	if(Files::loadedImage[index])
	{
		SDL_DestroyTexture(Files::loadedImage[index]);
		Files::loadedImage[index] = NULL;
	}
}
/*
void freeFont(Fonts index)
{
	if(Files::loadedFont[index])
	{
		TTF_CloseFont(Files::loadedFont[index]);
		Files::loadedFont[index] = NULL;
	}
}
*/
void freeMusic(Musics index)
{
	if(Files::loadedMusic[index])
	{
		Mix_FreeMusic(Files::loadedMusic[index]);
		Files::loadedMusic[index] = NULL;
	}
}

void freeAllFiles()
{
	for(SDL_Texture *image : Files::loadedImage)
		if(image)
		{
			SDL_DestroyTexture(image);
			image = NULL;
		}

	for(Mix_Music *music : Files::loadedMusic)
		if(music)
		{
			Mix_FreeMusic(music);
			music = NULL;
		}
}
