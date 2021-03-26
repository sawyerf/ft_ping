#include "libft.h"
#include "ft_ping.h"
#include <math.h>

extern t_ping g_ping;

void print_ping(t_packet *packet, float diff, int bfrom)
{
	ft_printf("%d bytes from %s: icmp_seq=%d",
		bfrom, g_ping.address, packet->icmp.un.echo.sequence);
	if (packet->icmp.type == ICMP_TIME_EXCEEDED)
		ft_printf(" Time to Live\n");
	else
		ft_printf(" ttl=%d time=%.1f ms\n",
			packet->ip.ttl, diff);
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
	ft_printf("%d paquets transmis, %d reçus, %d%% packet loss, time %.0fms\n",
		g_ping.icmp_hdr.un.echo.sequence, g_ping.ti,
		100 - ((g_ping.ti / g_ping.icmp_hdr.un.echo.sequence) * 100),
		ft_timediff(g_ping.tstart, ft_time()) / 1000.0);

	ft_printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
		g_ping.tmin / 1000.0,
		g_ping.tavg / 1000.0,
		g_ping.tmax / 1000.0,
		mdev / 1000.0);
	free(g_ping.ai);
	exit(0);
}

