#include "libft.h"
#include "ft_ping.h"
#include <signal.h>

extern t_ping g_ping;

static uint16_t	csum(uint16_t *buffer, size_t len)
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
	// hints.ai_protocol = IPPROTO_ICMP;
	hints.ai_protocol = IPPROTO_RAW;

	if ((s = getaddrinfo (host, NULL, &hints, &result)) != 0)
	{
		ft_printf ("getaddrinfo: %s\n", gai_strerror (s));
		exit (EXIT_FAILURE);
	}
	//result->ai_socktype = SOCK_DGRAM;
	return result;
}

void ft_ping(int sig)
{
	int rc;
	(void)sig;
	t_packet packet;

	g_ping.icmp_hdr.un.echo.sequence++;
	packet.icmp = g_ping.icmp_hdr;

	fill_ip(&packet.ip, g_ping.host);
    	packet.ip.check = 0;
    	packet.ip.check = csum ((unsigned short *) &packet.ip, packet.ip.tot_len);

	packet.tv = ft_time();
	packet.icmp.checksum = 0;
	packet.icmp.checksum = csum((void*)&packet.icmp, sizeof(t_packet) - sizeof(t_iphdr));

	ft_printf("sendto start\n");
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
	ft_printf("recvmsg start\n");
	ret = recvmsg(g_ping.sock, &msg, 0);

	print_ping(msg, 0, ret);
	ft_printf("lolipop: %d\n", ret);
	diff = ft_updatetstat(packet.tv, ft_time());
	print_ping(msg, diff, ret);
	return 0;
}

int ft_socket(t_addrinfo *ai)
{
	int sock;

	sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	//sock = socket(ai->ai_family, SOCK_DGRAM, IPPROTO_UDP);

	if (sock < 0)
	{
		perror("socket");
		ft_printf("socket fail");
	}
	return sock;
}
