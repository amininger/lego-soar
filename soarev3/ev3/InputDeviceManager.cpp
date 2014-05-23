/*
 * InputDeviceManager.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: aaron
 */

#include "InputDeviceManager.h"

#include "Constants.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <iostream>
using namespace std;


InputDeviceManager::InputDeviceManager() {
	uartFile = open(UART_DEVICE_NAME, O_RDWR | O_SYNC);
	if(uartFile == -1){
		cout << "Failed to open UART file" << endl;
	} else {
		// Map the data
		uart = (UART*)mmap(0, sizeof(UART), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, uartFile, 0);
		if(uart == MAP_FAILED){
			cout << "FAILED TO MAP UART" << endl;
			close(uartFile);
			uartFile = -1;
		}
	}

	analogFile = open(ANALOG_DEVICE_NAME, O_RDWR | O_SYNC);
	if(analogFile == -1){
		cout << "Failed to open analog file" << endl;
	} else {
		analog = (ANALOG*)mmap(0, sizeof(ANALOG), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, analogFile, 0);
		if(analog == MAP_FAILED){
			cout << "FAILED TO MAP ANALOG" << endl;
			close(analogFile);
			analogFile = -1;
		}
	}

	for(int i = 0; i < NUM_INPUTS; i++){
		analogDevs[i] = 0;
	}
}

InputDeviceManager::~InputDeviceManager() {
	close(uartFile);
	close(analogFile);
}

void InputDeviceManager::handleCommand(Ev3Command* command){
	uint commandCode = command->params[0];
	if(commandCode == CHANGE_MODE_COMMAND){
		uchar port, mode, type, empty;
		unpackBytes(command->params[1], port, mode, type, empty);

		if(PRINT_EV3_DEBUG){
			cout << "SET MODE: " << (uint)port << ", " << (uint)mode << ", " << (uint)type << endl;
		}
		devcon.Mode[port] = mode;
		ioctl(uartFile, UART_SET_CONN, &devcon);
	} else if(commandCode == CREATE_ANALOG_SENSOR_COMMAND){
		cout << "Create Analog SEnsor" << endl;
		uchar port, type, blank1, blank2;
		unpackBytes(command->params[1], port, type, blank1, blank2);
		analogDevs[port] = type;
	} else if(commandCode == DELETE_ANALOG_SENSOR_COMMAND){
		uchar port, blank1, blank2, blank3;
		unpackBytes(command->params[2], port, blank1, blank2, blank3);
		analogDevs[port] = 0;
	}
}

Ev3Status InputDeviceManager::getStatus(){
	Ev3Status status(INPUT_MAN_DEV);
	status.info.push_back(NUM_INPUTS);

	for(uint i = 0; i < NUM_INPUTS; i++){
		// Check to see if there is a serial sensor on the port
		if(uartFile != -1){
			UARTCTL info;
			info.Port = i;
			info.Mode = devcon.Mode[i];
			ioctl(uartFile, UART_READ_MODE_INFO, &info);


			if(info.TypeData.Type != 0){
				uint sensorInfo = packBytes(SENSOR_CAT_SERIAL, i,
						info.TypeData.Type, info.TypeData.Mode);
				status.info.push_back(sensorInfo);

				uchar* data = (uchar*)uart->Raw[i][uart->Actual[i]];
				status.info.push_back(packBytes(&data[0]));
				status.info.push_back(packBytes(&data[4]));

				devcon.Type[i] = info.TypeData.Type;
				devcon.Connection[i] = CONN_INPUT_UART;

				continue;
			}
		} 
		if(analogDevs[i] != 0){
			// Report back information about an analog device 
			status.info.push_back(packBytes(SENSOR_CAT_ANALOG, i, analogDevs[i], 0));
			status.info.push_back(packShorts(analog->Pin6[i][analog->Actual[i]], 0));
			continue;
		}
		status.info.push_back(packBytes(SENSOR_CAT_NONE, i, 0, 0));
	}

	return status;
}
