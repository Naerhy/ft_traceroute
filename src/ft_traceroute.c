#include "ft_traceroute.h"

static int exit_traceroute(char const* errstr)
{
	printf("ft_ping: %s\n", errstr);
	return errstr ? 1 : 0;
}

static void parse_args(int argc, char** argv, Traceroute* traceroute)
{
	traceroute->host = *(argv + argc - 1);
}

int main(int argc, char** argv)
{
	Traceroute traceroute;

	if (argc < 2)
		return exit_traceroute("missing host operand");
	parse_args(argc, argv, &traceroute);
	return 0;
}
