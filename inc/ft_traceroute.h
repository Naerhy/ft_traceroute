#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>

typedef struct Traceroute
{
	char* host;
	int sockfd;
} Traceroute;

#endif
