#include "ft_traceroute.h"

int send_udp(Tr* tr)
{
	char msg[9];
	ssize_t nbsent;
	int print_addr;

	strcpy(msg, "SUPERMAN");
	if (setsockopt(tr->udpsock, IPPROTO_IP, IP_TTL, &tr->ttl, sizeof(tr->ttl)) == -1)
		return 0;
	print_addr = 1;
	tr->host_addr.sin_port = htons(UDP_PORT + tr->ttl - 1);
	for (size_t i = 0; i < 3; i++)
	{
		nbsent = sendto(tr->udpsock, msg, strlen(msg) + 1, 0,
				(struct sockaddr*)&tr->host_addr, sizeof(tr->host_addr));
		if (nbsent == -1 || !recv_icmp(tr, i, &print_addr))
			return 0;
	}
	tr->ttl++;
	tr->line_index++;
	return 1;
}
