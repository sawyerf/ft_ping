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

/*void fill_ip(t_iphdr *ip)
{
	ft_memset (ip , 0, sizeof(ip));
	ip->version = 4;
	ip->ihl = 5; //sizeof(t_iphdr) / 4;
	ip->tos = 0;
	ip->tot_len = htons(sizeof(t_packet));
	ft_printf("%d\n", ip->tot_len);
	ip->id = 0;
	ip->frag_off = 0;
	ip->ttl = 64;
	ip->protocol = g_ping.ai->ai_protocol;
	ip->check = 0;
	// struct in_addr saddr;
	// struct in_addr daddr;
	// inet_pton(AF_INET, "192.168.1.18", &saddr);
	// inet_pton(AF_INET, g_ping.address, &daddr);
	ip->saddr = inet_addr("127.0.0.1"); // saddr.s_addr;
	ip->daddr = inet_addr(g_ping.host); // daddr.s_addr;
}*/

void fill_ping()
{
	fill_icmp(&g_ping.icmp_hdr);
	g_ping.ai = get_addr(g_ping.host);
//	fill_ip(&g_ping.ip_hdr);
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
	ft_printf("PING %s(%s) 56(64) data bytes\n", g_ping.host, g_ping.address);
	if ((g_ping.sock = ft_socket(g_ping.ai)) < 0)
		return 1;


	ft_ping(0);
	while (1)
		ft_pong();
}
