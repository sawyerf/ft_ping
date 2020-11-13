#include "libft.h"
#include "ft_ping.h"
#include <math.h>

extern t_ping g_ping;

void print_ping(float diff, t_icmphdr rcv_hdr, int bfrom)
{
	ft_printf("%d bytes from %s: icmp_seq=%d ttl=?? ",
		bfrom, g_ping.address, rcv_hdr.un.echo.sequence);
	ft_printf("time=%.1f ms type=%d\n", diff, rcv_hdr.type);
//	else
//		ft_printf("time=%d s %.2f ms type=%d\n", diff.tv_sec, usec, rcv_hdr.type);
}

void ft_finalstat(int sig)
{
	(void)sig;
	int tsum;
	double tsum2;
	int mdev;

	if (g_ping.ti)
		g_ping.tavg += g_ping.tsum / g_ping.ti;
	ft_printf("\n--- statistiques ping %s ---\n", g_ping.host);
	ft_printf("%d paquets transmis, %d reçus, %d%% packet loss, time %.0fms\n",
		g_ping.icmp_hdr.un.echo.sequence, g_ping.ti,
		100 - ((g_ping.ti / g_ping.icmp_hdr.un.echo.sequence) * 100),
		ft_timediff(g_ping.tstart, ft_time()) / 1000.0);

	tsum = g_ping.tsum / g_ping.ti;
	tsum2 = g_ping.tsumd / g_ping.ti;
	mdev = /*0 + 0 * tsum * tsum2; */sqrt(tsum2 - tsum * tsum);
	ft_printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
		g_ping.tmin / 1000.0,
		g_ping.tavg / 1000.0,
		g_ping.tmax / 1000.0,
		mdev / 1000.0);
	free(g_ping.ai);
	exit(0);
}

