#include "libft.h"
#include "ft_ping.h"
#include <signal.h>

t_ping g_ping;

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


void ft_finalstat(int sig)
{
	(void)sig;
	ft_printf("\n--- statistiques ping %s ---\n", g_ping.host);
	ft_printf("%d paquets transmis, ? reçus, ?%% packet loss, time 1002ms\n", g_ping.icmp_hdr.un.echo.sequence);
	ft_printf("rtt min/avg/max/mdev = ?/?/?/? ms\n");
	exit(0);
}

t_timeval ft_time()
{
	t_timeval tv;
	int ret;

	ret = gettimeofday(&tv, NULL);
	(void)ret;
	//ft_printf("ret=%d tv_sec=%d tv_usec=%d\n",
	//	ret, tv.tv_sec, tv.tv_usec);
	return tv;
}

float ft_timediff(t_timeval t1, t_timeval t2)
{
	t_timeval diff;
	float fdiff;

	diff.tv_sec = t2.tv_sec - t1.tv_sec;
	diff.tv_usec = 0;
	if (t1.tv_usec <= t2.tv_usec)
		diff.tv_usec = t2.tv_usec - t1.tv_usec;
	else
	{
		diff.tv_sec -= 1;
		diff.tv_usec = t2.tv_usec + t1.tv_usec;
	}
	fdiff = (int)diff.tv_sec * 100;
	if (diff.tv_usec)
		fdiff += (float)diff.tv_usec / 1000;
	// ft_printf("%d/%d\n", diff.tv_sec, diff.tv_usec);
	// ft_printf("%f\n", fdiff);
	return fdiff;
}

int main(int arg, char **argv)
{
	signal(SIGALRM, ft_ping);
	signal(SIGINT, ft_finalstat);

	g_ping.host = argv[1];
	g_ping.ai = get_addr(g_ping.host);
	g_ping.address = atos(g_ping.ai);
	g_ping.t1 = ft_time();
	ft_printf("PING %s(%s) 56(64) data bytes\n", g_ping.host, g_ping.address);
	if ((g_ping.sock = ft_socket(g_ping.ai)) < 0)
		return 1;

	fill_icmp(&g_ping.icmp_hdr);

	ft_ping(0);
	while (1)
		ft_pong();
}
