#include "LcmliteWrapper.h"

LcmliteWrapper::LcmliteWrapper(){
	numSubscriptions = 0;
	readFD = 0;
}

int LcmliteWrapper::init(){
	uint8_t mc_ttl = 0;
	struct in_addr mc_addr;
	int mc_port = htons(7667);

	if (inet_aton("239.255.76.67", (struct in_addr*) &mc_addr) < 0){
		perror("Didn't get an address\n");
		return 1;
		}

	// create the Multicast UDP socket
	struct sockaddr_in read_addr, send_addr;

	memset(&read_addr, 0, sizeof(read_addr));
	read_addr.sin_family = AF_INET;
	read_addr.sin_addr.s_addr = INADDR_ANY;
	read_addr.sin_port = mc_port;

	memset(&send_addr, 0, sizeof(read_addr));
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr = mc_addr;
	send_addr.sin_port = mc_port;

	readFD = socket(AF_INET, SOCK_DGRAM, 0);
	if (readFD < 0) {
		perror("socket()");
		return 1;
	}

	int opt = 1;
	if (setsockopt(readFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt(SOL_SOCKET, SO_REUSEADDR)");
		return 1;
	}

	#ifdef USE_REUSEPORT
	if (setsockopt(readFD, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
		perror("setsockopt(SOL_SOCKET, SO_REUSEPORT)");
		return 1;
	}
	#endif

	// join the multicast group
	struct ip_mreq mreq;
	mreq.imr_multiaddr = mc_addr;
	mreq.imr_interface.s_addr = INADDR_ANY;

	if (setsockopt(readFD, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
		perror ("setsockopt (IPPROTO_IP, IP_ADD_MEMBERSHIP)");
		return -1;
	}

	if (bind(readFD, (struct sockaddr*) &read_addr, sizeof(read_addr)) < 0) {
		perror("bind");
		return -1;
	}

	int send_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (setsockopt(readFD, IPPROTO_IP, IP_MULTICAST_TTL, &mc_ttl, sizeof(mc_ttl)) < 0) {
		perror("setsockopt(IPPROTO_IP, IP_MULTICAST_TTL)");
		return 1;
	}

	////////////////////////////////////////////////////////////////
	sendInfo.send_addr = send_addr;
	sendInfo.send_fd = send_fd;

	lcmlite_init(&lcm, LcmliteWrapper::transmitPacket, &sendInfo);
}

void LcmliteWrapper::transmitPacket(const void *_buf, int buf_len, void *user)
{
	TransmitInfo* tinfo = (TransmitInfo*)user;

    ssize_t res = sendto(tinfo->send_fd, _buf, buf_len, 0, (struct sockaddr*) &tinfo->send_addr, sizeof(tinfo->send_addr));
    if (res < 0)
        perror("transmit_packet: sendto");
}

void LcmliteWrapper::checkIncoming(){
	char buf[65536];
	struct sockaddr_in from_addr; // only IPv4 compatible
	socklen_t from_addr_sz = sizeof(from_addr);

	ssize_t buf_len = recvfrom(readFD, buf, sizeof(buf), 0, (struct sockaddr*) &from_addr, &from_addr_sz);
	assert(from_addr_sz == sizeof(struct sockaddr_in));

	int res = lcmlite_receive_packet(&lcm, buf, buf_len,
									 from_addr.sin_addr.s_addr | ((uint64_t) from_addr.sin_port << 32));
	if (res < 0)
		printf("ERR %d\n", res);

}

void LcmliteWrapper::publish(const char* channel, const void* buf, int buf_len){
	lcmlite_publish(&lcm, channel, buf, buf_len);
}

void LcmliteWrapper::subscribe(char* channel, void (*callback)(lcmlite_t* lcm, const char* channel, const void* buf, int buf_len, void* user), void* user){
	if(numSubscriptions == MAX_SUBSCRIPTIONS){
		return;
	}
	int i = numSubscriptions++;
	subscriptions[i].channel = new char[strlen(channel)+1];
	strcpy(subscriptions[i].channel, channel);
	subscriptions[i].callback = callback;
	subscriptions[i].user = user;

	lcmlite_subscribe(&lcm, &subscriptions[i]);
}

