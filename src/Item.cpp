//
// Created by 劉瑄穎 on 2021/1/2.
//

#include "Item.h"
#include "Files.h"
#include "Map.h"
#include <iostream>
using namespace std;

int Item::itemNumber = 0;
Item* createItem(ItemList& list)
{
    //a random Item
    switch (list)
    {
        case STAR:
            return  new Star;
        //case INVISIBLE_CLOAK:
        //    return new Invisible_Cloak;
        case POTION:
            return new Potion;
        case MEAT:
            return new Meat;
        case SHEET:
            return new Sheet;
        case LIBRARY:
            return new Library;
        case PE_TEACHER:
            return new PE_Teacher;
        case TA1:
            return new TeacherAssistant1;
        case TA2:
            return new TeacherAssistant2;
        case TA3:
            return new TeacherAssistant3;
        //case JOKE:
        //    return new Joke;
        //case CHEM_TEACHER:
        //    return new CHEM_Teacher;
        case WEBWORK1:
            return new WebWork1;
        case WEBWORK2:
            return new WebWork2;
        case WEBWORK3:
            return new WebWork3;
    }
}
Item::Item(std::string _name):BasicObject(_name)
{
    rectOnScreen.w = Map::getPixelWidth();
    rectOnScreen.h = Map::getPixelHeight();
}

void Item::setPixelPos(SDL_Point& pixelPos)   // center
{
    rectOnScreen.x = pixelPos.x - rectOnScreen.w / 2;
    rectOnScreen.y = pixelPos.y - rectOnScreen.h / 2;
}

ItemList Item::getItemType() const {return type;}

//Star
Star::Star():Item("Star")
{
    itemNumber++;
    texture = loadImage(STAR_IMAGE);
    rectOnTexture.x = 0;
    rectOnTexture.y = 0;
    rectOnTexture.w = 512;
    rectOnTexture.h = 512;
    type = STAR;
}
Star::~Star(){itemNumber--;}
bool Star::handleEvents(SDL_Event& e){return false;}
bool Star::update(){return false;}

//Invisible Cloak
/*Invisible_Cloak::Invisible_Cloak():Item("Invisible Cloak"){}
Invisible_Cloak::~Invisible_Cloak(){}
bool Invisible_Cloak::handleEvents(SDL_Event &e) {}
bool Invisible_Cloak::update() {}
*/

//Potion
Potion::Potion():Item("Potion")
{
    itemNumber++;
    texture = loadImage(POTION_IMAGE);
    rectOnTexture.x = 85;
    rectOnTexture.y = 88;
    rectOnTexture.w = 45;
    rectOnTexture.h = 45;
    type = POTION;
}
Potion::~Potion(){itemNumber--;}
bool Potion::handleEvents(SDL_Event &e){return false;}
bool Potion::update() {return false;}

//Meat
Meat::Meat():Item("Meat")
{
    itemNumber++;
    texture = loadImage(MEAT_IMAGE);
    rectOnTexture.x = 0;
    rectOnTexture.y = 50;
    rectOnTexture.w = 45;
    rectOnTexture.h = 42;
    type = MEAT;
}
Meat::~Meat(){itemNumber--;}
bool Meat::handleEvents(SDL_Event &e) {return false;}
bool Meat::update() {return false;}

//Sheet
Sheet::Sheet():Item("Sheet")
{
    itemNumber++;
    texture = loadImage(SHEET_IMAGE);
    rectOnTexture.x = 0;
    rectOnTexture.y = 0;
    rectOnTexture.w = 512;
    rectOnTexture.h = 512;
    type = SHEET;
}
Sheet::~Sheet(){itemNumber--;}
bool Sheet::handleEvents(SDL_Event &e) {return false;}
bool Sheet::update() {return false;}

//Library
Library::Library():Item("Library")
{
    itemNumber++;
    texture = loadImage(LIBRARY_IMAGE);
    rectOnTexture.x = 0;
    rectOnTexture.y = 0;
    rectOnTexture.w = 199;
    rectOnTexture.h = 250;
    type = LIBRARY;
}
Library::~Library(){itemNumber--;}
bool Library::handleEvents(SDL_Event &e) {return false;}
bool Library::update() {return false;}

