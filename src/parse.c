#include "ft_traceroute.h"

static int validate_arg(char const* opt, uint32_t value, Tr* tr)
{
	if (!strcmp(opt, "-q") && value && value <= 10)
	{
		tr->nb_packets = value;
		return 1;
	}
	else if (!strcmp(opt, "-m") && value && value <= 255)
	{
		tr->hops = value;
		return 1;
	}
	else if (!strcmp(opt, "-f") && value && value <= 255)
	{
		tr->ttl = value;
		return 1;
	}
	else if (!strcmp(opt, "-w") && value <= 60)
	{
		tr->waittime = value;
		return 1;
	}
	else if (!strcmp(opt, "-t") && value <= 255)
	{
		tr->tos = value;
		return 1;
	}
	else if (!strcmp(opt, "-p") && value && value <= 65535)
	{
		tr->destport = value;
		return 1;
	}
	else
		return 0;
}

int parse_args(int argc, char** argv, Tr* tr)
{
	uint32_t value;

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(*(argv + i), "-?") || !strcmp(*(argv + i), "--help"))
			tr->flags |= HELP;
		else if (!strcmp(*(argv + i), "--dns"))
			tr->flags |= DNS;
		else if (!strcmp(*(argv + i), "-q") || !strcmp(*(argv + i), "-m")
				|| !strcmp(*(argv + i), "-f") || !strcmp(*(argv + i), "-w")
				|| !strcmp(*(argv + i), "-t") || !strcmp(*(argv + i), "-p"))
		{
			if (!*(argv + i + 1) || !ft_atoi(*(argv + i + 1), &value))
				return 0;
			if (!validate_arg(*(argv + i), value, tr))
				return 0;
			i++;
		}
		else
			tr->host = *(argv + i);
	}
	return 1;
}
