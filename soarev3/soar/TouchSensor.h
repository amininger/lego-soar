/*
 * SoarTouchSensor.h
 *
 *  Created on: Nov 28, 2013
 *      Author: aaron
 */

#ifndef SOARTOUCHSENSOR_H_
#define SOARTOUCHSENSOR_H_

#include "SoarDevice.h"
#include "comm/SoarCommunication.h"

#include "lms2012.h"

#include "sml_Client.h"

#include <string>

class TouchSensor : public SoarDevice{
public:
	TouchSensor(uint port, SoarCommunicator* comm);
	~TouchSensor();

	void updateInputLink(sml::Identifier* inputLink);
	void readStatus(IntBuffer& buffer, uint& offset);
	bool readSoarCommand(sml::Identifier* commandId);
	uint getDeviceType(){
		return EV3_TOUCH_SENSOR_TYPE;
	}

private:
	SoarCommunicator* comm;

	uint port;

	bool pressed;
	bool prev;

	sml::Identifier* rootId;
};


#endif /* SOARTOUCHSENSOR_H_ */
