/*
 * SoarTouchSensor.cpp
 *
 *  Created on: Nov 28, 2013
 *      Author: aaron
 */

#include "TouchSensor.h"

#include "WMUtil.h"
#include "LcmUtil.h"

#include "lms2012.h"
#include "Constants.h"

#include <sys/ioctl.h>

using namespace std;

TouchSensor::TouchSensor(uint port, SoarCommunicator* comm)
: comm(comm), port(port), pressed(false), prev(false), rootId(0) {

}

TouchSensor::~TouchSensor(){
	if(rootId){
		rootId->DestroyWME();
		rootId = 0;
	}
}

void TouchSensor::updateInputLink(sml::Identifier* inputLink){
	if(!rootId){
		rootId = inputLink->CreateIdWME("sensor");
		rootId->CreateStringWME("type", "touch");
		rootId->CreateIntWME("port", port);
	}
	WMUtil::updateStringWME(rootId, "current-state", (pressed ? "pressed" : "released"));
	WMUtil::updateStringWME(rootId, "previous-state", (prev ? "pressed" : "released"));
	prev = pressed;
}

bool TouchSensor::readSoarCommand(sml::Identifier* commandId){
	cout << "UNEXPECTED TOUCH SENSOR COMMAND: Port " << port << endl;
	return false;
}

void TouchSensor::readStatus(IntBuffer& buffer, uint& offset){
	offset++;

	ushort str, blank;
	unpackShorts(buffer[offset], str, blank);

	pressed = (str > 2000);
}



