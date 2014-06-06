#include "Motor.h"

#include "comm/SoarCommunication.h"

#include "WMUtil.h"
#include "LcmUtil.h"
#include "Constants.h"

#include "bytecodes.h"

using namespace sml;

Motor::Motor(uchar port, SoarCommunicator* comm)
:port(port), comm(comm), speed(0), tachoCount(0), tachoSensor(0), motorId(0){
	portStr = "A";
	portStr[0] += port;
}

void Motor::readStatus(IntBuffer& buffer, uint& offset){
	speed = buffer[offset++];
	tachoCount = buffer[offset++];
	tachoSensor = buffer[offset++];
}

void Motor::updateInputLink(Identifier* parentId){
	if(!motorId){
		motorId = parentId->CreateIdWME("motor");
		motorId->CreateStringWME("port", portStr.c_str());
	}
	WMUtil::updateIntWME(motorId, "speed", speed);

	if(tachoCount == -1){
		WMUtil::updateStringWME(motorId, "direction", "backward");
	} else if(tachoCount == 1){
		WMUtil::updateStringWME(motorId, "direction", "forward");
	} else {
		WMUtil::updateStringWME(motorId, "direction", "stopped");
	}

	WMUtil::updateIntWME(motorId, "amount-rotated", tachoSensor);
}

bool Motor::readSoarCommand(Identifier* commandId){
	Ev3Command command;
	command.dev = OUTPUT_MAN_DEV;

	// set-power command
	int newPower;
	bool setPower = WMUtil::getValue(commandId, "set-power", newPower);
	if(setPower){
		if(newPower == 0){
			command.params.push_back(packBytes(port, MOTOR_COMMAND_STOP, 0, 0));
		} else if(newPower > 0 && newPower <= 100){
			command.params.push_back(packBytes(port, MOTOR_COMMAND_START, 0, 0));
			command.params.push_back(packBytes(port, MOTOR_COMMAND_SET_POWER, (char)newPower, 0));
		} else {
			cout << "MOTOR " << portStr << " set-power " << newPower << " OUT OF RANGE (0-100)" << endl;
			return false;
		}
	}
		
	// set-direction command
	string newDir;
	bool setDir = WMUtil::getValue(commandId, "set-direction", newDir);
	if(setDir){
		char d;
		if(newDir == "forward"){
			d = MOTOR_DIR_FORWARD;
		} else if(newDir == "backward"){
			d = MOTOR_DIR_BACKWARD;
		} else {
			cout << "MOTOR " << portStr << " set-direction " << newDir << " INVALID" << endl;
			return false;
		}
		command.params.push_back(packBytes(port, MOTOR_COMMAND_SET_DIRECTION, d, 0));
	}

	if(setPower || setDir){
		comm->sendCommandToEv3(command, commandId);
		return true;
	} else {
		cout << "MOTOR " << portStr << " INVALID COMMAND" << endl;
		return false;
	}
}

