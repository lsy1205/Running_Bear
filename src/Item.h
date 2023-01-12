//
// Created by 劉瑄穎 on 2021/1/2.
//

#ifndef ITEM_H
#define ITEM_H
#include"includeSDL.h"
#include"BasicObject.h"
#include"UserEvent.h"

enum ItemList{BAD_ITEM, STAR, INVISIBLE_CLOAK, POTION, MEAT, SHEET, LIBRARY, PE_TEACHER,
        TA1, TA2, TA3 , JOKE, CHEM_TEACHER, WEBWORK1, WEBWORK2, WEBWORK3, TOTAL_ITEM };

class Item :public BasicObject
{
    friend class Runner;
protected:
    ItemList type;
    SDL_Point PixelPos;         // small grid on map
    static int itemNumber;
public:
    Item(std::string _name = "");
    virtual bool handleEvents(SDL_Event& e)=0;
    virtual bool update()=0;
    ItemList getItemType()const;
    void setPixelPos(SDL_Point&);       // set small grid on map (center)
};

Item* createItem(ItemList& list);

class Star:public Item
{
public:
    Star();
    ~Star();
    bool handleEvents(SDL_Event& e);
    bool update();
};

/*
class Invisible_Cloak:public Item
{
public:
    Invisible_Cloak();
    ~Invisible_Cloak();
    bool handleEvents(SDL_Event& e);
    bool update();
};
*/

class Potion:public Item
{
public:
    Potion();
    ~Potion();
    bool handleEvents(SDL_Event& e);
    bool update();
};

class Meat:public Item
{
public:
    Meat();
    ~Meat();
    bool handleEvents(SDL_Event& e);
    bool update();
};

class Sheet:public Item
{
public:
    Sheet();
    ~Sheet();
    bool handleEvents(SDL_Event& e);
    bool update();
};

class Library:public Item
{
public:
    Library();
    ~Library();
    bool handleEvents(SDL_Event& e);
    bool update();
};
class PE_Teacher:public Item
{
public:
    PE_Teacher();
    ~PE_Teacher();
    bool handleEvents(SDL_Event& e);
    bool update();
};

class TeacherAssistant1:public Item
{
public:
    TeacherAssistant1();
    ~TeacherAssistant1();
    bool handleEvents(SDL_Event& e);
    bool update();
};

class TeacherAssistant2:public Item
{
public:
    TeacherAssistant2();
    ~TeacherAssistant2();
    bool handleEvents(SDL_Event& e);
    bool update();
};

class TeacherAssistant3:public Item
{
public:
    TeacherAssistant3();
    ~TeacherAssistant3();
    bool handleEvents(SDL_Event& e);
    bool update();
};

/*
class Joke:public Item
{
public:
    Joke();
    ~Joke();
    bool handleEvents(SDL_Event& e);
    bool update();
};

class CHEM_Teacher:public Item
{
public:
    CHEM_Teacher();
    ~CHEM_Teacher();
    bool handleEvents(SDL_Event& e);
    bool update();
};
 */

class WebWork1:public Item
{
public:
    WebWork1();
    ~WebWork1();
    bool handleEvents(SDL_Event& e);
    bool update();
};

class WebWork2:public Item
{
public:
    WebWork2();
    ~WebWork2();
    bool handleEvents(SDL_Event& e);
    bool update();
};

class WebWork3:public Item
{
public:
    WebWork3();
    ~WebWork3();
    bool handleEvents(SDL_Event& e);
    bool update();
};

#endif //ITEM_H
