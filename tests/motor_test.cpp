#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include "lms2012.h"
#include <iostream>
#include <unistd.h>
using namespace std;


const int MOTOR_SPEED = 10;

const char PORT_1 = 0x01;
const char PORT_2 = 0x02;

const int MAX_READINGS = 10000;

int main(){
	MOTORDATA* motorData;
	char motor_command[4];
	int motorFile;
	int encoderFile;

	motorFile = open(PWM_DEVICE_NAME, O_WRONLY);
	if(motorFile == -1){
		cout << "Failed to open motor file\n";
		return -1;
	}
	cout << "Opened File: " << motorFile << endl;
	
	encoderFile = open(MOTOR_DEVICE_NAME, O_RDWR | O_SYNC);
	if(encoderFile == -1){
		cout << "Failed to open encoder file\n";
		close(motorFile);
		return -1;
	}
	cout << "Opened File: " << encoderFile << endl;

	motorData = (MOTORDATA*)mmap(0, sizeof(MOTORDATA)*vmOUTPUTS, 
							PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, encoderFile, 0);

	if(motorData == MAP_FAILED){
		cout << "Failed to map file\n";
		close(motorFile);
		close(encoderFile);
		return -1;
	}

	motor_command[0] = opOUTPUT_SPEED;
	motor_command[1] = PORT_1 | PORT_2;
	motor_command[2] = MOTOR_SPEED;
	write(motorFile, motor_command, 3);

	motor_command[0] = opOUTPUT_START;
	motor_command[1] = PORT_1 | PORT_2;
	write(motorFile, motor_command, 2);

	for(int i = 0; i < 10; i++){
		cout << "Time: " << i << endl;

		int s = motorData[0].Speed;
		int tc = motorData[0].TachoCounts;
		int ts = motorData[0].TachoSensor;

		cout << "Speed: " << s << endl;
		cout << "Count: " << tc << endl;
		cout << "Sensor: " << ts << endl;

		sleep(1);
	}

	motor_command[0] = opOUTPUT_STOP;
	motor_command[1] = PORT_1 | PORT_2;
	write(motorFile, motor_command, 2);

	close(encoderFile);
	close(motorFile);

	return 0;
}
