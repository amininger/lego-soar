/*
 * SoarCommunication.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef SOARCOMMUNICATION_H_
#define SOARCOMMUNICATION_H_

#include <string>

class SoarManager;

#include "sml_Client.h"

#include "CommStructs.h"

typedef std::map<uint, sml::Identifier*> IdentifierMap;
typedef std::set<sml::Identifier*> IdentifierSet;

class SoarCommunicator {
public:
	virtual ~SoarCommunicator(){}

	virtual void sendCommandToEv3(Ev3Command command, sml::Identifier* id) = 0;

	virtual void updateSoar() = 0;
};


class SoarLcmCommunicator : public SoarCommunicator{
public:
	SoarLcmCommunicator(const char* channel);

	virtual ~SoarLcmCommunicator(){
		pthread_mutex_destroy(&mutex);
	}

	void assignManager(SoarManager* manager){
		soarManager = manager;
	}

	void sendCommandToEv3(Ev3Command command, sml::Identifier* id);

	void start();

	void updateSoar();

	char* getInChannel(){
		return inChannel;
	}

private:
	static void* lcmliteThreadFunction(void* arg);

	static void lcmHandler(lcmlite_t* lcm, const char* channel, const void* buf, int buf_len, void* user);

	void receiveAckMessage(IntBuffer& buffer, uint& offset);

	void receiveStatusMessage(IntBuffer& buffer, uint& offset);

	static void* sendCommandThreadFunction(void* arg);

	void sendCommandMessage();

private:
	// lcmlite variables
	LcmliteWrapper wrapper;

	pthread_t lcmliteThread;
	pthread_t sendCommandThread;
	pthread_mutex_t mutex;

	SoarManager* soarManager;

	uint nextAck;
	CommandMap waitingCommands;
	IdentifierMap waitingIdentifiers;
	IdentifierSet finishedIdentifiers;

	char inChannel[8];
	char outChannel[8];


};


#endif /* SOARCOMMUNICATION_H_ */
