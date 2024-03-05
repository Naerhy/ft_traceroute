#include "ft_traceroute.h"

int send_udp(Tr* tr)
{
	char msg[9];
	ssize_t nbsent;

	strcpy(msg, "SUPERMAN");
	if (setsockopt(tr->udpsock, IPPROTO_IP, IP_TTL, &tr->ttl, sizeof(tr->ttl)) == -1)
		return 0;
	for (size_t i = 0; i < 3; i++)
	{
		nbsent = sendto(tr->udpsock, msg, strlen(msg) + 1, 0,
				(struct sockaddr*)&tr->host_addr, sizeof(tr->host_addr));
		if (nbsent == -1)
			return 0;
		printf("message has been sent\n");
	}
	tr->ttl++;
	return 1;
}
