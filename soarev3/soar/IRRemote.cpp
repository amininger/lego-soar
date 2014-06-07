/*
 * SoarIRRemote.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "IRRemote.h"

#include "WMUtil.h"
#include "LcmUtil.h"

#include "lms2012.h"
#include "Constants.h"

#include <sys/ioctl.h>

using namespace std;

/****************************************************
 *
 * RemoteButton Method Definitions
 *
 ****************************************************/
RemoteButton::RemoteButton(ushort buttonType)
:buttonType(buttonType), id(0), curState(false), prevState(false){
	if(buttonType == 0){
		name = "red-up-button";
	} else if(buttonType == 1){
		name = "red-down-button";
	} else if(buttonType == 2){
		name = "blue-up-button";
	} else {
		name = "blue-down-button";
	}
}

RemoteButton::~RemoteButton(){
	if(id){
		id->DestroyWME();
		id = 0;
	}
}

void RemoteButton::updateInputLink(Identifier* parentId){
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

void RemoteButton::setState(int stateCode){
	if(buttonType == 0){
		curState = (stateCode == 1) || (stateCode == 5) || (stateCode == 6) || (stateCode == 10);
	} else if(buttonType == 1){
		curState = (stateCode == 2) || (stateCode == 7) || (stateCode == 8) || (stateCode == 10);
	} else if(buttonType == 2){
		curState = (stateCode == 3) || (stateCode == 5) || (stateCode == 7) || (stateCode == 11);
	} else {
		curState = (stateCode == 4) || (stateCode == 6) || (stateCode == 8) || (stateCode == 11);
	}
}

/****************************************************
 *
 * IRRemote Method Definitions
 *
 ****************************************************/

IRRemote::IRRemote(uint port, SoarCommunicator* comm)
: comm(comm), port(port), mode(0), rootId(0) {
	channel = 0;
	distance = 0;
	direction = 0;

	buttonState = 0;
	for(int b = 0; b < NUM_IR_REMOTE_BUTTONS; b++){
		buttons[b] = 0;
	}
}

IRRemote::~IRRemote(){
	for(int b = 0; b < NUM_IR_REMOTE_BUTTONS; b++){
		if(buttons[b]){
			delete buttons[b];
			buttons[b] = 0;
		}
	}
	if(rootId){
		rootId->DestroyWME();
		rootId = 0;
	}
}

void IRRemote::updateInputLink(sml::Identifier* inputLink){
	if(!rootId){
		rootId = inputLink->CreateIdWME("sensor");
		rootId->CreateStringWME("type", "ir-remote");
		rootId->CreateIntWME("port", port);
	}

	if(soarMode != mode){
		changeSoarMode(mode);
	} else if(soarMode == EV3_IR_REMOTE_PROXIMITY_MODE){
		WMUtil::updateStringWME(rootId, "mode", "proximity");
		WMUtil::updateIntWME(rootId, "distance", distance);
	} else if(soarMode == EV3_IR_REMOTE_BEACON_MODE){
		WMUtil::updateStringWME(rootId, "mode", "beacon");
		WMUtil::updateIntWME(rootId, "channel", channel+1);
		WMUtil::updateIntWME(rootId, "direction", direction);
		WMUtil::updateIntWME(rootId, "distance", distance);
	} else if(soarMode == EV3_IR_REMOTE_REMOTE_MODE){
		WMUtil::updateStringWME(rootId, "mode", "remote");
		WMUtil::updateIntWME(rootId, "channel", channel+1);
		for(int b = 0; b < NUM_IR_REMOTE_BUTTONS; b++){
			if(buttons[b]){
				buttons[b]->updateInputLink(rootId);
			}
		}
	}
}

void IRRemote::changeSoarMode(uchar newMode){
	if(!rootId){
		return;
	}

	// Cleanup old stuff
	if(soarMode == EV3_IR_REMOTE_PROXIMITY_MODE){
		WMUtil::removeWME(rootId, "distance");
	} else if(soarMode == EV3_IR_REMOTE_BEACON_MODE){
		WMUtil::removeWME(rootId, "channel");
		WMUtil::removeWME(rootId, "distance");
		WMUtil::removeWME(rootId, "direction");
	} else if(soarMode == EV3_IR_REMOTE_REMOTE_MODE){
		WMUtil::removeWME(rootId, "channel");
		for(int b = 0; b < NUM_IR_REMOTE_BUTTONS; b++){
			if(buttons[b]){
				delete buttons[b];
				buttons[b] = 0;
			}
		}
	}

	// Add new stuff
	soarMode = newMode;
	if(mode == EV3_IR_REMOTE_PROXIMITY_MODE){
		rootId->CreateIntWME("distance", distance);
	} else if(mode == EV3_IR_REMOTE_BEACON_MODE){
		rootId->CreateIntWME("channel", channel+1);
		rootId->CreateIntWME("distance", distance);
		rootId->CreateIntWME("direction", direction);
	} else if(mode == EV3_IR_REMOTE_REMOTE_MODE){
		rootId->CreateIntWME("channel", channel+1);
		for(int b = 0; b < NUM_IR_REMOTE_BUTTONS; b++){
			buttons[b] = new RemoteButton(b);
		}
	}
}


bool IRRemote::readSoarCommand(sml::Identifier* commandId){
	bool validCommand = false;

	// set-channel
	int newChannel;
	if(WMUtil::getValue(commandId, "set-channel", newChannel)){
		if(newChannel < 1 || newChannel > 4){
			cout << "IR-REMOTE INVALID CHANNEL " << newChannel << endl;
			return false;
		}
		channel = newChannel - 1;
		validCommand = true;
	}

	// set-mode
	string newMode;
	if(WMUtil::getValue(commandId, "set-mode", newMode)){
		if(newMode == "proximity"){
			mode = EV3_IR_REMOTE_PROXIMITY_MODE;
		} else if(newMode == "beacon"){
			mode = EV3_IR_REMOTE_BEACON_MODE;
		} else if(newMode == "remote"){
			mode = EV3_IR_REMOTE_REMOTE_MODE;
		} else {
			cout << "IR-REMOTE UNKNOWN MODE: " << newMode << endl;
			return false;
		}
		validCommand = true;
		Ev3Command command;
		command.dev = INPUT_MAN_DEV;
		command.params.push_back(CHANGE_MODE_COMMAND);
		command.params.push_back(packBytes(port-1, mode, EV3_IR_REMOTE_SENSOR_TYPE, 0));
		comm->sendCommandToEv3(command, commandId);
	} else if(validCommand){
		commandId->CreateStringWME("status", "complete");
	}

	if(!validCommand){
		cout << "IR-REMOTE INVALID COMMAND" << endl;
	}
	return validCommand;
}

void IRRemote::readStatus(IntBuffer& buffer, uint& offset){
	uchar cat, port, type;
	unpackBytes(buffer[offset++], cat, port, type, mode);

	uchar data[8];
	unpackBytes(buffer[offset++], &data[0]);
	unpackBytes(buffer[offset++], &data[4]);

	if(mode == EV3_IR_REMOTE_PROXIMITY_MODE){
		distance = data[0];
	} else if(mode == EV3_IR_REMOTE_BEACON_MODE){
		direction = data[2*channel];
		distance = data[2*channel+1];
	} else if(mode == EV3_IR_REMOTE_REMOTE_MODE){
		buttonState = data[channel];
		for(int b = 0; b < NUM_IR_REMOTE_BUTTONS; b++){
			if(buttons[b]){
				buttons[b]->setState(buttonState);
			}
		}
	}
}


