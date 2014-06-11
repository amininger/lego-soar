/*
 * Ev3Communication.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "Ev3Communication.h"

#include "ev3/Ev3Manager.h"

#include "LcmUtil.h"

#include <iostream>
#include <sstream>
using namespace std;


// Ev3Communicator
void* Ev3Communicator::sendStatusThreadFunction(void* arg){
	Ev3Communicator* ev3Comm = (Ev3Communicator*)arg;
	while(true){
		ev3Comm->sendStatusMessage();
		usleep(1000000/EV3_SEND_STATUS_FPS);
	}
	return 0;
}

void Ev3Communicator::start(){
	pthread_create(&sendStatusThread, 0, &sendStatusThreadFunction, this);
}


// Ev3LcmCommunicator
Ev3LcmCommunicator::Ev3LcmCommunicator(const char* channel)
: ev3Manager(0){
	// Channel to publish to
	snprintf(outChannel, 8, "L2S%s", channel);

	// Channel to listen to
	snprintf(inChannel, 8, "S2L%s", channel);

	wrapper.init();
	wrapper.subscribe(inChannel, lcmHandler, (void*)this);

	pthread_mutex_init(&mutex, 0);
}

void Ev3LcmCommunicator::start(){
	pthread_create(&lcmliteThread, 0, &lcmliteThreadFunction, this);
	pthread_create(&sendAckThread, 0, &sendAckThreadFunction, this);
	Ev3Communicator::start();
}

void* Ev3LcmCommunicator::lcmliteThreadFunction(void* arg){
	Ev3LcmCommunicator* ev3Comm = (Ev3LcmCommunicator*)arg;
	while(true){
		ev3Comm->wrapper.checkIncoming();
	}
	return 0;
}

void Ev3LcmCommunicator::lcmHandler(lcmlite_t* lcm, const char* channel, const void* buf, int buf_len, void* user){
	Ev3LcmCommunicator* comm = (Ev3LcmCommunicator*)user;
	if(strcmp(comm->getInChannel(), channel) != 0){
		return;
	}

	IntBuffer params;
	uint offset = 0;
	const uchar* charBuff = (const uchar*)buf;
	unpackBuffer(charBuff, buf_len, params);

	uint messageType = params[offset++];
	if(messageType == COMMAND_MESSAGE){
		comm->receiveCommandMessage(params, offset);
	}
}

void Ev3LcmCommunicator::receiveCommandMessage(IntBuffer& buffer, uint& offset){
	pthread_mutex_lock(&mutex);
	//cout << "--> Ev3 Receive Command" << endl;

	// finishedAcks - acks that we stopped receiving (processed)
	AckSet finishedAcks;
	finishedAcks.insert(acks.begin(), acks.end());

	uint numCommands = buffer[offset++];
	for(uint i = 0; i < numCommands; i++){
		Ev3Command command;
		command.unpackCommand(buffer, offset);
		if(acks.find(command.ack) == acks.end()){
			// New command, execute
			ev3Manager->executeEv3Command(&command);
			acks.insert(command.ack);
		}
		finishedAcks.erase(command.ack);
	}

	for(AckSetIt i = finishedAcks.begin(); i != finishedAcks.end(); i++){
		acks.erase(*i);
	}
	//cout << "<-- Ev3 Receive Command" << endl;
	pthread_mutex_unlock(&mutex);
	sendAckMessage();
}


void Ev3LcmCommunicator::sendStatusMessage(){
	pthread_mutex_lock(&mutex);
	//cout << "--> Ev3 Send Status" << endl;
	IntBuffer buffer;
	buffer.push_back(STATUS_MESSAGE);

	StatusList statuses;
	ev3Manager->writeStatus(statuses);
	buffer.push_back(statuses.size());
	for(uint i = 0; i < statuses.size(); i++){
		statuses[i].packStatus(buffer);
	}

	// Create + send outgoing buffer
	uchar* outBuffer;
	uint buf_size;
	packBuffer(buffer, outBuffer, buf_size);

	wrapper.publish(outChannel, (void*)outBuffer, buf_size);
	delete [] outBuffer;
	//cout << "<-- Ev3 Send Status" << endl;
	pthread_mutex_unlock(&mutex);
}

void* Ev3LcmCommunicator::sendAckThreadFunction(void* arg){
	Ev3LcmCommunicator* ev3Comm = (Ev3LcmCommunicator*)arg;
	while(true){
		ev3Comm->sendAckMessage();
		usleep(1000000/EV3_SEND_ACK_FPS);
	}
	return 0;
}

void Ev3LcmCommunicator::sendAckMessage(){
	pthread_mutex_lock(&mutex);
	//cout << "--> Ev3 Send Ack" << endl;
	if(acks.size() == 0){
		//cout << "<-- Ev3 Send Ack" << endl;
		pthread_mutex_unlock(&mutex);
		return;
	}

	IntBuffer buffer;
	buffer.push_back(ACK_MESSAGE);
	buffer.push_back(acks.size());
	for(AckSetIt it = acks.begin(); it != acks.end(); it++){
		buffer.push_back(*it);
	}

	// Create + send outgoing buffer
	uchar* outBuffer;
	uint buf_size;
	packBuffer(buffer, outBuffer, buf_size);

	wrapper.publish(outChannel, (void*)outBuffer, buf_size);
	delete [] outBuffer;

	//cout << "<-- Ev3 Send Ack" << endl;
	pthread_mutex_unlock(&mutex);
}


