/*
 * SoarBrick.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "Brick.h"
#include "comm/SoarCommunication.h"

#include "bytecodes.h"
#include "lms2012.h"

/****************************************************
 *
 * SoarButton Method Definitions
 *
 ****************************************************/

void BrickButton::updateInputLink(Identifier* parentId){
	if(id == 0){
		// Initialize
		id = parentId->CreateIdWME(name.c_str());
		id->CreateStringWME("type", "button");
	}
	// Update
	WMUtil::updateStringWME(id, "current-state", (curState ? "pressed" : "released"));
	WMUtil::updateStringWME(id, "previous-state", (prevState ? "pressed" : "released"));
	prevState = curState;
}


/****************************************************
 *
 * SoarBrick Method Definitions
 *
 ****************************************************/

Brick::Brick(SoarCommunicator* comm):comm(comm){
	brickId = 0;

	buttons.push_back(new BrickButton("up-button", BUTTON_ID_UP));
	buttons.push_back(new BrickButton("down-button", BUTTON_ID_DOWN));
	buttons.push_back(new BrickButton("left-button", BUTTON_ID_LEFT));
	buttons.push_back(new BrickButton("right-button", BUTTON_ID_RIGHT));
	buttons.push_back(new BrickButton("enter-button", BUTTON_ID_ENTER));
	buttons.push_back(new BrickButton("escape-button", BUTTON_ID_ESCAPE));

	// INITIALIZE LEDS
	leds["off"] = LED_BLACK;
	leds["solid-green"] = LED_GREEN;
	leds["solid-red"] = LED_RED;
	leds["solid-orange"] = LED_ORANGE;
	leds["flash-green"] = LED_GREEN_FLASH;
	leds["flash-red"] = LED_RED_FLASH;
	leds["flash-orange"] = LED_ORANGE_FLASH;
	leds["pulse-green"] = LED_GREEN_PULSE;
	leds["pulse-red"] = LED_RED_PULSE;
	leds["pulse-orange"] = LED_ORANGE_PULSE;
}

Brick::~Brick(){
	for(ButtonVectorIt i = buttons.begin(); i != buttons.end(); i++){
		delete *i;
	}

	if(brickId){
		brickId->DestroyWME();
		brickId = 0;
	}
}

void Brick::updateInputLink(Identifier* inputLink){
	if(brickId == 0){
		brickId = inputLink->CreateIdWME("brick");
	}
	for(ButtonVectorIt i = buttons.begin(); i != buttons.end(); i++){
		(*i)->updateInputLink(brickId);
	}
}

void Brick::readStatus(IntBuffer& buffer, uint& offset){
	int buttonState = buffer[offset++];
	for(ButtonVectorIt i = buttons.begin(); i != buttons.end(); i++){
		(*i)->setState(buttonState);
	}
}

bool Brick::readSoarCommand(Identifier* commandId){
	string ledPattern;
	if(WMUtil::getValue(commandId, "set-led-pattern", ledPattern)){
		LedMapIt ledState = leds.find(ledPattern);
		if(ledState != leds.end()){
			Ev3Command command;
			command.dev = BRICK_DEV;
			command.params.push_back(BRICK_COMMAND_SET_LED);
			command.params.push_back(ledState->second);
			comm->sendCommandToEv3(command, commandId);
			return true;
		} else {
			cout << "UNKNOWN BRICK LED PATTERN: " << ledPattern << endl;
			return false;
		}
	}

	cout << "INVALID BRICK COMMAND" << endl;
	return false;
}

