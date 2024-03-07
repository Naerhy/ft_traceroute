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

static void parse_args(int argc, char** argv, Tr* tr)
{
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(*(argv + i), "-?") || !strcmp(*(argv + i), "--help"))
			tr->flags |= HELP;
		else
			tr->host = *(argv + i);
	}
}

int main(int argc, char** argv)
{
	Tr tr;

	init_traceroute(&tr);
	parse_args(argc, argv, &tr);
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
	printf("traceroute to %s (%s), 64 hops max\n", tr.host, tr.host_ipstr);
	for (uint8_t i = 0; i < tr.hops && !tr.reached_dest; i++)
	{
		if (!send_udp(&tr))
			return exit_traceroute(strerror(errno), &tr);
	}
	return exit_traceroute(NULL, &tr);
}
