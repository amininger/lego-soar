#include <iostream>

#include "comm/SoarCommunication.h"
#include "soar/SoarManager.h"


using namespace std;

int main(){
	SoarLcmCommunicator comm;
	SoarManager manager(&comm, "../src/agents/test_remote.soar", true);
	comm.assignManager(&manager);
	comm.start();

	while(true){
		sleep(1000);
	}

	return 0;
}
