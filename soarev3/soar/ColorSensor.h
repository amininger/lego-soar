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
#include <map>

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
	void changeSoarMode(std::string newMode);

	SoarCommunicator* comm;

	uint port;
	std::string mode;
	std::string soarMode;
	short value;

	sml::Identifier* rootId;

	std::map<short, string> colorMap;
};

#endif /* SOARCOLORSENSOR_H_ */
