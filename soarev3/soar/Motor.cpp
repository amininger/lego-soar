#include "Motor.h"

#include "comm/SoarCommunication.h"

#include "WMUtil.h"
#include "LcmUtil.h"
#include "Constants.h"

#include "bytecodes.h"

using namespace sml;

Motor::Motor(uchar port, SoarCommunicator* comm)
:port(port), comm(comm), speed(0), tachoCount(0), tachoSensor(0), motorId(0){

}

void Motor::readStatus(IntBuffer& buffer, uint& offset){
	speed = buffer[offset++];
	tachoCount = buffer[offset++];
	tachoSensor = buffer[offset++];
}

void Motor::updateInputLink(Identifier* parentId){
	if(!motorId){
		motorId = parentId->CreateIdWME("motor");
		motorId->CreateIntWME("port", port);
	}
	WMUtil::updateIntWME(motorId, "speed", speed);
	WMUtil::updateIntWME(motorId, "tach", tachoCount);
	WMUtil::updateIntWME(motorId, "sensor", tachoSensor);
}

bool Motor::readSoarCommand(Identifier* commandId){
	Identifier* childId;

	Ev3Command command;
	command.dev = OUTPUT_MAN_DEV;
	if(WMUtil::getValue(commandId, "start", childId)){
		if(!handleStartCommand(command.params, childId)){
			return false;
		}
	} else if(WMUtil::getValue(commandId, "set", childId)){
		if(!handleSetCommand(command.params, childId)){
			return false;
		}
	} else if(WMUtil::getValue(commandId, "stop", childId)){
		if(!handleStopCommand(command.params, childId)){
			return false;
		}
	} else {
		commandId->CreateStringWME("error", "Unrecognized command");
		return false;
	}

	comm->sendCommandToEv3(command, commandId);
	return true;
}

bool Motor::handleStartCommand(IntBuffer& buffer, Identifier* commandId){
	buffer.push_back(packBytes(port-1, MOTOR_COMMAND_START, 0, 0));
	return handleSetCommand(buffer, commandId);
}

bool Motor::handleStopCommand(IntBuffer& buffer, Identifier* commandId){
	buffer.push_back(packBytes(port-1, MOTOR_COMMAND_STOP, 0, 0));
	return true;
}

bool Motor::handleSetCommand(IntBuffer& buffer, Identifier* commandId){
	string dir;
	if(WMUtil::getValue(commandId, "direction", dir)){
		char d;
		if(dir == "forward"){
			d = MOTOR_DIR_FORWARD;
		} else if(dir == "backward"){
			d = MOTOR_DIR_BACKWARD;
		} else {
			commandId->CreateStringWME("error", "Unrecognized direction");
			return false;
		}
		//cout << "Set motor " << (short)port << " direction to " << dir << endl;
		buffer.push_back(packBytes(port-1, MOTOR_COMMAND_SET_DIRECTION, d, 0));
	}

	int speed;
	if(WMUtil::getValue(commandId, "speed", speed)){
		if(speed >= -100 && speed <= 100){
		//	cout << " Set motor " << (short)port << " speed to " << speed << endl;
			buffer.push_back(packBytes(port-1, MOTOR_COMMAND_SET_SPEED, (char)speed, 0));
		} else {
			commandId->CreateStringWME("error", "Speed must be between -100 and 100");
			return false;
		}
	}

	int power;
	if(WMUtil::getValue(commandId, "power", power)){
		if(power >= -100 && power <= 100){
			buffer.push_back(packBytes(port-1, MOTOR_COMMAND_SET_POWER, (char)power, 0));
		} else {
			commandId->CreateStringWME("error", "Power must be between -100 and 100");
			return false;
		}
	}

	return true;
}
