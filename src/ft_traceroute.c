#include "ft_traceroute.h"

static void print_help(void)
{
	printf("Usage: traceroute [OPTION...] HOST\n");
	printf("Print the route packets trace to network host.\n\n");
	printf("  -?, --help                  give this help list\n");
}

static int exit_traceroute(char const* errstr)
{
	if (errstr)
		printf("ft_ping: %s\n", errstr);
	return errstr ? 1 : 0;
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

static void init_traceroute(Traceroute* traceroute)
{
	traceroute->flags = 0;
}

int main(int argc, char** argv)
{
	Traceroute traceroute;

	if (argc < 2)
		return exit_traceroute("missing host operand");
	init_traceroute(&traceroute);
	parse_args(argc, argv, &traceroute);
	if (traceroute.flags & HELP)
	{
		print_help();
		return 0;
	}
	return 0;
}
