
#include <stdio.h>
#include <unistd.h>

#include "LcmliteWrapper.h"

int main(){
	LcmliteWrapper wrapper;
	
	wrapper.init();

	int* buf = new int[3];
	buf[0] = 0;
	buf[1] = 1;
	buf[2] = 0;
	while(true){
		wrapper.publish("EVTOSOAR", (void*)buf, 3*sizeof(int));
		sleep(2);
	}

}
