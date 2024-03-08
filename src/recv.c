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

static uint16_t sw16(uint16_t v)
{
	return (v << 8) | (v >> 8);
}

// FIXME: check that identifier in packet is equal to our sent PID
static int valid_icmp_msg(char* buffer, ssize_t nbrecv)
{
	struct ip* ip;
	struct icmp* icmp;
	uint8_t hdrlen;

	ip = (struct ip*)buffer;
	if (nbrecv < IP_HDR_MIN_SIZE || nbrecv < sw16(ip->ip_len))
		return 0;
	hdrlen = ip->ip_hl * 4;
	if (hdrlen < IP_HDR_MIN_SIZE || hdrlen > IP_HDR_MAX_SIZE
			|| hdrlen + ICMP_MINLEN > sw16(ip->ip_len) || ip->ip_p != IPPROTO_ICMP)
		return 0;
	icmp = (struct icmp*)(buffer + hdrlen);
	if (icmp->icmp_type != ICMP_TIMXCEED && icmp->icmp_type != ICMP_UNREACH)
		return 0;
	return icmp->icmp_type == ICMP_TIMXCEED ? 1 : 2;
}

int recv_icmp(Tr* tr, size_t index, int* print_addr)
{
	fd_set fds;
	struct timeval start;
	struct timeval end;
	struct timeval timeout;
	int loop;
	int retselect;
	char buffer[IP_MSS];
	struct sockaddr_in recvaddr;
	socklen_t recvaddrlen;
	ssize_t nbrecv;
	int icmp_msg;
	Ts ts;

	FD_ZERO(&fds);
	FD_SET(tr->rawsock, &fds);
	if (gettimeofday(&start, NULL) == -1)
		return 0;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
	loop = 1;
	while (loop)
	{
		// TODO: subtract current elapsed time from timeout
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
			memset(buffer, 0, sizeof(buffer));
			recvaddrlen = sizeof(recvaddr);
			// TODO: improve this call to check if we received full packet with the help
			// of total length in IP header??
			nbrecv = recvfrom(tr->rawsock, buffer, sizeof(buffer), 0,
					(struct sockaddr*)&recvaddr, &recvaddrlen);
			if (nbrecv == -1)
				return 0;
			if (gettimeofday(&end, NULL) == -1)
				return 0;
			icmp_msg = valid_icmp_msg(buffer, nbrecv);
			if (icmp_msg)
			{
				// TODO: add missing timestamp calculation
				ts = convert_ts(timestamp_diff(&start, &end));
				if (!index)
				{
					fprintf(stderr, "%3u   %s  %lu,%lums", tr->ttl, inet_ntoa(recvaddr.sin_addr),
							ts.whole, ts.fract);
					*print_addr = 0;
				}
				else
				{
					if (*print_addr)
					{
						fprintf(stderr, "  %s  %lu,%lums", inet_ntoa(recvaddr.sin_addr),
								ts.whole, ts.fract);
						*print_addr = 0;
					}
					else
						fprintf(stderr, "  %lu,%lums", ts.whole, ts.fract);
				}
				loop = 0;
				if (icmp_msg == 2)
					tr->reached_dest = 1;
			}
		}
	}
	if (index == 2)
		fprintf(stderr, "\n");
	return 1;
}
