#include <iostream>

#include "comm/Ev3Communication.h"
#include "ev3/Ev3Manager.h"

using namespace std;


int main(int argc, char** argv){
	Ev3LcmCommunicator comm;
	Ev3Manager manager;
	comm.assignManager(&manager);
	comm.start();

	while(true){
		sleep(10);
	}

	return 0;
}
