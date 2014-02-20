/*
 * SoarColorSensor.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "ColorSensor.h"

#include "WMUtil.h"
#include "LcmUtil.h"

#include "lms2012.h"
#include "Constants.h"

#include <sys/ioctl.h>

using namespace std;

ColorSensor::ColorSensor(uint port, SoarCommunicator* comm)
: comm(comm), port(port), mode(""), value(0), rootId(0) {

}

ColorSensor::~ColorSensor(){
	if(rootId){
		rootId->DestroyWME();
		rootId = 0;
	}
}

void ColorSensor::updateInputLink(sml::Identifier* inputLink){
	if(!rootId){
		rootId = inputLink->CreateIdWME("sensor");
		rootId->CreateStringWME("type", "color");
		rootId->CreateIntWME("port", port);
	}
	WMUtil::updateStringWME(rootId, "mode", mode);
	WMUtil::updateIntWME(rootId, "value", value);
}

bool ColorSensor::readSoarCommand(sml::Identifier* commandId){
	Ev3Command command;
	command.dev = INPUT_MAN_DEV;

	Identifier* subId;
	if(WMUtil::getValue(commandId, "set", subId)){
		if(!readSetCommand(subId, command.params)){
			return false;
		}
	} else {
		cout << "UNKNOWN COLOR SENSOR COMMAND" << endl;
		return false;
	}
	comm->sendCommandToEv3(command, commandId);
	return true;
}

bool ColorSensor::readSetCommand(sml::Identifier* commandId, IntBuffer& params){
	string newMode;
	uchar modeId;
	if(WMUtil::getValue(commandId, "mode", newMode)){
		if(newMode == "reflect"){
			modeId = COLOR_SENSOR_MODE_REFLECT;
		} else if(newMode == "ambient"){
			modeId = COLOR_SENSOR_MODE_AMBIENT;
		} else if(newMode == "color"){
			modeId = COLOR_SENSOR_MODE_COLOR;
		} else {
			cout << "UNKNOWN COLOR SENSOR MODE: " << newMode << endl;
			return false;
		}
	}
	params.push_back(CHANGE_MODE_COMMAND);
	params.push_back(packBytes(port-1, modeId, EV3_COLOR_SENSOR_TYPE, 0));
	return true;
}

void ColorSensor::readStatus(IntBuffer& buffer, uint& offset){
	uchar cat, port, type, modeId;
	unpackBytes(buffer[offset++], cat, port, type, modeId);

	uchar data[8];
	unpackBytes(buffer[offset++], &data[0]);
	unpackBytes(buffer[offset++], &data[4]);

	value = data[0];
	if(modeId == COLOR_SENSOR_MODE_REFLECT){
		this->mode = "reflect";
	} else if(modeId == COLOR_SENSOR_MODE_AMBIENT){
		this->mode = "ambient";
	} else if(modeId == COLOR_SENSOR_MODE_COLOR){
		this->mode = "color";
	}
}
