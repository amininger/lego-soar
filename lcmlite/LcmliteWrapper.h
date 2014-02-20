#ifndef _LCMLITE_WRAPPER_H
#define _LCMLITE_WRAPPER_H


#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <vector>

using std::vector;

#include "lcmlite.h"

#define MAX_SUBSCRIPTIONS 20

struct TransmitInfo{
	sockaddr_in send_addr;
	int send_fd;
};

class LcmliteWrapper{
	public:
		LcmliteWrapper();
		
		int init();

		void publish(const char* channel, const void* buf, int buf_len);

		void subscribe(char* channel, void (*callback)(lcmlite_t* lcm, const char* channel, const void* buf, int buf_len, void* user), void* user);

		void checkIncoming();

	private:
		static void transmitPacket(const void* buf, int buf_len, void* user);

		lcmlite_t lcm;
		TransmitInfo sendInfo;
		int readFD;
		int numSubscriptions;
		lcmlite_subscription_t subscriptions[MAX_SUBSCRIPTIONS];
};

#endif // _LCMLITE_WRAPPER_H
