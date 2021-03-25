#include "libft.h"
#include "ft_ping.h"

# define OPT_ARG 0
# define OPT_OPT 1
# define OPT_END 2
# define OPT_ERR 3

t_ping g_ping;

void fill_icmp(t_icmphdr *icmp)
{
	ft_memset(icmp, 0, sizeof(t_icmphdr));
	icmp->type = 8;
	icmp->code = 0;
	icmp->un.echo.sequence = 0;
	icmp->un.echo.id = 1234;
}

void fill_ip(t_iphdr *ip, char *host)
{
	ft_memset(ip, 0, sizeof(t_iphdr));
	ip->version = 4;
	ip->ihl = sizeof(t_iphdr) / 4;
	ip->tos = 0;
	ip->tot_len = sizeof(t_packet);
	ip->id = htonl(54321);
	ip->frag_off = 0;
	ip->ttl = 255;
	ip->protocol = IPPROTO_ICMP;
	ip->saddr = inet_addr("10.0.2.15"); //INADDR_ANY;
	ip->daddr = inet_addr(host);
	ip->check = 0;
	// iph->saddr = inet_addr ( source_ip );    //Spoof the source ip address
	// iph->daddr = sin.sin_addr.s_addr;
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

int options(char **argv)
{
	t_opt	*opt;
	int		ret;

	g_ping.ttl = 64;
	opt_init(&opt);
	opt_addvar(&opt, "-v", NULL, 0);
	opt_addvar(&opt, "-t", (void*)&g_ping.ttl, OPT_INT);
	ret = opt_parser(opt, ++argv, &g_ping.popt);
	opt_free(&opt);
	if (g_ping.ttl < 0 || g_ping.ttl > 255)
	{
		ft_printf("ping: invalid argument: '%d': out of range: 0 <= value <= 255\n", g_ping.ttl);
		return 1;
	}
	if (ft_tablen(g_ping.popt.arg))
		g_ping.host = g_ping.popt.arg[0];
	else if (!ret)
	{
		ft_printf("lol y manque un argument"); // erreur
		ret = 1; // autre chiffre
	}
	return ret;
}

int main(int arg, char **argv)
{
	int ret;	

	signal(SIGALRM, ft_ping);
	signal(SIGINT, ft_finalstat);
	if ((ret = options(argv)))
		return ret;
	fill_ping();
	printf("PING %s(%s) %zu(%zu) data bytes\n", g_ping.host, g_ping.address,
			sizeof(t_packet) - sizeof(t_icmphdr), sizeof(t_packet) + 20);
	if ((g_ping.sock = ft_socket(g_ping.ai)) < 0)
		return 1;
	ft_ping(0);
	while (1)
		ft_pong();
}

// int i =0;
// while (optpars.opt[i])
// {
// 	ft_printf("opt[%d]: %s\n", i, optpars.opt[i]);
// 	i++;
// }
// i = 0;
// while (optpars.arg[i])
// {
// 	ft_printf("arg[%d]: %s\n", i, optpars.arg[i]);
// 	i++;
// }
