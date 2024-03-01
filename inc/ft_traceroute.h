#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define HELP (1 << 0)

typedef struct Traceroute
{
	char* host;
	uint8_t flags;
} Traceroute;

#endif
