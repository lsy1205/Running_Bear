#ifndef RUNNER_H
#define RUNNER_H
#include "includeSDL.h"
#include "BasicObject.h"
#include <iostream>
extern int mode;

class Item;
class Map;

enum character_list{BEAR};
enum face {DOWN,LEFT,RIGHT,UP};
class Runner:public BasicObject{
    friend class Backpack;
    friend class StrengthBar;
    friend std::ostream & operator<<(std::ostream &,const Runner&);
private:
    std::string username;
    face direction;             // character direction(used for render different graphs)
    SDL_Point PixelPos;         // small grid on map
    SDL_Point MapPos;           // big grid on map
    int velocity_x;             // runner x velocity
    int velocity_y;             // runner y velocity
    int velocity;               // the velocity when pressing button
    double strength;            // the strength of runner
    bool sprint;                // check is sprinting or not
    int width;                  // rectangle width (for collision box)
    int height;                 // rectangle height (for collision box)
    int updateRate;             // different update rate according to running or walking
    Item* backpack;             // item in hand
    Map* map;
    static Runner* runnerInstance;

    inline void operator++(int);// operator overloading to update current position
    inline void use(Item*);

    static const int gridWidth; // the width of grid on map
    static const int gridHeight;// the height of grid on map

public:
    Runner(SDL_Point&, SDL_Point&, character_list = BEAR);
    Runner(Runner&);
    ~Runner();
    bool handleEvents(SDL_Event &e);
    bool update();
    void move();
    bool checkbackpack();               //check whether the backpack is empty
    bool collisionBox(SDL_Rect&);
    void checkCollision();
    void initclips();
    void setMapPos(SDL_Point&);         // set big grid on map
    void setPixelPos(SDL_Point&);       // set small grid on map (center)
    //void setScreenPos(SDL_Point&);      // set rectOnScreen
    void updateMapPos(SDL_Point&);      // ask map for big grid and update MapPos
    SDL_Point getMapPos() const;        // get big grid on map
    virtual SDL_Point getPixelPos()const;       // get small grid on map (center)
    int getStrength()const;  // get strength
    inline static Runner& getRunner() { return *runnerInstance; }
};


std::ostream & operator<<(std::ostream& output,const Runner& runner);

class Backpack : public BasicObject
{
public:
    Backpack();
    ~Backpack();
    bool update();
private:
    Item *item;
};

class StrengthBar : public BasicObject
{
public:
    StrengthBar();
    ~StrengthBar();
    bool update();
};

#endif //RUNNER_H
