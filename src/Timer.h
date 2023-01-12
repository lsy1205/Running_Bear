//
// Created by 劉瑄穎 on 2021/1/9.
//

#ifndef TIMER_H
#define TIMER_H
#include"BasicObject.h"
#include<cstdio>
#include<string>
#include<sstream>
#include"Files.h"
#include"Mission.h"
#include"UserEvent.h"
#include"Game.h"

enum timerstate{TIMERSTART,TIMERPAUSE,TIMERUNPAUSE,TIMERSTOP};
class Timer:public BasicObject
{
private:
    //The time game continue
    Uint32 gametime;
    //The timepassed
    Uint32 timepassed;
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //Temp time for converting to seconds
    Uint32 restTime;

    int minute;
    int second;

    //The timer status
    bool mPaused;
    bool mStarted;

    //Timer Texture
    //SDL_Texture* timerTexture;

    //TimerColor
    SDL_Color timercolor;

    //Font
    TTF_Font * timerFont;

    //Load font
    bool loadTimerFont();

    //Free font
    void closetimer();

    //Gets the timer's time
    Uint32 getTicks()const;

    //TimeText
    std::stringstream timerText;

    //Mission
    Mission* currentMission;

    bool missionOngoing;
    void convert(MissionTypes&,int&);

    //Game
    Game* game;
public:
    //Initializes variables
    Timer();
    ~Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    bool handleEvents(SDL_Event& e);
    bool update();
};
#endif //SRC_TIMER_H
