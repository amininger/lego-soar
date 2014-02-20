
#include <stdio.h>
#include <unistd.h>

#include "lcmlite_wrapper.h"

void my_callback(lcmlite_t *lcm, const char *channel, const void *buf, int buf_len, void *user)
{
	printf("CALLBACK FOR %s\n", channel);
	printf("%s\n", (char*)buf);

}


int main(){
	lcmlite_wrapper wrapper;
	
	lcmlite_wrapper_init(&wrapper);

	lcmlite_wrapper_subscribe(&wrapper, "CHANNEL1", my_callback, NULL);

	while(1){
		lcmlite_wrapper_check_incoming(&wrapper);
		sleep(50);
	}

	//char buf[8] = {'M', 'e', 's', 's', 'a', 'g', 'e', '\0'};
	//lcmlite_publish(&wrapper.lcm, "CHANNEL1", &buf, 8);

}
