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
: comm(comm), port(port), mode(""), value(0), rootId(0), soarMode("")
{
	colorMap[0] = "none";
	colorMap[1] = "black";
	colorMap[2] = "blue";
	colorMap[3] = "green";
	colorMap[4] = "yellow";
	colorMap[5] = "red";
	colorMap[6] = "white";
	colorMap[7] = "brown";
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

	if(mode != soarMode){
		changeSoarMode(mode);
	}
	if(mode == "reflect" || mode == "ambient"){
		WMUtil::updateIntWME(rootId, "brightness", value);
	} else if(mode == "color"){
		string color = "none";
		map<short, string>::iterator cIt = colorMap.find(value);
		if(cIt != colorMap.end()){
			color = cIt->second;
		}
		WMUtil::updateStringWME(rootId, "value", color);
	}
}

void ColorSensor::changeSoarMode(string newMode){
	if(rootId == 0){
		return;
	}
	// Remove the old
	if(soarMode == "reflect" || soarMode == "ambient"){
		WMUtil::removeWME(rootId, "brightness");
	} else if(soarMode == "color"){
		WMUtil::removeWME(rootId, "color");
	}

	// Add the new
	if(newMode == "reflect" || newMode == "ambient"){
		rootId->CreateIntWME("brightness", 0);
	} else if(newMode == "color"){
		rootId->CreateStringWME("value", "none");
	}

	soarMode = newMode;
}

bool ColorSensor::readSoarCommand(sml::Identifier* commandId){
	string newMode;
	if(WMUtil::getValue(commandId, "set-mode", newMode)){
		uchar modeId;
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

		Ev3Command command;
		command.dev = INPUT_MAN_DEV;
		command.params.push_back(CHANGE_MODE_COMMAND);
		command.params.push_back(packBytes(port-1, modeId, EV3_COLOR_SENSOR_TYPE, 0));
		comm->sendCommandToEv3(command, commandId);
		return true;
	}

	cout << "INVALID COLOR SENSOR COMMAND" << endl;
	return false;
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
