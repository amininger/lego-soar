/*
 * Ev3Manager.h
 *
 *  Created on: Nov 20, 2013
 *      Author: aaron
 */

#ifndef EV3MANAGER_H_
#define EV3MANAGER_H_

#include "Constants.h"
#include "comm/CommStructs.h"

#include "Ev3Brick.h"
#include "OutputDeviceManager.h"
#include "InputDeviceManager.h"

#include <map>
typedef std::map<int, CommandHandler*> CommandHandlers;
typedef CommandHandlers::iterator CommandHandlersIt;

class Ev3Manager : public CommandHandler {
public:
	Ev3Manager();
	virtual ~Ev3Manager();

	void writeStatus(StatusList& statuses);

	void executeEv3Command(Ev3Command* command);

	void handleEv3Command(Ev3Command* command);

private:
	Ev3Status getStatus();
	void handleCommand(Ev3Command* command);

	Ev3Brick* brick;
	InputDeviceManager* inputDevices;
	OutputDeviceManager* outputDevices;

	CommandHandlers commandHandlers;
};

#endif /* EV3MANAGER_H_ */
