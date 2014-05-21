/*
 * OutputDeviceManager.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: aaron
 */

#include "OutputDeviceManager.h"

#include "Constants.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <iostream>
using namespace std;

OutputDeviceManager::OutputDeviceManager() {
	motorOutputFile = open(PWM_DEVICE_NAME, O_WRONLY);
	if(motorOutputFile == -1){
		return;
	}
	cout << "OPENED MOTOR OUTPUT FILE: " << motorOutputFile << endl;

	motorInputFile = open(MOTOR_DEVICE_NAME, O_RDWR | O_SYNC);
	if(motorInputFile == -1){
		close(motorOutputFile);
		motorOutputFile = -1;
		return;
	}
	cout << "OPENED MOTOR INPUT FILE: " << motorInputFile << endl;

	motorData  =  (MOTORDATA*)mmap(0, sizeof(MOTORDATA)*vmOUTPUTS, PROT_READ | PROT_WRITE,
			MAP_FILE | MAP_SHARED, motorInputFile, 0);
	if (motorData == MAP_FAILED)
	{
		close(motorInputFile);
		motorInputFile = -1;
		close(motorOutputFile);
		motorOutputFile = -1;
		return;
	}

	cout << "MAPPED MOTOR FILES" << endl;
}

OutputDeviceManager::~OutputDeviceManager() {
	if(motorInputFile != -1){
		close(motorInputFile);
	}
	if(motorOutputFile != -1){
		close(motorOutputFile);
	}
}

void OutputDeviceManager::handleCommand(Ev3Command* command){
	for(unsigned int i = 0; i < command->params.size(); i++){
		uint nextCommand = command->params[i];
		uchar port, cmd, param, empty;
		unpackBytes(nextCommand, port, cmd, param, empty);



		char data[3];
		data[0] = cmd;
		data[1] = 1 << port;
		data[2] = param;

		if(cmd == MOTOR_COMMAND_SET_DIRECTION && param == MOTOR_DIR_BACKWARD){
			data[2] = -1;
		}

		if(PRINT_EV3_DEBUG){
			printf("Received command: %d %d %d\n", (uint)data[0], (uint)data[1], (uint)data[2]);
		}

		switch(cmd){
		// Commands that don't have a parameter (start/stop)
		case MOTOR_COMMAND_START:
		case MOTOR_COMMAND_STOP:
			write(motorOutputFile, data, 2);
			break;
		// Commands that do have a parameter (set dir/speed/power)
		case MOTOR_COMMAND_SET_DIRECTION:
		case MOTOR_COMMAND_SET_SPEED:
		case MOTOR_COMMAND_SET_POWER:
			write(motorOutputFile, data, 3);
			break;
		}
	}
}

Ev3Status OutputDeviceManager::getStatus(){
	Ev3Status status(OUTPUT_MAN_DEV);
	for(int port = 0; port < NUM_OUTPUTS; port++){
		status.info.push_back(motorData[port].Speed);
		status.info.push_back(motorData[port].TachoCounts);
		status.info.push_back(motorData[port].TachoSensor);
	}
	return status;
}
