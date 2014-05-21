/*
 * SoarDevice.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef SOARDEVICE_H_
#define SOARDEVICE_H_

class SoarDevice;

#include "comm/CommStructs.h"

#include "sml_Client.h"

class SoarDevice{
public:
	virtual ~SoarDevice(){}

	virtual void updateInputLink(sml::Identifier* inputLink) = 0;
	virtual bool readSoarCommand(sml::Identifier* command) = 0;
	virtual void readStatus(IntBuffer& buffer, uint& offset) = 0;
	virtual uint getDeviceType() = 0;
};

#endif /* SOARDEVICE_H_ */
