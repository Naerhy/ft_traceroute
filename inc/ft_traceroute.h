#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define HELP (1 << 0)

typedef struct Tr
{
	char* host;
	uint8_t flags;
	uint8_t hops;
	uint8_t ttl;
	int udpsock;
	int rawsock;
	struct sockaddr_in host_addr;
	char* host_ipstr;
	char const* strerr;
} Tr;

void init_traceroute(Tr* tr);
int init_sockets(Tr* tr);

int send_udp(Tr* tr);

int recv_icmp(Tr* tr, size_t index, int* print_addr);

void print_help(void);

#endif
