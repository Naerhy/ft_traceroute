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
	printf("traceroute to %s (%s), %u hops max\n", tr.host, tr.host_ipstr, tr.hops);
	for (uint8_t i = 0; i < tr.hops && !tr.reached_dest; i++)
	{
		if (!send_udp(&tr))
			return exit_traceroute(strerror(errno), &tr);
	}
	return exit_traceroute(NULL, &tr);
}
