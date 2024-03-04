#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define HELP (1 << 0)

typedef struct Traceroute
{
	char* host;
	uint8_t flags;
	int udpsock;
	int rawsock;
	struct sockaddr_in host_addr;
	char* host_ipstr;
	char const* strerr;
} Traceroute;

void init_traceroute(Traceroute* traceroute);
int init_sockets(Traceroute* traceroute);

void print_help(void);

#endif
