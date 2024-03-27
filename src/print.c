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
	printf("  -?, --help                  give this help list\n");
}

void print_timeout(uint8_t probe, uint16_t ttl)
{
	if (!probe)
		fprintf(stderr, "%3u   * ", ttl);
	else
		fprintf(stderr, " * ");
}

void print_packet(uint8_t probe, uint16_t ttl, struct in_addr* previous_addr,
		struct in_addr* addr, Ts* ts, char const* host)
{
	if (!probe)
		fprintf(stderr, "%3u   %s (%s) %lu,%lums ", ttl, host, inet_ntoa(*addr), ts->whole, ts->fract);
	else
	{
		if (previous_addr->s_addr != addr->s_addr)
			fprintf(stderr, " %s (%s) %lu,%lums ", host, inet_ntoa(*addr), ts->whole, ts->fract);
		else
			fprintf(stderr, " %lu,%lums ", ts->whole, ts->fract);
	}
}
