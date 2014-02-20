/*
 * SoarColorSensor.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef SOARCOLORSENSOR_H_
#define SOARCOLORSENSOR_H_

#include "SoarDevice.h"
#include "comm/SoarCommunication.h"

#include "lms2012.h"

#include "sml_Client.h"

#include <string>

class ColorSensor : public SoarDevice{
public:
	ColorSensor(uint port, SoarCommunicator* comm);
	~ColorSensor();

	void updateInputLink(sml::Identifier* inputLink);
	void readStatus(IntBuffer& buffer, uint& offset);
	bool readSoarCommand(sml::Identifier* commandId);
	bool readSetCommand(sml::Identifier* commandId, IntBuffer& params);
	uint getDeviceType(){
		return EV3_COLOR_SENSOR_TYPE;
	}

private:
	SoarCommunicator* comm;

	uint port;
	std::string mode;
	short value;

	sml::Identifier* rootId;
};

#endif /* SOARCOLORSENSOR_H_ */
