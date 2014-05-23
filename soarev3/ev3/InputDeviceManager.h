/*
 * InputDeviceManager.h
 *
 *  Created on: Nov 21, 2013
 *      Author: aaron
 */

#ifndef INPUTDEVICEMANAGER_H_
#define INPUTDEVICEMANAGER_H_

#include "lms2012.h"

#include "comm/CommStructs.h"

class InputDeviceManager : public CommandHandler {
public:
	InputDeviceManager();
	void initDevices();
	virtual ~InputDeviceManager();
	void handleCommand(Ev3Command* command);
	Ev3Status getStatus();

private:
	int uartFile;
	UART* uart;

	int analogFile;
	ANALOG* analog;
	uchar analogDevs[NUM_INPUTS];

	DEVCON devcon;
};

#endif /* INPUTDEVICEMANAGER_H_ */
