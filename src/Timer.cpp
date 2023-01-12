//
// Created by 劉瑄穎 on 2021/1/9.
//

#include <iomanip>
#include "Timer.h"
using namespace std;

Timer::Timer():BasicObject("Timer"), gametime(600000), mPaused(false), mStarted(false), mStartTicks(0), mPausedTicks(0), timerFont(NULL),
timerText("0"), minute(10), second(0), missionOngoing(false), currentMission(NULL),game(&Game::GetGame())
{
    loadTimerFont();
    TTF_SetFontStyle(timerFont, TTF_STYLE_BOLD);

    timercolor = {0,0,0,255};
    rectOnScreen.w = 250;
    rectOnScreen.h = 120;
    rectOnScreen.x = 30;
    rectOnScreen.y = 15;
}

Timer::~Timer()
{
    closetimer();
}

void Timer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void Timer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}
void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}
void Timer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

bool Timer::handleEvents(SDL_Event& e)
{
    if(e.type == TIMERCHANGE)
    {
        //Start Timer
        if(e.user.code == TIMERSTART){this->start();}
        if(e.user.code == TIMERPAUSE){this->pause();}
        if(e.user.code == TIMERUNPAUSE){this->unpause();}
        if(e.user.code == TIMERSTOP){this->stop();}
        return true;
    }
    if(e.type == ITEM_USED and currentMission != NULL)
    {
        currentMission->handleEvents(e);
    }
    return false;
}
bool Timer::update()
{
    //Deal time
    this->timepassed = getTicks();
    timerText.str( "" );
    //To confirm text end at 00:00
    restTime = (gametime-timepassed)/1000.0f;
    if (mStarted && restTime > 1) restTime = (gametime-timepassed)/1000.0f;

    //Set output string
    minute = restTime/60;               //left minute
    second = restTime - minute*60;      //left second
    timerText << std::setw(2) << std::setfill('0') << minute << ":" ;
    if(second >= 1) timerText << std::setw(2) << std::setfill('0') << second;
    else timerText << "00";         //remain 00 at the end

    //Deal Mission1
    if(minute == 8 && second == 59 &&(!missionOngoing))
    {
        int tmp = game->rdEngine()%3;
        MissionTypes choice;
        convert(choice, tmp);
        currentMission = createMission(choice);
        missionOngoing = true;
    }
    //Mission1 Fail
    if(minute == 6 && missionOngoing)
    {
        gametime += 60000;
        if(currentMission!=NULL)
        {
            delete currentMission;
            currentMission = NULL;
        }
        missionOngoing = false;
    }

    //Deal Mission2
    if(minute == 5 && second ==59 && (!missionOngoing))
    {
        int tmp = game->rdEngine()%2+3;
        MissionTypes choice;
        convert(choice, tmp);
        currentMission = createMission(choice);
        missionOngoing = true;
    }

    //Mission2 Fail
    if(minute==2 && missionOngoing)
    {
        currentMission ->timeup();
        if(currentMission!=NULL)
        {
            delete currentMission;
            currentMission = NULL;
        }

        missionOngoing = false;
    }
    //Mission Success
    if(missionOngoing && currentMission->success)
    {
        if(currentMission!=NULL)
        {
            delete currentMission;
            currentMission = NULL;
        }
        missionOngoing = false;
    }

    //End Game
    if(gametime-timepassed<20)
    {
        //printf("end game\n");
        createUserEvent(GAMESTATE_CHANGE, END, new bool(true), NULL);
    }

    //Put Timertext to timertexture (uncertained due to renderer)
    SDL_Surface *surface = TTF_RenderText_Blended(timerFont, timerText.str().c_str(), timercolor);
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface for timer: %s", SDL_GetError());
        return 3;
    }

    texture = SDL_CreateTextureFromSurface(game->getRenderer(), surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture for timer: %s", SDL_GetError());
        return 3;
    }
    SDL_QueryTexture(texture, NULL, NULL, &rectOnTexture.w, &rectOnTexture.h);
    SDL_FreeSurface(surface);

    return true;
}


//private function
bool Timer::loadTimerFont()
{
    bool success = true;
    timerFont = TTF_OpenFont( "./media/font/octin_college_rg.ttf", 80 );
    if( timerFont == NULL )
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load timer font: %s\n", TTF_GetError());
        success = false;
    }
    else
    {
        //Set text color as black
        SDL_Color timertextColor = { 0, 0, 0, 255 };
    }
    return success;
}


Uint32 Timer::getTicks()const
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

void Timer::closetimer()
{
    //Free global font
    if(timerFont!=NULL)
    {
        TTF_CloseFont( timerFont );
        timerFont = NULL;
    }
}

void Timer::convert(MissionTypes& mission, int& tmp)
{
    switch(tmp)
    {
        case 0:
            mission = Mission1Type1;
            break;
        case 1:
            mission = Mission1Type2;
            break;
        case 2:
            mission = Mission1Type3;
            break;
        case 3:
            mission = Mission2Type1;
            break;
        case 4:
            mission = Mission2Type3;
            break;
    }
}

