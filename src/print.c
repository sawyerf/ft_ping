#include "libft.h"
#include "ft_ping.h"
#include <math.h>

extern t_ping g_ping;

struct s_lol {
  uint8_t	type;		/* message type */
  uint8_t	code;		/* type sub-code */
  uint16_t	checksum;
  uint16_t	id;
  uint16_t	sequence;
};

void print_ping(t_packet *packet, float diff, int bfrom)
{
	struct s_lol *icmp;

	ft_printf("%d bytes from %s: icmp_seq=%d",
		bfrom, g_ping.address, packet->icmp.icmp_seq);
	if (packet->icmp.icmp_type == ICMP_TIME_EXCEEDED)
	{
		ft_printf(" Time to Live\n");
		icmp = (struct s_lol*)&packet->icmp.icmp_radv;
		ft_printf("seq=%d\n", icmp->sequence);
		ft_printf("%d %d\n", sizeof(struct s_lol), sizeof(packet->icmp.icmp_radv));
		ft_printf("ip_version=%d ip_hl=%d\n", packet->icmp.icmp_ip.ip_v, packet->icmp.icmp_ip.ip_hl);
	}
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
		g_ping.icmp_hdr.icmp_seq, g_ping.ti,
		100 - ((g_ping.ti / g_ping.icmp_hdr.icmp_seq) * 100),
		ft_timediff(g_ping.tstart, ft_time()) / 1000.0);

	ft_printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
		g_ping.tmin / 1000.0,
		g_ping.tavg / 1000.0,
		g_ping.tmax / 1000.0,
		mdev / 1000.0);
	free(g_ping.ai);
	exit(0);
}
