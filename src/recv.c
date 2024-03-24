#include "ft_traceroute.h"

static Ts convert_ts(double nb)
{
	Ts ts;

	ts.whole = (uint64_t)nb;
	ts.fract = (uint64_t)((nb - ts.whole) * 1000);
	return ts;
}

static double timestamp_diff(struct timeval* start, struct timeval* end)
{
	return (((double)end->tv_sec * 1000) + ((double)end->tv_usec / 1000))
			- (((double)start->tv_sec * 1000) + ((double)start->tv_usec / 1000));
}

static int valid_icmp_msg(char* buffer, ssize_t nbrecv, uint16_t pid)
{
	struct ip* ip;
	struct icmp* icmp;
	uint8_t hdrlen;
	uint16_t srcport;

	ip = (struct ip*)buffer;
	if (nbrecv < IP_HDR_MIN_SIZE || nbrecv != ntohs(ip->ip_len))
		return 0;
	hdrlen = ip->ip_hl * 4;
	if (hdrlen < IP_HDR_MIN_SIZE || hdrlen > IP_HDR_MAX_SIZE
			|| hdrlen + ICMP_MINLEN + hdrlen + UDP_HDR_MIN_SIZE > nbrecv
			|| ip->ip_p != IPPROTO_ICMP)
		return 0;
	icmp = (struct icmp*)(buffer + hdrlen);
	if (icmp->icmp_type != ICMP_TIMXCEED && icmp->icmp_type != ICMP_UNREACH)
		return 0;
	srcport = ntohs(*((uint16_t*)(buffer + hdrlen + ICMP_MINLEN + hdrlen)));
	if (srcport != pid)
		return 0;
	return icmp->icmp_type == ICMP_TIMXCEED ? 1 : 2;
}

int recv_icmp(Tr* tr, uint8_t probe, struct sockaddr_in* previous_recvaddr)
{
	fd_set fds;
	struct timeval start;
	struct timeval end;
	struct timeval timeout;
	int retselect;
	char buffer[IP_MSS];
	struct sockaddr_in recvaddr;
	socklen_t recvaddrlen;
	ssize_t nbrecv;
	int icmp_msg;
	Ts ts;
	char host[1024];

	FD_ZERO(&fds);
	FD_SET(tr->rawsock, &fds);
	if (gettimeofday(&start, NULL) == -1)
		return 0;
	timeout.tv_sec = tr->opt.wait_time;
	timeout.tv_usec = 0;
	while (1)
	{
		retselect = select(tr->rawsock + 1, &fds, NULL, NULL, &timeout);
		if (retselect == -1)
			return 0;
		else if (!retselect)
		{
			print_timeout(probe, tr->opt.ttl);
			break;
		}
		else
		{
			memset(buffer, 0, sizeof(buffer));
			recvaddrlen = sizeof(recvaddr);
			nbrecv = recvfrom(tr->rawsock, buffer, sizeof(buffer), 0, (struct sockaddr*)&recvaddr, &recvaddrlen);
			if (nbrecv == -1 || gettimeofday(&end, NULL) == -1)
				return 0;
			icmp_msg = valid_icmp_msg(buffer, nbrecv, tr->pid);
			if (icmp_msg)
			{
				ts = convert_ts(timestamp_diff(&start, &end));
				memset(host, 0, sizeof(host));
				if (getnameinfo((struct sockaddr*)&recvaddr, recvaddrlen, host, sizeof(host), NULL, 0, 0))
					return 0;
				print_packet(probe, tr->opt.ttl, &previous_recvaddr->sin_addr, &recvaddr.sin_addr,
						&ts, host);
				if (icmp_msg == 2)
					tr->end = 1;
				memcpy(previous_recvaddr, &recvaddr, recvaddrlen);
				break;
			}
		}
	}
	if (probe == tr->opt.max_probes - 1)
		fprintf(stderr, "\n");
	return 1;
}
