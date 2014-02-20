#include <iostream>

#include "comm/DirectCommunication.h"
#include "ev3/Ev3Manager.h"
#include "soar/SoarManager.h"

using namespace std;


int main(int argc, char** argv){
	DirectCommunicator comm;
	Ev3Manager em;
	SoarManager sm((SoarCommunicator*)&comm, "test-agent.soar", false);
	comm.assignManagers(&sm, &em);
	comm.start();

	while(true){
		sm.step();
		usleep(10);
	}

	return 0;
}
