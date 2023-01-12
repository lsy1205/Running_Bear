#ifndef MISSION_H
#define MISSION_H
#include "Item.h"
#include <vector>
#include <string>
#include "includeSDL.h"


enum MissionTypes
{
	Mission1Type1, Mission1Type2, Mission1Type3,
	Mission2Type1, Mission2Type3,
	MissionTotal
};

class Mission
{
	friend class Timer;
protected:
	int total;
	bool success;
	static MissionTypes curMission;
public:
	Mission(std::string _name = "");
	virtual ~Mission();
	virtual bool handleEvents(SDL_Event &) = 0;
	void timeup();
	//inline bool isSuccess() const { return success; }
	inline static MissionTypes &getMission() { return curMission; }

		
};
class Mission1_1 : public Mission
{
	Sheet* sheet;
public:
	Mission1_1();
	~Mission1_1();
	bool handleEvents(SDL_Event &);
};
class Mission1_2 : public Mission
{
	Library* lib;
public:
	Mission1_2();
	~Mission1_2();
	bool handleEvents(SDL_Event &);
};
class Mission1_3 : public Mission
{
	PE_Teacher* pe;
public:
	Mission1_3();
	~Mission1_3();
	bool handleEvents(SDL_Event &);
};
class Mission2_1 : public Mission
{
	TeacherAssistant1* ta1;
	TeacherAssistant2* ta2;
	TeacherAssistant3* ta3;
public:
	Mission2_1();
	~Mission2_1();
	bool handleEvents(SDL_Event &);
};
 
class Mission2_3 : public Mission
{
	WebWork1* web1;
	WebWork2* web2;
	WebWork3* web3;
public:
	Mission2_3();
	~Mission2_3();
	bool handleEvents(SDL_Event &);
};

Mission *createMission(MissionTypes choice);


#endif
