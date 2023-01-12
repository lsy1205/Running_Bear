#ifndef INCLUDE_SDL_H
#define INCLUDE_SDL_H

#ifdef _WIN32
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_bgi.h>
#endif

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "SDL_bgi.h"
#endif

#endif	// INCLUDE_SDL_H

#ifndef SDL_POINT_OPERATOR_ADD
#define SDL_POINT_OPERATOR_ADD

SDL_Point &operator+(SDL_Point &, SDL_Point &);
bool operator==(SDL_Point &, SDL_Point &);
bool operator<(SDL_Point &, SDL_Point &);

#endif