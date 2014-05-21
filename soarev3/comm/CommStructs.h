/*
 * CommStructs.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef COMMSTRUCTS_H_
#define COMMSTRUCTS_H_


#include "Constants.h"

#include <pthread.h>
#include "LcmliteWrapper.h"
#include "LcmUtil.h"

// Vectors
#include <vector>

class Ev3Status;
typedef std::vector<Ev3Status> StatusList;

// Sets
#include <set>

typedef std::set<uint> AckSet;
typedef AckSet::iterator AckSetIt;

// Maps
#include <map>

class Ev3Command;
typedef std::map<uint, Ev3Command> CommandMap;
typedef CommandMap::iterator CommandMapIt;


class CommandHandler{
public:
	virtual ~CommandHandler(){};
	virtual void handleCommand(Ev3Command* command) = 0;
};

struct Ev3Command{
public:
	Ev3Command(): ack(0), dev(0){}

	Ev3Command(uint ack, ushort dev, const IntBuffer& params)
		:ack(ack), dev(dev), params(params){}

	void packCommand(IntBuffer& buffer);

	void unpackCommand(const IntBuffer& buffer, uint& offset);

	bool operator < (const Ev3Command& cmd2){
		return ack < cmd2.ack;
	}

	// Variables
	uint ack;
	ushort dev;
	IntBuffer params;
};

struct Ev3Status{
public:
	Ev3Status(const IntBuffer& buffer, uint& offset){
		unpackStatus(buffer, offset);
	}

	Ev3Status(ushort dev):dev(dev){}

	void packStatus(IntBuffer& buffer);

	void unpackStatus(const IntBuffer& buffer, uint& offset);

	bool operator < (const Ev3Status& s2){
		return dev < s2.dev;
	}

	// Variables
	ushort dev;
	IntBuffer info;
};

#endif /* COMMSTRUCTS_H_ */
