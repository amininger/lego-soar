/*
 * Ev3Brick.h
 *
 *  Created on: Nov 27, 2013
 *      Author: aaron
 */

#ifndef EV3BRICK_H_
#define EV3BRICK_H_

#include "comm/CommStructs.h"

#include "Constants.h"
#include "lms2012.h"

class Ev3Brick : public CommandHandler {
public:
	Ev3Brick();
	virtual ~Ev3Brick();
	void handleCommand(Ev3Command* command);
	Ev3Status getStatus();

private:
	int uiFile;
	UI* uiData;
};


#endif /* EV3BRICK_H_ */
