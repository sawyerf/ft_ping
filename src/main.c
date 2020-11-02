#include "libft.h"
#include "ft_ping.h"

char *atos(t_addrinfo *ai)
{
	struct addrinfo * _res;
	char address[INET6_ADDRSTRLEN];

	for(_res = ai; _res != NULL; _res = _res->ai_next)
	{
		if (_res->ai_family == AF_INET)
		{
			if (NULL == inet_ntop(AF_INET,
				&((struct sockaddr_in *)_res->ai_addr)->sin_addr,
					address,sizeof(address) ))
			{
				perror("inet_ntop");
				return NULL;
			}
		}
	}
	return ft_strdup(address);
}

void fill_icmp(t_icmphdr *icmp)
{
	memset(icmp, 0, sizeof(t_icmphdr));
	icmp->type = 8;
	icmp->un.echo.sequence = 0;
	icmp->un.echo.id = 1234;
}

t_timeval ft_time()
{
	t_timeval tv;
	int ret;

	ret = gettimeofday(&tv, NULL);
	ft_printf("ret=%d tv_sec=%d tv_usec=%d\n",
		ret, tv.tv_sec, tv.tv_usec);
	return tv;
}

t_timeval ft_timediff(t_timeval t1, t_timeval t2)
{
	t_timeval diff;

	diff.tv_sec = t2.tv_sec - t1.tv_sec;
	diff.tv_usec = 0;
	if (t1.tv_usec <= t2.tv_usec)
		diff.tv_usec = t2.tv_usec - t1.tv_usec;
	return diff;
}


int main(int arg, char **argv)
{
	char *address;
	t_addrinfo *ai;
	int sock;
	t_icmphdr icmp_hdr;

	ai = get_addr(argv[1]);
	address = atos(ai);
	ft_printf("PING %s(%s) ?? data bytes\n", argv[1], address);
	if ((sock = ft_socket(ai)) < 0)
		return 1;

	fill_icmp(&icmp_hdr);

	while (1)
	{
		ft_ping(sock, ai, icmp_hdr);
		ft_pong(sock, ai, icmp_hdr);
	}
}
