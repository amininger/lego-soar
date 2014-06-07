#include <iostream>

#include "comm/DirectCommunication.h"
#include "ev3/Ev3Manager.h"
#include "soar/SoarManager.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 2){
		cout << "No soar file specified" << endl;
		return 0;
	}

	DirectCommunicator comm;
	Ev3Manager em;
SoarManager sm((SoarCommunicator*)&comm, argv[1], false);
	comm.assignManagers(&sm, &em);
	comm.start();

	while(true){
		sm.step();
	}

	return 0;
}
