/*
 * SoarBrick.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef SOARBRICK_H_
#define SOARBRICK_H_

class SoarCommunicator;

#include "SoarDevice.h"
#include "comm/CommStructs.h"

#include "sml_Client.h"
#include "WMUtil.h"
using namespace sml;

class BrickButton{
public:
	BrickButton(std::string name, int mask)
	:name(name), mask(mask), id(0), curState(false), prevState(false){

	}

	~BrickButton(){
		if(id){
			id->DestroyWME();
			id = 0;
		}
	}

	void setState(int buttonState){
		curState = ((buttonState & mask) > 0);
	}

	void updateInputLink(Identifier* parentId);

	uint getDeviceType(){
		return 0;
	}

private:
	std::string name;
	int mask;

	Identifier* id;

	bool curState;
	bool prevState;
};

/*****************************************
 *
 * Brick
 *
 *****************************************/

#include <vector>
#include <map>
#include <string>

typedef std::vector<BrickButton*> ButtonVector;
typedef ButtonVector::iterator ButtonVectorIt;

typedef std::map<std::string, int> LedMap;
typedef LedMap::iterator LedMapIt;

class Brick : public SoarDevice{
public:
	Brick(SoarCommunicator* comm);
	~Brick();

	void updateInputLink(Identifier* inputLink);
	bool readSoarCommand(Identifier* command);
	void readStatus(IntBuffer& buffer, uint& offset);
	uint getDeviceType(){
		return 0;
	}

private:
	SoarCommunicator* comm;

	Identifier* brickId;

	ButtonVector buttons;
	LedMap leds;

};


#endif /* SOARBRICK_H_ */
