#include "libft.h"
#include "ft_ping.h"
#include <math.h>

extern t_ping g_ping;

void print_ping(t_msghdr msg, float diff, int bfrom)
{
	t_icmphdr *icmp;

	icmp = msg.msg_iov->iov_base;
	ft_printf("icmp_seq=%d type=%d code=%d\n",
		icmp->un.echo.sequence, icmp->type, icmp->code);
	return ;
	ft_printf("%d bytes from %s: icmp_seq=%d ",
		bfrom, g_ping.address, icmp->un.echo.sequence);
	ft_printf("time=%.1f ms type=%d\n", diff, icmp->type);
//	else
//		ft_printf("time=%d s %.2f ms type=%d\n", diff.tv_sec, usec, icmp->type);
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

