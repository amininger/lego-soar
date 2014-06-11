/*
 * SoarCommunication.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "SoarCommunication.h"


#include "Constants.h"
#include "CommStructs.h"
#include "LcmUtil.h"

#include "soar/SoarManager.h"

#include "LcmliteWrapper.h"

#include <iostream>
using namespace std;

using namespace sml;

// SoarLcmCommunicator
SoarLcmCommunicator::SoarLcmCommunicator(const char* channel)
: soarManager(0), nextAck(1){
	// Channel to publish to
	snprintf(outChannel, 8, "S2L%s", channel);

	// Channel to listen to
	snprintf(inChannel, 8, "L2S%s", channel);

	wrapper.init();
	wrapper.subscribe(inChannel, lcmHandler, this);

	pthread_mutex_init(&mutex, 0);
}

void SoarLcmCommunicator::start(){
	pthread_create(&lcmliteThread, 0, &lcmliteThreadFunction, this);
}

void SoarLcmCommunicator::sendCommandToEv3(Ev3Command command, Identifier* id){
	pthread_mutex_lock(&mutex);
	// Need to send the command over LCM and wait for the ack
	uint ack = nextAck++;
	command.ack = ack;
	waitingCommands[ack] = command;
	waitingIdentifiers[ack] = id;
	pthread_mutex_unlock(&mutex);
	sendCommandMessage();
}

void* SoarLcmCommunicator::lcmliteThreadFunction(void* arg){
	SoarLcmCommunicator* soarComm = (SoarLcmCommunicator*)arg;
	while(true){
		soarComm->wrapper.checkIncoming();
	}
	return 0;
}

void SoarLcmCommunicator::lcmHandler(lcmlite_t* lcm, const char* channel, const void* buf, int buf_len, void* user){
	SoarLcmCommunicator* comm = (SoarLcmCommunicator*)user;
	if(strcmp(comm->getInChannel(), channel) != 0){
		return;
	}

	IntBuffer params;
	uint offset = 0;
	unpackBuffer((const uchar*)buf, buf_len, params);

	int messageType = params[offset++];
	if(messageType == ACK_MESSAGE){
		comm->receiveAckMessage(params, offset);
	} else if(messageType == STATUS_MESSAGE){
		comm->receiveStatusMessage(params, offset);
	}
}

void SoarLcmCommunicator::receiveAckMessage(IntBuffer& buffer, uint& offset){
	//cout << "--> Soar Receive Ack" << endl;
	pthread_mutex_lock(&mutex);

	uint numAcks = buffer[offset++];
	for(uint i = 0; i < numAcks; i++){
		uint ack = buffer[offset++];
		CommandMapIt it = waitingCommands.find(ack);
		if(it != waitingCommands.end()){
			waitingCommands.erase(it);
		}
		IdentifierMap::iterator jt = waitingIdentifiers.find(ack);
		if(jt != waitingIdentifiers.end()){
			//cout << "Added "<< ack << " to finished ids " << jt->second->GetValueAsString() << endl;
			finishedIdentifiers.insert(jt->second);
			waitingIdentifiers.erase(jt);
		}
	}

	pthread_mutex_unlock(&mutex);
	//cout << "<-- Soar Receive Ack" << endl;
}

void SoarLcmCommunicator::receiveStatusMessage(IntBuffer& buffer, uint& offset){
	//cout << "--> Soar Receive Status" << endl;
	StatusList statuses;
	uint numStatuses = buffer[offset++];
	for(uint i = 0; i < numStatuses; i++){
		statuses.push_back(Ev3Status(buffer, offset));
	}
	soarManager->readStatus(statuses);
	//cout << "<-- Soar Receive Status" << endl;
}

void* SoarLcmCommunicator::sendCommandThreadFunction(void* arg){
	SoarLcmCommunicator* soarComm = (SoarLcmCommunicator*)arg;
	while(true){
		soarComm->sendCommandMessage();
		usleep(1000000/SOAR_SEND_COMMAND_FPS);
	}
	return 0;
}

void SoarLcmCommunicator::sendCommandMessage(){
	//cout << "--> Soar Send Command" << endl;
	// Send Message to Ev3 of all queued commands
	IntBuffer buffer;
	buffer.push_back(COMMAND_MESSAGE);

	// Add Commands to the message
	pthread_mutex_lock(&mutex);
	if(waitingCommands.size() == 0){
		//cout << "<-- Soar Send Command" << endl;
		pthread_mutex_unlock(&mutex);
		return;
	}
	buffer.push_back(waitingCommands.size());
	for(CommandMapIt i = waitingCommands.begin(); i != waitingCommands.end(); i++){
		i->second.packCommand(buffer);
	}
	pthread_mutex_unlock(&mutex);

	uchar* outBuffer;
	uint buf_size;
	packBuffer(buffer, outBuffer, buf_size);
	wrapper.publish(outChannel, (void*)outBuffer, buf_size);

	delete [] outBuffer;
	//cout << "<-- Soar Send Command" << endl;
}

void SoarLcmCommunicator::updateSoar(){
	pthread_mutex_lock(&mutex);
	for(IdentifierSet::iterator i = finishedIdentifiers.begin(); i != finishedIdentifiers.end(); i++){
		(*i)->CreateStringWME("status", "success");
	}
	finishedIdentifiers.clear();
	pthread_mutex_unlock(&mutex);
}