//Physical education Teacher
PE_Teacher::PE_Teacher():Item("Physical Education Teacher")
{
    itemNumber++;
    texture = loadImage(PE_TEACHER_IMAGE);
    rectOnTexture.x = 0;
    rectOnTexture.y = 0;
    rectOnTexture.w = 260;
    rectOnTexture.h = 260;
    type = PE_TEACHER;
}
PE_Teacher::~PE_Teacher(){itemNumber--;}
bool PE_Teacher::handleEvents(SDL_Event &e) {return false;}
bool PE_Teacher::update() {return false;}

//TA1
TeacherAssistant1::TeacherAssistant1():Item("Teacher Assistant")
{
    itemNumber++;
    texture = loadImage(TA1_IMAGE);
    rectOnTexture.x = 0;
    rectOnTexture.y = 0;
    rectOnTexture.w = 796;
    rectOnTexture.h = 888;
    type = TA1;
}
TeacherAssistant1::~TeacherAssistant1(){itemNumber--;}
bool TeacherAssistant1::handleEvents(SDL_Event &e) {return false;}
bool TeacherAssistant1::update() {return false;}

//TA2
TeacherAssistant2::TeacherAssistant2():Item("Teacher Assistant")
{
    itemNumber++;
    texture = loadImage(TA2_IMAGE);
    rectOnTexture.x = 0;
    rectOnTexture.y = 0;
    rectOnTexture.w = 216;
    rectOnTexture.h = 122;
    type = TA2;
}
TeacherAssistant2::~TeacherAssistant2(){itemNumber--;}
bool TeacherAssistant2::handleEvents(SDL_Event &e) {return false;}
bool TeacherAssistant2::update() {return false;}

//TA3
TeacherAssistant3::TeacherAssistant3():Item("Teacher Assistant")
{
    itemNumber++;
    texture = loadImage(TA3_IMAGE);
    rectOnTexture.x = 0;
    rectOnTexture.y = 0;
    rectOnTexture.w = 200;
    rectOnTexture.h = 250;
    type = TA3;
}
TeacherAssistant3::~TeacherAssistant3(){itemNumber--;}
bool TeacherAssistant3::handleEvents(SDL_Event &e) {return false;}
bool TeacherAssistant3::update() {return false;}

/*
//Joke
Joke::Joke() {}
Joke::~Joke(){}
bool Joke::handleEvents(SDL_Event &e) {}
bool Joke::update() {}

//Chemistry Teacher
CHEM_Teacher::CHEM_Teacher():Item("Chemistry Teacher") {}
CHEM_Teacher::~CHEM_Teacher(){}
bool CHEM_Teacher::handleEvents(SDL_Event &e) {}
bool CHEM_Teacher::update() {}
*/

//WebWork
WebWork1::WebWork1():Item("WebWork")
{
    texture = loadImage(WEBWORK1_IMAGE);
    rectOnTexture.x = 0;
    rectOnTexture.y = 0;
    rectOnTexture.w = 900;
    rectOnTexture.h = 577;
    type = WEBWORK1;
}
WebWork1::~WebWork1(){}
bool WebWork1::handleEvents(SDL_Event &e) {return false;}
bool WebWork1::update() {return false;}

WebWork2::WebWork2():Item("WebWork")
{
    texture = loadImage(WEBWORK2_IMAGE);
    rectOnTexture.x = 0;
    rectOnTexture.y = 0;
    rectOnTexture.w = 512;
    rectOnTexture.h = 512;
    type = WEBWORK2;
}
WebWork2::~WebWork2(){}
bool WebWork2::handleEvents(SDL_Event &e) {return false;}
bool WebWork2::update() {return false;}

WebWork3::WebWork3():Item("WebWork")
{
    texture = loadImage(WEBWORK3_IMAGE);
    rectOnTexture.x = 0;
    rectOnTexture.y = 0;
    rectOnTexture.w = 368;
    rectOnTexture.h = 86;
    type = WEBWORK3;
}
WebWork3::~WebWork3(){}
bool WebWork3::handleEvents(SDL_Event &e) {return false;}
bool WebWork3::update() {return false;}