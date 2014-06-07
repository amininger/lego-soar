/*
 * Ev3Brick.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "Ev3Brick.h"
#include "comm/Ev3Communication.h"

#include "lms2012.h"
#include "bytecodes.h"

#include <fcntl.h>
//#include <stdio.h>
#include <sys/mman.h>

#include <iostream>
using namespace std;

/****************************************************
 *
 * Ev3Brick Method Definitions
 *
 ****************************************************/

Ev3Brick::Ev3Brick(){
	uiFile = open(UI_DEVICE_NAME, O_RDWR | O_SYNC);
	if(uiFile == -1){
		cout << "Failed to open ui file" << endl;
		return;
	}

	uiData = (UI*)mmap(0, sizeof(UI), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, uiFile, 0);
	if(uiData == MAP_FAILED){
		cout << "Failed to map ui file" << endl;
		close(uiFile);
		return;
	}
}

Ev3Brick::~Ev3Brick() {
	close(uiFile);
}

void Ev3Brick::handleCommand(Ev3Command* command){
	int commandType = command->params[0];
	if(commandType == BRICK_COMMAND_SET_LED){
		char cmd[2] = {0, 0};
		cmd[0] = '0' + command->params[1];	// LED Pattern, need to add '0' offset for some reason
		write(uiFile, cmd, 2);
	}
}

Ev3Status Ev3Brick::getStatus(){
	Ev3Status status(BRICK_DEV);

	// Write out button states
	int buttonState = 0;
	for(int i = 0; i < BUTTONS; i++){
		buttonState = buttonState | ((uiData->Pressed[i] ? 1 : 0) << i);
	}

	status.info.push_back(buttonState);
	return status;
}



