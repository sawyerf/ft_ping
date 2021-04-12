#include "libft.h"
#include "ft_ping.h"
#include <signal.h>

extern t_ping	g_ping;

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

void			atos(t_addrinfo *ai)
{
	struct addrinfo	*res;

	for (res = ai; res != NULL; res = res->ai_next)
	{
		if (res->ai_family == AF_INET)
		{
			if (NULL == inet_ntop(AF_INET,
				&((struct sockaddr_in *)res->ai_addr)->sin_addr,
					g_ping.address, sizeof(g_ping.address)))
			{
				ft_dprintf(2, "ping: inet_ntop: fail\n");
				exit(1);
			}
		}
	}
}

char	*ft_gaierr(int err)
{
	if (err == -2)
		return ("Name or service not known");
	else if (err == -3)
		return ("Temporary failure in name resolution");
	return ("Fail to resolve host");
}

t_addrinfo		*get_addr(char *host)
{
	t_addrinfo	hints;
	t_addrinfo	*result;
	int			s;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	if ((s = getaddrinfo(host, NULL, &hints, &result)) != 0)
	{
		ft_printf("ping: %s: %s\n", host, ft_gaierr(s));

		exit(EXIT_FAILURE);
	}
	return (result);
}

void			ft_ping(int sig)
{
	int			rc;
	t_packet	packet;

	(void)sig;
	ft_memset(&packet, 0, sizeof(t_packet));
	g_ping.icmp_hdr.icmp_seq++;
	packet.icmp = g_ping.icmp_hdr;
	fill_ip(&packet.ip);
	packet.ip.ip_sum = 0;
	packet.ip.ip_sum = check_sum((unsigned short*)&packet.ip, packet.ip.ip_len);
	packet.tv = ft_time();
	packet.icmp.icmp_otime = ft_ttime();
	packet.icmp.icmp_cksum = 0;
	packet.icmp.icmp_cksum = check_sum((void*)&packet.icmp, sizeof(t_packet) - sizeof(t_iphdr));
	rc = sendto(g_ping.sock, &packet, sizeof(t_packet),
		0, g_ping.ai->ai_addr, g_ping.ai->ai_addrlen);
	if (rc <= 0)
	{
		ft_dprintf(2, "ping: sendto: fail\n");
		exit(1);
	}
	alarm(1);
}

int				ft_pong(void)
{
	float			diff;
	struct iovec	io;
	struct msghdr	msg;
	char			buffer[512];
	t_packet		packet;
	int				ret;

	ft_memset(&packet, 0, sizeof(t_packet));
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
	return (0);
}

int				ft_socket(t_addrinfo *ai)
{
	int	sock;
	int	yes;

	yes = 1;
	sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &yes, sizeof(yes));
	if (sock < 0)
		ft_dprintf(2, "ping: socket: Operation not permitted\n");
	return (sock);
}
