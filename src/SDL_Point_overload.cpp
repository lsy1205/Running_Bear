#include "includeSDL.h"

SDL_Point &operator+(SDL_Point &a, SDL_Point &b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

bool operator==(SDL_Point &a, SDL_Point &b)
{
    if(a.x == b.x && a.y == b.y){return true;}
    else{return false;}
}

bool operator<(SDL_Point &a, SDL_Point &b)
{
    if(a.x < b.x){return true;}
    else if(a.x == b.x)
    {
        if (a.y < b.y)
            return true;
        else
            return false;
    }
    return false;
}