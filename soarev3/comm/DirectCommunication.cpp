/*
 * DirectCommunication.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "DirectCommunication.h"

#include "soar/SoarManager.h"
#include "ev3/Ev3Manager.h"

using namespace sml;

DirectCommunicator::DirectCommunicator()
: soarManager(0), ev3Manager(0){
}

void DirectCommunicator::sendStatusMessage(){
	IntBuffer buffer;

	StatusList statuses;
	ev3Manager->writeStatus(statuses);
	soarManager->readStatus(statuses);
}

void DirectCommunicator::sendCommandToEv3(Ev3Command command, Identifier* id){
	ev3Manager->executeEv3Command(&command);
	id->CreateStringWME("status", "complete");
}


