#ifndef __SOAR_MOTOR_H__
#define __SOAR_MOTOR_H__

class SoarCommunicator;

#include "SoarDevice.h"

#include "sml_Client.h"


class Motor : public SoarDevice {
public:
	Motor(uchar port, SoarCommunicator* comm);

	void readStatus(IntBuffer& buffer, uint& offset);

	void updateInputLink(sml::Identifier* parentId);

	bool readSoarCommand(sml::Identifier* commandId);

	uint getDeviceType(){
		return LARGE_MOTOR_TYPE;
	}

private:
	uchar port;
	string portStr;

	SoarCommunicator* comm;

	int speed;
	int tachoCount;
	int tachoSensor;

	sml::Identifier* motorId;
};

#endif
