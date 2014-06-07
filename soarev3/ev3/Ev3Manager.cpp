/*
 * Ev3Manager.cpp
 *
 *  Created on: Nov 20, 2013
 *      Author: aaron
 */

#include "Ev3Manager.h"

#include "Constants.h"

#include <iostream>
using namespace std;

Ev3Manager::Ev3Manager() {
	brick = new Ev3Brick();
	inputDevices = new InputDeviceManager();
	outputDevices = new OutputDeviceManager();

	commandHandlers[MANAGER_DEV] = this;
	commandHandlers[BRICK_DEV] = brick;
	commandHandlers[OUTPUT_MAN_DEV] = outputDevices;
	commandHandlers[INPUT_MAN_DEV] = inputDevices;
}

Ev3Manager::~Ev3Manager() {
	delete brick;
	delete inputDevices;
	delete outputDevices;
}

void Ev3Manager::writeStatus(StatusList& statuses){
	statuses.push_back(this->getStatus());
	statuses.push_back(brick->getStatus());
	statuses.push_back(inputDevices->getStatus());
	statuses.push_back(outputDevices->getStatus());
}

void Ev3Manager::executeEv3Command(Ev3Command* command){
	CommandHandlersIt it = commandHandlers.find(command->dev);
	if(it != commandHandlers.end()){
		it->second->handleCommand(command);
	} else {
		cout << "UNKNOWN COMMAND FOR: " << command->dev << endl;
	}
}

Ev3Status Ev3Manager::getStatus(){
	return Ev3Status(MANAGER_DEV);
}

void Ev3Manager::handleCommand(Ev3Command* command){}

