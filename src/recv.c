#include "ft_traceroute.h"

int recv_icmp(Tr* tr, size_t index, int* print_addr)
{
	fd_set fds;
	struct timeval start;
	struct timeval end;
	struct timeval timeout;
	int loop;
	int retselect;
	char buffer[1000];
	struct sockaddr_in recvaddr;
	socklen_t recvaddrlen;
	ssize_t nbrecv;

	FD_ZERO(&fds);
	FD_SET(tr->rawsock, &fds);
	if (gettimeofday(&start, NULL) == -1)
		return 0;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
	loop = 1;
	while (loop)
	{
		retselect = select(tr->rawsock + 1, &fds, NULL, NULL, &timeout);
		if (retselect == -1)
			return 0;
		else if (!retselect)
		{
			if (!index)
				fprintf(stderr, "%3u   *", tr->ttl);
			else
				fprintf(stderr, "  *");
			loop = 0;
		}
		else
		{
			// TODO: check if ICMP message is valid and matches sendto
			memset(buffer, 0, sizeof(buffer));
			recvaddrlen = sizeof(recvaddr);
			nbrecv = recvfrom(tr->rawsock, buffer, sizeof(buffer), 0,
					(struct sockaddr*)&recvaddr, &recvaddrlen);
			if (nbrecv == -1)
				return 0;
			if (gettimeofday(&end, NULL) == -1)
				return 0;
			// TODO: add missing timestamp calculation
			if (!index)
			{
				fprintf(stderr, "%3u   %s  xx,xxms", tr->ttl, inet_ntoa(recvaddr.sin_addr));
				*print_addr = 0;
			}
			else
			{
				if (*print_addr)
				{
					fprintf(stderr, "  %s  xx,xxms", inet_ntoa(recvaddr.sin_addr));
					*print_addr = 0;
				}
				else
					fprintf(stderr, "  xx,xxms");
			}
			loop = 0;
		}
	}
	if (index == 2)
		fprintf(stderr, "\n");
	return 1;
}
