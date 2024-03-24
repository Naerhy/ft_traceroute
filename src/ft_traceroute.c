#include "ft_traceroute.h"

static int exit_traceroute(Tr* tr, char const* strerr)
{
	if (strerr)
		printf("ft_traceroute: %s\n", strerr);
	if (tr->udpsock != -1)
		close(tr->udpsock);
	if (tr->rawsock != -1)
		close(tr->rawsock);
	return strerr ? 1 : 0;
}

static int loop(Tr* tr)
{
	uint16_t ttl_start;
	char msg[9];
	ssize_t nbsent;
	struct sockaddr_in previous_recvaddr;

	ttl_start = tr->opt.ttl;
	strcpy(msg, "SUPERMAN");
	memset(&previous_recvaddr, 0, sizeof(previous_recvaddr));
	while (tr->opt.ttl <= tr->opt.max_ttl && !tr->end)
	{
		if (setsockopt(tr->udpsock, IPPROTO_IP, IP_TTL, &tr->opt.ttl, sizeof(tr->opt.ttl)) == -1)
			return 0;
		tr->host_addr.sin_port = htons(tr->opt.dest_port + tr->opt.ttl - ttl_start + 1);
		for (uint8_t i = 0; i < tr->opt.max_probes; i++)
		{
			nbsent = sendto(tr->udpsock, msg, strlen(msg) + 1, 0,
					(struct sockaddr*)&tr->host_addr, sizeof(tr->host_addr));
			if (nbsent == -1 || !recv_icmp(tr, i, &previous_recvaddr))
				return 0;
		}
		tr->opt.ttl++;
	}
	return 1;
}

int main(int argc, char** argv)
{
	Tr tr;

	init_traceroute(&tr);
	if (!parse_args(argc, argv, &tr))
		return exit_traceroute(&tr, "Invalid argument");
	else if (tr.opt.help)
	{
		print_help();
		return 0;
	}
	else if (!tr.host)
		return exit_traceroute(&tr, "Missing host operand");
	else if (!init_sockets(&tr))
		return exit_traceroute(&tr, tr.gai_errcode ? gai_strerror(tr.gai_errcode) : strerror(errno));
	else
	{
		printf("traceroute to %s (%s), %u hops max, 60 byte packets\n",
				tr.host, tr.host_ipstr, tr.opt.max_ttl);
		if (!loop(&tr))
			return exit_traceroute(&tr, tr.gai_errcode ? gai_strerror(tr.gai_errcode) : strerror(errno));
		return exit_traceroute(&tr, NULL);
	}
}
