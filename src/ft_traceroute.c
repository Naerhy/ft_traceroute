#include "ft_traceroute.h"

static int exit_traceroute(char const* strerr, Traceroute* traceroute)
{
	if (strerr)
		printf("ft_traceroute: %s\n", strerr);
	if (traceroute->socket != -1)
		close(traceroute->socket);
	return strerr ? 1 : 0;
}

static void parse_args(int argc, char** argv, Traceroute* traceroute)
{
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(*(argv + i), "-?") || !strcmp(*(argv + i), "--help"))
			traceroute->flags |= HELP;
		else
			traceroute->host = *(argv + i);
	}
}

int main(int argc, char** argv)
{
	Traceroute traceroute;

	init_traceroute(&traceroute);
	parse_args(argc, argv, &traceroute);
	if (traceroute.flags & HELP)
	{
		print_help();
		return 0;
	}
	if (!traceroute.host)
		return exit_traceroute("Missing host operand", &traceroute);
	if (!init_socket(&traceroute))
		return exit_traceroute(traceroute.strerr, &traceroute);
	return 0;
}
