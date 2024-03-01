#include "ft_traceroute.h"

void init_traceroute(Traceroute* traceroute)
{
	traceroute->host = NULL;
	traceroute->flags = 0;
	traceroute->socket = -1;
	traceroute->strerr = NULL;
}

int init_socket(Traceroute* traceroute)
{
	struct addrinfo hints;
	struct addrinfo* res;
	int retgai;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	retgai = getaddrinfo(traceroute->host, NULL, &hints, &res);
	if (retgai)
	{
		// FIXME: still reachable leaks in Valgrind
		traceroute->strerr = gai_strerror(retgai);
		return 0;
	}
	memcpy(&traceroute->host_addr, res->ai_addr, sizeof(struct sockaddr_in));
	traceroute->socket = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);
	freeaddrinfo(res);
	if (traceroute->socket == -1)
	{
		traceroute->strerr = strerror(errno);
		return 0;
	}
	traceroute->host_ipstr = inet_ntoa(traceroute->host_addr.sin_addr);
	return 1;
}
