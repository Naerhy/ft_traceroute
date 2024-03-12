#include "ft_traceroute.h"

void print_help(void)
{
	printf("Usage: traceroute [OPTION...] HOST\n");
	printf("Print the route packets trace to network host.\n\n");
	printf("  -?, --help                  give this help list\n");
}

void print_timeout(size_t index, uint8_t line_index)
{
	if (!index)
		fprintf(stderr, "%3u   *", line_index);
	else
		fprintf(stderr, "  *");
}

void print_packet(size_t index, uint8_t line_index, struct in_addr* addr, Ts* ts, int* print_addr)
{
	if (!index)
	{
		fprintf(stderr, "%3u   %s  %lu,%lums", line_index, inet_ntoa(*addr),
				ts->whole, ts->fract);
		*print_addr = 0;
	}
	else
	{
		if (*print_addr)
		{
			fprintf(stderr, "  %s  %lu,%lums", inet_ntoa(*addr),
					ts->whole, ts->fract);
			*print_addr = 0;
		}
		else
			fprintf(stderr, "  %lu,%lums", ts->whole, ts->fract);
	}
}
