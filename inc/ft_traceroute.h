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
#define HELP (1 << 0)

// TODO: args
// -q => nb or probe packets
// -f => initial ttl
// -m => max ttl
// -p => base destination port (still incremented)
// -w => time in sec to wait for a response
// -t => set type of service

typedef struct Tr
{
	uint16_t pid;
	char* host;
	uint8_t flags;
	uint8_t hops;
	uint8_t nb_packets;
	uint8_t ttl;
	uint8_t waittime;
	uint8_t tos;
	uint16_t destport;
	int udpsock;
	int rawsock;
	struct sockaddr_in host_addr;
	char* host_ipstr;
	uint8_t line_index;
	int reached_dest;
	char const* strerr;
} Tr;

typedef struct Ts
{
	uint64_t whole;
	uint64_t fract;
} Ts;

void init_traceroute(Tr* tr);
int init_sockets(Tr* tr);

int send_udp(Tr* tr);

int recv_icmp(Tr* tr, uint8_t index, int* print_addr);

void print_help(void);
void print_timeout(size_t index, uint8_t line_index);
void print_packet(size_t index, uint8_t line_index, struct in_addr* addr, Ts* ts, int* print_addr);

int ft_atoi(char const* s, uint32_t* res);

#endif
