/*
 * DirectCommunication.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef DIRECTCOMMUNICATION_H_
#define DIRECTCOMMUNICATION_H_

#include "Ev3Communication.h"
#include "SoarCommunication.h"

#include "sml_Client.h"

class DirectCommunicator : public Ev3Communicator, SoarCommunicator {
public:
	DirectCommunicator();

	void assignManagers(SoarManager* sm, Ev3Manager* em){
		soarManager = sm; 	ev3Manager = em;
	}

	virtual ~DirectCommunicator(){}

	void sendCommandToEv3(Ev3Command command, sml::Identifier* id);

	void sendStatusMessage();

	void updateSoar(){}

private:
	SoarManager* soarManager;
	Ev3Manager* ev3Manager;
};


#endif /* DIRECTCOMMUNICATION_H_ */
