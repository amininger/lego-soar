#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "lms2012.h"
#include "unistd.h"

#include <iostream>
#include <string>
using namespace std;

std::string dec2bin(unsigned short n){
	const int size=sizeof(n)*8;
	std::string res = "";
	for(int i = size-1; i >= 0; i--){
		int s = 1 << i;
		res.push_back(n & s ? '1' : '0');
	}
	return res;
}

void readUartTypes(int file){
	cout << "Reading Uart Types" << endl;
	for(int i = 0; i < 4; i++){
		UARTCTL info;
		info.Port = i;
		info.Mode = 0;
		ioctl(file, UART_READ_MODE_INFO, &info);

		cout << "Port " << (i+1) << ": " << (int)info.TypeData.Type << endl;
	}
}

int main(){
	int file;
	UART *pUart;
	int dev = 0;

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

	int analogFile = open(ANALOG_DEVICE_NAME, O_RDWR | O_SYNC);
	if(analogFile == -1){
		cout << "Failed to open analog device" << endl;
		return -1;
	}

	ANALOG* analog = (ANALOG*)mmap(0, sizeof(ANALOG), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, analogFile, 0);
	if(analog == MAP_FAILED){
		cout << "failed to map analog device" << endl;
		close(file);
		close(analogFile);
		return -1;
	}

	for(int i = 0; i < 100; i++){
		for(int p = 0; p < 1; p++){
			//cout << "PORT " << p << endl;
			//cout << "  PIN1: " << (int)analog->InPin1[p] << endl;
			//cout << "  PIN6: " << (int)analog->InPin6[p] << endl;
			//cout << "  INDC: " << (int)analog->InDcm[p] << endl;
			//cout << "  INCN: " << (int)analog->InConn[p] << endl;
			//cout << "  OUTD: " << (int)analog->OutDcm[p] << endl;
			//cout << "  OUTC: " << (int)analog->OutConn[p] << endl;
			bool pressed = (analog->Pin6[p][analog->Actual[p]] > 1000);
			if(pressed){
				cout << "IN" << endl;
			} else {
				cout << "OUT" << endl;
			}
		}
		usleep(100000);
	}

	close(file);

	return 0;
}
