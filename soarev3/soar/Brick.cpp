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
		id = parentId->CreateIdWME("button");
		string nameStr = "name";
		WMUtil::updateStringWME(id, nameStr, name);
	}
	// Update
	WMUtil::updateStringWME(id, "cur-state", (curState ? "pressed" : "released"));
	WMUtil::updateStringWME(id, "prev-state", (prevState ? "pressed" : "released"));
	prevState = curState;
}


/****************************************************
 *
 * SoarBrick Method Definitions
 *
 ****************************************************/

Brick::Brick(SoarCommunicator* comm):comm(comm){
	brickId = 0;
	buttonsId = 0;

	buttons.push_back(new BrickButton("up", BUTTON_ID_UP));
	buttons.push_back(new BrickButton("down", BUTTON_ID_DOWN));
	buttons.push_back(new BrickButton("left", BUTTON_ID_LEFT));
	buttons.push_back(new BrickButton("right", BUTTON_ID_RIGHT));
	buttons.push_back(new BrickButton("enter", BUTTON_ID_ENTER));
	buttons.push_back(new BrickButton("escape", BUTTON_ID_ESCAPE));

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

	if(buttonsId){
		buttonsId->DestroyWME();
		buttonsId = 0;
	}
}

void Brick::updateInputLink(Identifier* inputLink){
	if(brickId == 0){
		brickId = inputLink->CreateIdWME("brick");
		buttonsId = brickId->CreateIdWME("buttons");
	}
	for(ButtonVectorIt i = buttons.begin(); i != buttons.end(); i++){
		(*i)->updateInputLink(buttonsId);
	}
}

void Brick::readStatus(IntBuffer& buffer, uint& offset){
	int buttonState = buffer[offset++];
	for(ButtonVectorIt i = buttons.begin(); i != buttons.end(); i++){
		(*i)->setState(buttonState);
	}
}

bool Brick::readSoarCommand(Identifier* commandId){
	Ev3Command command;
	command.dev = BRICK_DEV;

	Identifier* subId;
	if(WMUtil::getValue(commandId, "set", subId)){
		if(!handleSetCommand(command.params, subId)){
			return false;
		}
	} else {
		return false;
	}

	comm->sendCommandToEv3(command, commandId);
	return true;
}

bool Brick::handleSetCommand(IntBuffer& buffer, Identifier* commandId){
	string state;
	if(WMUtil::getValue(commandId, "led", state)){
		LedMapIt ledState = leds.find(state);
		if(ledState != leds.end()){
			buffer.push_back(BRICK_COMMAND_SET_LED);
			buffer.push_back(ledState->second);
			return true;
		}
	}
	return false;
}


