#ifndef FILES_H
#define FILES_H

#include <string>
#include <fstream>
#include "includeSDL.h"

enum Images
{
	SAMPLE_IMAGE,
	DONATE_IMAGE,
	START_IMAGE,
	BACKGROUND_IMAGE,
	STATUS_IMAGE,
	END_LOSE_IMAGE,
	END_WIN_IMAGE,
	BOARD_IMAGE,
	HUNTER_IMAGE,
	RUNNER_IMAGE,
	START_BUTTON_IMAGE,
	INTRO_BUTTON_IMAGE,
	PAUSE_BUTTON_IMAGE,
	MISSION_BUTTON_IMAGE,
	RESUME_BUTTON_IMAGE,
	LEAVE_BUTTON_IMAGE,
	RESTART_BUTTON_IMAGE,
	OK_BUTTON_IMAGE,
	INTRO_IMAGE,
	MISSION_1_1_IMAGE,
	MISSION_1_2_IMAGE,
	MISSION_1_3_IMAGE,
	MISSION_2_1_IMAGE,
	MISSION_2_2_IMAGE,
	MISSION_DEFAULT_IMAGE,
	TIMER_BACKGROUND_IMAGE,
	STAR_IMAGE,
	POTION_IMAGE,
	MEAT_IMAGE,
	SHEET_IMAGE,
	LIBRARY_IMAGE,
	PE_TEACHER_IMAGE,
	TA1_IMAGE,
	TA2_IMAGE,
	TA3_IMAGE,
	WEBWORK1_IMAGE,
	WEBWORK2_IMAGE,
	WEBWORK3_IMAGE,
	WALL_IMAGE,
	STRENGTH_IMAGE,
	IMAGE_COUNT,
};
/*
enum Fonts
{
	SAMPLE_FONT,
	FONT_COUNT,
};
*/
enum Musics
{
	SAMPLE_MUSIC,
	BGM_MUSIC,
	SPRINT_MUSIC,
	MUSIC_COUNT,
};

enum Maps
{
	SAMPLE_MAP,
	MAP_01,
	MAP_02,
	MAP_03,
	MAP_04,
	MAP_COUNT,
};

SDL_Texture *loadImage(Images index);
//TTF_Font *loadFont(Fonts index, int ptSize);
Mix_Music *loadMusic(Musics index);
std::ifstream *openMapFile(Maps index);

void freeImage(Images index);
//void freeFont(Fonts index);
void freeMusic(Musics index);
void freeAllFiles();

namespace Files
{
	extern std::string P_Images[IMAGE_COUNT];
	//extern std::string P_Fonts[FONT_COUNT];
	extern std::string P_Musics[MUSIC_COUNT];
	extern std::string P_Maps[MAP_COUNT];

	extern SDL_Texture *loadedImage[IMAGE_COUNT];
	//extern TTF_Font *loadedFont[FONT_COUNT];
	extern Mix_Music *loadedMusic[MUSIC_COUNT];
};

#endif	// FILES_H
