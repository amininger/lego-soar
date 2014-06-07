/*
 * SoarManager.h
 *
 *  Created on: Nov 20, 2013
 *      Author: aaron
 */

#ifndef SOARMANAGER_H_
#define SOARMANAGER_H_

#include "comm/CommStructs.h"
#include "SoarDevice.h"
#include "Brick.h"
#include "Motor.h"

#include "sml_Client.h"

#include <string>
#include <map>
typedef std::map<uint, Motor*> OutputDevices;
typedef std::map<uint, SoarDevice*> InputDevices;

class SoarManager {
public:
	SoarManager(SoarCommunicator* comm, std::string agentSource, bool debugger);
	virtual ~SoarManager();
	static SoarManager* singleton;

	void readStatus(StatusList& statuses);

	void step();

	// input phase callback
	static void runEventHandler(sml::smlRunEventId eventID, void* data, sml::Agent* agent, sml::smlPhase phase);

	// output link event callback
	static void outputEventHandler(void* data, sml::Agent* agent, const char* attName,  sml::WMElement* wme);

	void sendSoarMessage();
	void handleOutput(std::string attName, sml::WMElement* wme);
private:
	void initAgent(const char* agentSource);
	void readSensorsStatus(IntBuffer& status);
	void readMotorsStatus(IntBuffer& status);
	void createSensor(uint port, uint type);
	void updateInputLink(sml::Identifier* inputLink);
	void addOutputListeners();

	bool readSoarCommand(sml::Identifier* id);
	bool handleCreateSensorCommand(sml::Identifier* id);
	bool handleDeleteSensorCommand(sml::Identifier* id);

	SoarCommunicator* comm;

	Brick* brick;
	Motor* motors[4];
	SoarDevice* inputs[4];

	// Soar agent variables
	sml::Kernel* kernel;
	sml::Agent* agent;
	int timeStep;
};

#endif /* SOARMANAGER_H_ */

