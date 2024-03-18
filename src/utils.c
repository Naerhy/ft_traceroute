#include "ft_traceroute.h"

size_t ft_strlen(char const* s)
{
	size_t len;

	len = 0;
	while (*(s + len))
		len++;
	return len;
}

static int valid_input(char const* s)
{
	if (ft_strlen(s) > 5)
		return 0;
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return 0;
		s++;
	}
	return 1;
}

int ft_atoi(char const* s, uint32_t* res)
{
	if (!valid_input(s))
		return 0;
	*res = 0;
	while (*s)
	{
		*res = *res * 10 + (*s - '0');
		s++;
	}
	return 1;
}
