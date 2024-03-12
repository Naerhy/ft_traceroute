#include "ft_traceroute.h"

void init_traceroute(Tr* tr)
{
	tr->pid = getpid();
	tr->host = NULL;
	tr->flags = 0;
	tr->hops = 64;
	tr->nb_packets = 3;
	tr->ttl = 1;
	tr->waittime = 3;
	tr->tos = 0;
	tr->destport = 33434;
	tr->udpsock = -1;
	tr->rawsock = -1;
	tr->line_index = 1;
	tr->reached_dest = 0;
	tr->strerr = NULL;
}

static int bind_udpsock(int udpsock, uint16_t pid)
{
	struct sockaddr_in myaddr;

	myaddr.sin_family = AF_INET;
	// TODO: check if need to validate pid != reserved linux port
	myaddr.sin_port = htons(pid);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	memset(myaddr.sin_zero, 0, sizeof(myaddr.sin_zero));
	if (bind(udpsock, (struct sockaddr*)&myaddr, sizeof(myaddr)) == -1)
		return 0;
	return 1;
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
	retgai = getaddrinfo(tr->host, NULL, &hints, &res);
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
	if (tr->udpsock == -1 || tr->rawsock == -1 || !bind_udpsock(tr->udpsock, tr->pid))
	{
		tr->strerr = strerror(errno);
		return 0;
	}
	if (setsockopt(tr->udpsock, IPPROTO_IP, IP_TOS, &tr->tos, sizeof(tr->tos)) == -1)
		return 0;
	tr->host_ipstr = inet_ntoa(tr->host_addr.sin_addr);
	return 1;
}
