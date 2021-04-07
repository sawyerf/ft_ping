#include "libft.h"
#include "ft_ping.h"
#include <math.h>

extern t_ping g_ping;

void print_ping(t_packet *packet, float diff, int bfrom)
{
	t_icmp_err *icmp;

	ft_printf("%d bytes from %s: ", bfrom, g_ping.address);
	if (packet->icmp.icmp_type == ICMP_TIME_EXCEEDED)
	{
		icmp = (t_icmp_err*)&packet->tv;
		ft_printf("icmp_seq=%d ", icmp->sequence);
		if (ft_tabfind(g_ping.popt.opt, "-v"))
			ft_printf("type=%d code=%d ", packet->icmp.icmp_type, packet->icmp.icmp_code);
		ft_printf("Time to live exceeded\n");
		g_ping.error++;
	}
	else
	{
		ft_printf("icmp_seq=%d ", packet->icmp.icmp_seq);
		if (ft_tabfind(g_ping.popt.opt, "-v"))
			ft_printf("type=%d code=%d ", packet->icmp.icmp_type, packet->icmp.icmp_code);
		ft_printf("ttl=%d time=%.1f ms\n", packet->ip.ip_ttl, diff);
	}
}

void ft_finalstat(int sig)
{
	(void)sig;
	int tsum;
	double tsum2;
	int mdev;

	mdev = 0;
	if (g_ping.ti)
	{
		g_ping.tavg += g_ping.tsum / g_ping.ti;
		tsum = g_ping.tsum / g_ping.ti;
		tsum2 = g_ping.tsumd / g_ping.ti;
		mdev = sqrt(tsum2 - tsum * tsum); // ft_sqrt
	}
	ft_printf("\n--- statistiques ping %s ---\n", g_ping.host);
	ft_printf("%d packets transmitted, %d received, ", g_ping.icmp_hdr.icmp_seq, g_ping.ti);
	if (g_ping.error)
		ft_printf("+%d errors, ", g_ping.error);
	else
		ft_printf("%d%% packet loss, ", 100 - ((g_ping.ti * 100) / g_ping.icmp_hdr.icmp_seq));
	ft_printf("time %.0fms\n", timediff(g_ping.tstart, ft_time()) / 1000.0);

	if (g_ping.ti)
		ft_printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			g_ping.tmin / 1000.0, g_ping.tavg / 1000.0,
			g_ping.tmax / 1000.0, mdev / 1000.0);
	free(g_ping.ai);
	exit(0);
}
