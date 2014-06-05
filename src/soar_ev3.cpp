#include <iostream>

#include "comm/SoarCommunication.h"
#include "soar/SoarManager.h"

#include "LcmUtil.h"

using namespace std;


int main(int argc, char** argv){
	if(argc < 3){
		cout << "Param 1: Channel (1-999)" << endl;
		cout << "Param 2: filepath for agent source file" << endl;
		return 0;
	}

	SoarLcmCommunicator comm(argv[1]);
	SoarManager manager(&comm, argv[2], true);
	comm.assignManager(&manager);
	comm.start();

	char c = -10;
	uint ui = c;
	int i = ui;
	cout << "C: " << i << endl;

	while(true){
//		manager.step();
		sleep(1);

	}

	return 0;
}
