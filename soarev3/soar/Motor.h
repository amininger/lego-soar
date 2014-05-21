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
	bool handleStartCommand(IntBuffer& buffer, sml::Identifier* commandId);
	bool handleStopCommand(IntBuffer& buffer, sml::Identifier* commandId);
	bool handleSetCommand(IntBuffer& buffer, sml::Identifier* commandId);

	uchar port;

	SoarCommunicator* comm;

	int speed;
	int tachoCount;
	int tachoSensor;

	sml::Identifier* motorId;
};

#endif
