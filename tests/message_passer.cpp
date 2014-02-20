// file: listener.cpp
//
// LCM example program.
//
// compile with:
//  $ gcc -o listener listener.cpp -llcm
//
// On a system with pkg-config, you can also use:
//  $ gcc -o listener listener.cpp `pkg-config --cflags --libs lcm`

#include <stdio.h>
#include <vector>
#include <iostream>

#include <pthread.h>

#include <lcm/lcm-cpp.hpp>
#include "ev3_message_t.hpp"
#include "LcmliteWrapper.h"

using namespace std;

void lcmliteHandler(lcmlite_t* lcm, const char* channel, const void* buf, int buf_len, void* user);

class Handler {
public:
	static Handler* Singleton;

	Handler();

	~Handler();

	static void* lcmThreadFunction(void*);

	static void* lcmliteThreadFunction(void*);

	void lcmCallback(const lcm::ReceiveBuffer* rbuf, const string& channel, const ev3_message_t* msg); 

	static void lcmliteCallback(lcmlite_t* lcm, const char* channel, const void* buf, int buf_len, void* user);

	void handleJavaMessage(const ev3_message_t* msg);

	void handleSoarMessage(int* buffer, int buf_len);

private:
	LcmliteWrapper wrapper; 
	lcm::LCM lcm;
	pthread_t lcm_thread;
	pthread_t lcmlite_thread;
};

Handler* Handler::Singleton = 0;

Handler::Handler(){
	Handler::Singleton = this;

	wrapper.init();

	lcm.subscribe("JAVATOSOAR", &Handler::lcmCallback, this);
	wrapper.subscribe("SOARTOEV", Handler::lcmliteCallback, 0);

	pthread_create(&lcm_thread, 0, &lcmThreadFunction, 0);
	pthread_create(&lcmlite_thread, 0, &lcmliteThreadFunction, 0);
}

Handler::~Handler(){}

void* Handler::lcmThreadFunction(void*){
	while(!Singleton->lcm.handle());
}

void* Handler::lcmliteThreadFunction(void*){
	while(true){
		Singleton->wrapper.checkIncoming();
	}
}

void Handler::lcmCallback(const lcm::ReceiveBuffer* rbuf, const string& channel, const ev3_message_t* msg){
	Singleton->handleJavaMessage(msg);
}

void Handler::lcmliteCallback(lcmlite_t* lcm, const char* channel, const void* buf, int buf_len, void* user){
	int len = buf_len/sizeof(int);
	int* buffer = new int[len];
	memcpy((void*)buffer, buf, buf_len);

	Singleton->handleSoarMessage(buffer, len);
	delete [] buffer;
}

void Handler::handleJavaMessage(const ev3_message_t* msg){
	int* buffer = new int[msg->buffer_len];
	for(int i = 0; i < msg->buffer_len; i++){
		buffer[i] = msg->buffer[i];
	}
	wrapper.publish("EVTOSOAR", (void*)buffer, msg->buffer_len*sizeof(int));
	delete [] buffer;
}

void Handler::handleSoarMessage(int* buffer, int buf_len){
	ev3_message_t* msg = new ev3_message_t();
	msg->buffer_len = buf_len;
	for(int i = 0; i < buf_len; i++){
		msg->buffer.push_back(buffer[i]);
	}
	lcm.publish("SOARTOJAVA", msg);
	delete msg;
}

int main(int argc, char** argv)
{

    Handler handlerObject;
		while(true){
			sleep(10);
		}
			
    return 0;
}
