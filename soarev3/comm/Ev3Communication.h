/*
 * Ev3Communication.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef EV3COMMUNICATION_H_
#define EV3COMMUNICATION_H_

class Ev3Manager;

#include "CommStructs.h"

#include "sml_Client.h"
#include <pthread.h>

#include <string>

class Ev3Communicator{
public:
	virtual ~Ev3Communicator(){}

	virtual void start();

	static void* sendStatusThreadFunction(void*);

	virtual void sendStatusMessage() = 0;

protected:
	pthread_t sendStatusThread;
};


class Ev3LcmCommunicator : public Ev3Communicator{
public:
	Ev3LcmCommunicator(const char* channel);


	virtual ~Ev3LcmCommunicator(){
		pthread_mutex_destroy(&mutex);
	}

	void assignManager(Ev3Manager* manager){
		ev3Manager = manager;
	}

	void start();

	char* getInChannel(){
		return inChannel;
	}

private:
	static void* lcmliteThreadFunction(void*);

	static void lcmHandler(lcmlite_t* lcm, const char* channel, const void* buf, int buf_len, void* user);

	void receiveCommandMessage(IntBuffer& buffer, uint& offset);

	void sendStatusMessage();

	static void* sendAckThreadFunction(void*);

	void sendAckMessage();


private:
	// lcmlite variables
	LcmliteWrapper wrapper;
	pthread_t lcmliteThread;
	pthread_t sendAckThread;
	pthread_mutex_t mutex;

	AckSet acks;

	Ev3Manager* ev3Manager;

	char inChannel[8];
	char outChannel[8];
};

#endif /* EV3COMMUNICATION_H_ */
