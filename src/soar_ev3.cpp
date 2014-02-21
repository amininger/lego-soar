#include <iostream>

#include "comm/SoarCommunication.h"
#include "soar/SoarManager.h"

#include "LcmUtil.h"

using namespace std;


int main(int argc, char** argv){
	if(argc < 2){
		cout << "Needs agent source file" << endl;
		return 0;
	}

	SoarLcmCommunicator comm;
	SoarManager manager(&comm, argv[1], true);
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
