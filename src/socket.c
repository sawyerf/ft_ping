#include "libft.h"
#include "ft_ping.h"
#include <signal.h>

extern t_ping g_ping;

static uint16_t	check_sum(uint16_t *buffer, size_t len)
{
	uint32_t	sum;

	sum = 0;
	while (len > 1)
	{
		sum += *buffer;
		buffer++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char*)buffer;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return ((uint16_t)~sum);
}

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

t_addrinfo *get_addr(char *host)
{
	t_addrinfo hints;
	t_addrinfo *result;
	int s;

	memset(&hints, 0, sizeof (struct addrinfo));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	if ((s = getaddrinfo (host, NULL, &hints, &result)) != 0)
	{
		ft_printf ("getaddrinfo: %s\n", gai_strerror (s));
		exit (EXIT_FAILURE);
	}
	return result;
}

void ft_ping(int sig)
{
	int rc;
	(void)sig;
	t_packet packet;

	g_ping.icmp_hdr.icmp_seq++;
	packet.icmp = g_ping.icmp_hdr;

	fill_ip(&packet.ip, g_ping.host);
    	packet.ip.check = 0;
    	packet.ip.check = check_sum((unsigned short *) &packet.ip, packet.ip.tot_len);

	packet.tv = ft_time();
	packet.icmp.icmp_otime = ft_ttime();
	packet.icmp.icmp_cksum = 0;
	packet.icmp.icmp_cksum = check_sum((void*)&packet.icmp, sizeof(t_packet) - sizeof(t_iphdr));
	rc = sendto(g_ping.sock, &packet, sizeof(t_packet),
		0, g_ping.ai->ai_addr, g_ping.ai->ai_addrlen);
	if (rc <= 0) {
		perror("Sendto\n");
		exit(1);
	}
	alarm(1);
}

int ft_pong()
{
	float diff;
	struct iovec io;
	struct msghdr msg;
	char		buffer[512];
	t_packet	packet;
	int ret;

	io.iov_base = &packet;
	io.iov_len = sizeof(packet);
	msg.msg_name = g_ping.ai;
	msg.msg_namelen = sizeof(t_addrinfo);
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;
	msg.msg_control = buffer;
	msg.msg_controllen = sizeof(buffer);
	msg.msg_flags = 0;
	ret = recvmsg(g_ping.sock, &msg, 0);

	diff = 0;
	if (!packet.icmp.icmp_type)
		diff = ft_updatetstat(packet.tv, ft_time());
	print_ping(msg.msg_iov->iov_base, diff, ret);
	return 0;
}

int ft_socket(t_addrinfo *ai)
{
	int sock;
	int yes;

	yes = 1;
	sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &yes, sizeof(yes));

	if (sock < 0)
		ft_dprintf(2, "ping: socket: Operation not permitted\n");
	return sock;
}
