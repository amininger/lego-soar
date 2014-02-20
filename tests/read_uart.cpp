#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "lms2012.h"
#include "unistd.h"

#include <iostream>
using namespace std;

int main(){
	int file;
	UART pUart;
	int dev = 0;

	char* byte = (char*)&pUart;
	for(int i = 0; i < sizeof(pUart); i++){
		cout << byte << endl;
		byte++;
		
		if(i % 6 == 0){
			cout << endl;
		}
	}

	return 0;

	file = open(UART_DEVICE_NAME, O_RDWR | O_SYNC);
	if(file == -1){
		cout << "Failed to open device" << endl;
		return -1;
	}

	pUart = (UART*)mmap(0, sizeof(UART), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, file, 0);
	if(pUart == MAP_FAILED){
		cout << "Failed to map device" << endl;
		close(file);
		return -1;
	}



	close(file);

	return 0;
}
