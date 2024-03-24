#include "ft_traceroute.h"

void init_traceroute(Tr* tr)
{
	tr->opt.max_probes = 3;
	tr->opt.ttl = 1;
	tr->opt.max_ttl = 30;
	tr->opt.wait_time = 3;
	tr->opt.tos = 0;
	tr->opt.dest_port = 33434;
	tr->opt.help = 0;
	tr->pid = getpid();
	tr->host = NULL;
	tr->udpsock = -1;
	tr->rawsock = -1;
	tr->end = 0;
	tr->gai_errcode = 0;
}

static int bind_udpsock(int udpsock, uint16_t pid)
{
	struct sockaddr_in myaddr;

	myaddr.sin_family = AF_INET;
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
		tr->gai_errcode = retgai;
		return 0;
	}
	memcpy(&tr->host_addr, res->ai_addr, sizeof(struct sockaddr_in));
	tr->udpsock = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);
	tr->rawsock = socket(hints.ai_family, SOCK_RAW, IPPROTO_ICMP);
	freeaddrinfo(res);
	if (tr->udpsock == -1 || tr->rawsock == -1 || !bind_udpsock(tr->udpsock, tr->pid))
		return 0;
	if (setsockopt(tr->udpsock, IPPROTO_IP, IP_TOS, &tr->opt.tos, sizeof(tr->opt.tos)) == -1)
		return 0;
	tr->host_ipstr = inet_ntoa(tr->host_addr.sin_addr);
	return 1;
}
