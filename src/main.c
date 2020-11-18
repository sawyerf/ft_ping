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
	t_opt *opt;
	signal(SIGALRM, ft_ping);
	signal(SIGINT, ft_finalstat);
	g_ping.host = argv[1];
	fill_ping();

	opt_init(&opt);
	char *lol;
	int ttl;

	lol = NULL;
	ttl = 0;
	opt_addvar(&opt, "-v", NULL, 0);
	opt_addvar2(&opt, "-r", (void**)&lol, OPT_STR);
	opt_addvar(&opt, "-t", (void*)&ttl, OPT_INT);
	opt_parser(opt, ++argv);
	ft_printf("lol: %s\n", lol);
	ft_printf("ttl: %d\n", ttl);
	printf("PING %s(%s) %zu(%zu) data bytes\n", g_ping.host, g_ping.address, sizeof(t_packet) - sizeof(t_icmphdr), sizeof(t_packet) + 20);
	if ((g_ping.sock = ft_socket(g_ping.ai)) < 0)
		return 1;

	ft_ping(0);
	while (1)
		ft_pong();
}
