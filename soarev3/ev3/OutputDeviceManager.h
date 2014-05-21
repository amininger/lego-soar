/*
 * OutputDeviceManager.h
 *
 *  Created on: Nov 21, 2013
 *      Author: aaron
 */

#ifndef OUTPUTDEVICEMANAGER_H_
#define OUTPUTDEVICEMANAGER_H_

#include "comm/CommStructs.h"
#include "LcmUtil.h"

#include "lms2012.h"

class OutputDeviceManager : public CommandHandler {
public:
	OutputDeviceManager();
	virtual ~OutputDeviceManager();
	void handleCommand(Ev3Command* command);
	Ev3Status getStatus();

private:
	int motorInputFile;
	int motorOutputFile;
	MOTORDATA* motorData;
};

#endif /* OUTPUTDEVICEMANAGER_H_ */
