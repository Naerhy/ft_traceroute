#include "ft_traceroute.h"

static int exit_traceroute(char const* strerr, Tr* tr)
{
	if (strerr)
		printf("ft_traceroute: %s\n", strerr);
	if (tr->udpsock != -1)
		close(tr->udpsock);
	if (tr->rawsock != -1)
		close(tr->rawsock);
	return strerr ? 1 : 0;
}

static int parse_args(int argc, char** argv, Tr* tr)
{
	uint32_t value;

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(*(argv + i), "-?") || !strcmp(*(argv + i), "--help"))
			tr->flags |= HELP;
		else if (!strcmp(*(argv + i), "-q"))
		{
			if (!*(argv + i + 1))
				return 0;
			if (!ft_atoi(*(argv + i + 1), &value) || !value || value > 10)
				return 0;
			tr->nb_packets = value;
			i++;
		}
		else if (!strcmp(*(argv + i), "-m"))
		{
			if (!*(argv + i + 1))
				return 0;
			if (!ft_atoi(*(argv + i + 1), &value) || !value || value > 255)
				return 0;
			tr->hops = value;
			i++;
		}
		else if (!strcmp(*(argv + i), "-f"))
		{
			if (!*(argv + i + 1))
				return 0;
			if (!ft_atoi(*(argv + i + 1), &value) || !value || value > 255)
				return 0;
			tr->ttl = value;
			i++;
		}
		else if (!strcmp(*(argv + i), "-w"))
		{
			if (!*(argv + i + 1))
				return 0;
			if (!ft_atoi(*(argv + i + 1), &value) || value > 60)
				return 0;
			tr->waittime = value;
			i++;
		}
		else if (!strcmp(*(argv + i), "-t"))
		{
			if (!*(argv + i + 1))
				return 0;
			if (!ft_atoi(*(argv + i + 1), &value) || value > 255)
				return 0;
			tr->tos = value;
			i++;
		}
		else if (!strcmp(*(argv + i), "-p"))
		{
			if (!*(argv + i + 1))
				return 0;
			if (!ft_atoi(*(argv + i + 1), &value) || !value || value > 65535)
				return 0;
			tr->destport = value;
			i++;
		}
		else
			tr->host = *(argv + i);
	}
	return 1;
}

int main(int argc, char** argv)
{
	Tr tr;

	init_traceroute(&tr);
	if (!parse_args(argc, argv, &tr))
		return exit_traceroute("Invalid argument", &tr);
	if (tr.flags & HELP)
	{
		print_help();
		return 0;
	}
	if (!tr.host)
		return exit_traceroute("Missing host operand", &tr);
	if (!init_sockets(&tr))
		return exit_traceroute(tr.strerr, &tr);
	// TODO: replace placeholder number (64) with variable value
	printf("traceroute to %s (%s), %u hops max\n", tr.host, tr.host_ipstr, tr.hops);
	for (uint8_t i = 0; i < tr.hops && !tr.reached_dest; i++)
	{
		if (!send_udp(&tr))
			return exit_traceroute(strerror(errno), &tr);
	}
	return exit_traceroute(NULL, &tr);
}
