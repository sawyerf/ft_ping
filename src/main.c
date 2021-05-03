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

int		help(char *opt, char *arg)
{
	(void)arg;
	(void)opt;
	printf("Usage: ping [-tv] destination\n");
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
		ret = 2;
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
	{
		free(g_ping.ai);
		return (1);
	}
	ft_ping(0);
	while (1)
		ft_pong();
}
