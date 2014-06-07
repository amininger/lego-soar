#include <iostream>

#include "comm/Ev3Communication.h"
#include "ev3/Ev3Manager.h"

using namespace std;


int main(int argc, char** argv){
	if(argc < 2){
		cout << "Param 1: Channel (1-999)" << endl;
		return 0;
	}


	Ev3LcmCommunicator comm(argv[1]);
	Ev3Manager manager;
	comm.assignManager(&manager);
	comm.start();

	while(true){
		sleep(10);
	}

	return 0;
}
