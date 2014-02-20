
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#include "LcmliteWrapper.h"

void my_callback(lcmlite_t *lcm, const char *channel, const void *buf, int buf_len, void *user)
{
	printf("CALLBACK FOR %s\n", channel);
	printf("SIZE: %d\n", buf_len);
	int* buffer = new int[buf_len];
	memcpy((void*)buffer, buf, buf_len);
	int len = buf_len/sizeof(int);
	for(int i = 0; i < len; i++){
		int num = buffer[i];
		printf("%2d: %d\n", i, num);
	}
	delete [] buffer;
}


int main(){
	LcmliteWrapper wrapper;
	wrapper.init();

	wrapper.subscribe("SOARTOEV", my_callback, 0);

	while(1){
		cout << "CHECK" << endl;
		wrapper.checkIncoming();
	}
}
