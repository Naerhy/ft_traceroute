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

#define IP_HDR_MIN_SIZE 20
#define IP_HDR_MAX_SIZE 60
#define UDP_HDR_MIN_SIZE 8

typedef struct Opt
{
	uint8_t max_probes;
	uint16_t ttl;
	uint16_t max_ttl;
	uint8_t wait_time;
	uint8_t tos;
	uint16_t dest_port;
	uint8_t help;
} Opt;

typedef struct Tr
{
	Opt opt;
	uint16_t pid;
	char* host;
	struct sockaddr_in host_addr;
	char* host_ipstr;
	int udpsock;
	int rawsock;
	int end;
	int gai_errcode;
} Tr;

typedef struct Ts
{
	uint64_t whole;
	uint64_t fract;
} Ts;

void init_traceroute(Tr* tr);
int init_sockets(Tr* tr);

int parse_args(int argc, char** argv, Tr* tr);

int recv_icmp(Tr* tr, uint8_t probe, struct sockaddr_in* previous_recvaddr);

void print_help(void);
void print_timeout(uint8_t probe, uint16_t ttl);
void print_packet(uint8_t probe, uint16_t ttl, struct in_addr* previous_addr,
		struct in_addr* addr, Ts* ts, char const* host);

size_t ft_strlen(char const* s);
int ft_atoi(char const* s, uint32_t* res);

#endif
