#include "BasicObject.h"
using namespace std;

BasicObject::BasicObject(string _name, bool _evEn, bool _upEn, bool _rdEn)
 : name(_name), eventEnable(_evEn), updateEnable(_upEn), renderEnable(_rdEn),
   texture(NULL), rectOnScreen({0, 0, 0 ,0}), rectOnTexture({0, 0, 0, 0})
{}

BasicObject::BasicObject(BasicObject &_from)
 : name(_from.name), eventEnable(_from.eventEnable), updateEnable(_from.updateEnable), renderEnable(_from.renderEnable),
   texture(_from.texture), rectOnScreen(_from.rectOnScreen), rectOnTexture(_from.rectOnTexture)
{}

BasicObject::~BasicObject()
{}

bool BasicObject::handleEvents(SDL_Event &event)	// return true if handled
{ return false; }

bool BasicObject::update()
{ return false; }

ostream &operator<<(ostream &output, const BasicObject &obj)
{
	output << obj.name << "\nEvEn: " << obj.eventEnable << " UpEn: " << obj.updateEnable << " ReEn: " << obj.renderEnable << "\nTexture: " << obj.texture << "\n"
		<< "rectOnScreen (x, y, w, h): (" << obj.rectOnScreen.x  << ", " << obj.rectOnScreen.y  << ", " << obj.rectOnScreen.w  << ", " << obj.rectOnScreen.h  << ")\n"
		<< "rectOnTexture(x, y, w, h): (" << obj.rectOnTexture.x << ", " << obj.rectOnTexture.y << ", " << obj.rectOnTexture.w << ", " << obj.rectOnTexture.h << ")\n";
	return output;
}
