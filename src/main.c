/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeyret <apeyret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 16:09:57 by apeyret           #+#    #+#             */
/*   Updated: 2021/04/12 16:10:07 by apeyret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_ping.h"

t_ping	g_ping;

void	fill_icmp(t_icmphdr *icmp)
{
	ft_memset(icmp, 0, sizeof(t_icmphdr));
	icmp->icmp_type = 8;
	icmp->icmp_code = 0;
	icmp->icmp_seq = 0;
	icmp->icmp_id = 1234;
}

void	fill_ip(t_iphdr *ip)
{
	ft_memset(ip, 0, sizeof(t_iphdr));
	ip->ip_v = 4;
	ip->ip_hl = sizeof(t_iphdr) / 4;
	ip->ip_tos = 0;
	ip->ip_len = sizeof(t_packet);
	ip->ip_id = 54321;
	ip->ip_off = 0;
	ip->ip_ttl = g_ping.ttl;
	ip->ip_p = IPPROTO_ICMP;
	ip->ip_src.s_addr = INADDR_ANY;
	ip->ip_dst.s_addr = \
		((struct sockaddr_in*)g_ping.ai->ai_addr)->sin_addr.s_addr;
	ip->ip_sum = 0;
}

void	fill_ping(void)
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

int		help(char *opt, char *arg)
{
	(void)arg;
	(void)opt;
	printf("help\n");
	return (1);
}

int		options(char **argv)
{
	t_opt	*opt;
	int		ret;

	ft_memset(&g_ping, 0, sizeof(g_ping));
	g_ping.ttl = 64;
	opt_init(&opt);
	opt_addvar(&opt, "-v", NULL, 0);
	opt_addvar(&opt, "-t", (void*)&g_ping.ttl, OPT_INT);
	opt_addfunc(&opt, "-h", &help);
	ret = opt_parser(opt, ++argv, &g_ping.popt, "ping");
	opt_free(&opt);
	if (g_ping.ttl < 0 || g_ping.ttl > 255)
	{
		printf("ping: invalid argument: "\
				"'%d': out of range: 0 <= value <= 255\n", g_ping.ttl);
		return (1);
	}
	if (ft_tablen(g_ping.popt.arg))
		g_ping.host = g_ping.popt.arg[0];
	else if (!ret)
	{
		printf("Usage: ping [-tv] destination\n");
		ret = 2; // autre chiffre
	}
	return (ret);
}

int		main(int arg, char **argv)
{
	int ret;

	(void)arg;
	signal(SIGALRM, ft_ping);
	signal(SIGINT, ft_finalstat);
	if ((ret = options(argv)))
		return (ret);
	fill_ping();
	printf("PING %s (%s) %zu(%zu) data bytes\n", g_ping.host, g_ping.address,
		sizeof(t_packet) - sizeof(t_icmphdr), sizeof(t_packet) + 20);
	if ((g_ping.sock = ft_socket(g_ping.ai)) < 0)
		return (1);
	ft_ping(0);
	while (1)
		ft_pong();
}
