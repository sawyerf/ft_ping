#include "libft.h"
#include "ft_ping.h"

t_ping g_ping;

void atos(t_addrinfo *ai)
{
	struct addrinfo * _res;

	for(_res = ai; _res != NULL; _res = _res->ai_next)
	{
		if (_res->ai_family == AF_INET)
		{
			if (NULL == inet_ntop(AF_INET,
				&((struct sockaddr_in *)_res->ai_addr)->sin_addr,
					g_ping.address, sizeof(g_ping.address)))
			{
				perror("inet_ntop");
				exit(1);
			}
		}
	}
}

void fill_icmp(t_icmphdr *icmp)
{
	ft_memset(icmp, 0, sizeof(t_icmphdr));
	icmp->type = 8;
	icmp->un.echo.sequence = 0;
	icmp->un.echo.id = 1234;
}

void fill_ping()
{
	fill_icmp(&g_ping.icmp_hdr);
	g_ping.ai = get_addr(g_ping.host);
	atos(g_ping.ai);
	g_ping.tstart = ft_time();
	g_ping.tmin = 0;
	g_ping.tmax = 0;
	g_ping.tdev = 0;
	g_ping.tsum = 0;
	g_ping.tavg = 0;
}

int main(int arg, char **argv)
{
	signal(SIGALRM, ft_ping);
	signal(SIGINT, ft_finalstat);

	g_ping.host = argv[1];
	fill_ping();
	printf("PING %s(%s) %zu(%zu) data bytes\n", g_ping.host, g_ping.address, sizeof(t_packet) - sizeof(t_icmphdr), sizeof(t_packet) + 20);
	if ((g_ping.sock = ft_socket(g_ping.ai)) < 0)
		return 1;


	ft_ping(0);
	while (1)
		ft_pong();
}
