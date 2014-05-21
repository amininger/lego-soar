/*
 * CommStructs.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#include "CommStructs.h"

#include "LcmUtil.h"

// Ev3Status
void Ev3Status::packStatus(IntBuffer& buffer){
	buffer.push_back(packShorts(dev, info.size()));
	buffer.insert(buffer.end(), info.begin(), info.end());
}

void Ev3Status::unpackStatus(const IntBuffer& buffer, uint& offset){
	ushort len;
	unpackShorts(buffer[offset++], dev, len);
	for(int i = 0; i < len; i++){
		info.push_back(buffer[offset++]);
	}
}

// Ev3Command
void Ev3Command::packCommand(IntBuffer&buffer){
	buffer.push_back(ack);
	buffer.push_back(packShorts(dev, params.size()));
	buffer.insert(buffer.end(), params.begin(), params.end());
}

void Ev3Command::unpackCommand(const IntBuffer& buffer, uint& offset){
	ack = buffer[offset++];
	ushort len;
	unpackShorts(buffer[offset++], dev, len);
	for(int i = 0; i < len; i++){
		params.push_back(buffer[offset++]);
	}
}
