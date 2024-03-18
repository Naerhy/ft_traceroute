#include "ft_traceroute.h"

void print_help(void)
{
	printf("Usage: traceroute [OPTION...] HOST\n");
	printf("Print the route packets trace to network host.\n\n");
	printf("  -f NUM                      set initial hop distance, i.e., time-to-live\n");
	printf("  -m NUM                      set maximal hop count\n");
	printf("  -p PORT                     use destination PORT port\n");
	printf("  -q NUM                      send NUM probe packets per hop\n");
	printf("  -t NUM                      set type of service (TOS) to NUM\n");
	printf("  -w NUM                      wait NUM seconds for response\n");
	printf("  --dns                       print domain names of received messages\n");
	printf("  -?, --help                  give this help list\n");
}

void print_timeout(size_t index, uint8_t line_index)
{
	if (!index)
		fprintf(stderr, "%3u   *", line_index);
	else
		fprintf(stderr, "  *");
}

void print_packet(size_t index, uint8_t line_index, struct in_addr* addr,
		Ts* ts, int* print_addr, char const* host)
{
	if (!index)
	{
		if (host)
			fprintf(stderr, "%3u   %s (%s)  %lu,%lums", line_index, inet_ntoa(*addr), host,
					ts->whole, ts->fract);
		else
			fprintf(stderr, "%3u   %s  %lu,%lums", line_index, inet_ntoa(*addr),
					ts->whole, ts->fract);
		*print_addr = 0;
	}
	else
	{
		if (*print_addr)
		{
			if (host)
				fprintf(stderr, "  %s (%s)  %lu,%lums", inet_ntoa(*addr), host,
						ts->whole, ts->fract);
			else
				fprintf(stderr, "  %s  %lu,%lums", inet_ntoa(*addr),
						ts->whole, ts->fract);
			*print_addr = 0;
		}
		else
			fprintf(stderr, "  %lu,%lums", ts->whole, ts->fract);
	}
}
