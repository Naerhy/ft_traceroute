#include "ft_traceroute.h"

void init_traceroute(Tr* tr)
{
	tr->host = NULL;
	tr->flags = 0;
	tr->udpsock = -1;
	tr->rawsock = -1;
	tr->strerr = NULL;
}

int init_sockets(Tr* tr)
{
	struct addrinfo hints;
	struct addrinfo* res;
	int retgai;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	retgai = getaddrinfo(tr->host, "33434", &hints, &res);
	if (retgai)
	{
		// FIXME: still reachable leaks in Valgrind
		tr->strerr = gai_strerror(retgai);
		return 0;
	}
	memcpy(&tr->host_addr, res->ai_addr, sizeof(struct sockaddr_in));
	tr->udpsock = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);
	tr->rawsock = socket(hints.ai_family, SOCK_RAW, IPPROTO_ICMP);
	freeaddrinfo(res);
	if (tr->udpsock == -1 || tr->rawsock == -1)
	{
		tr->strerr = strerror(errno);
		return 0;
	}
	tr->host_ipstr = inet_ntoa(tr->host_addr.sin_addr);
	return 1;
}
