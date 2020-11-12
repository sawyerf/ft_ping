#include "libft.h"
#include "ft_ping.h"
#include <signal.h>

extern t_ping g_ping;

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
		return NULL;
	}
	result->ai_socktype = SOCK_DGRAM;
	return result;
}

void ft_ping(int sig)
{
	int rc;
	(void)sig;
	t_packet packet;

	g_ping.icmp_hdr.un.echo.sequence++;
	packet.icmp = g_ping.icmp_hdr;
	packet.tv = ft_time();
	
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
	io.iov_len = sizeof(t_packet);
	msg.msg_name = g_ping.ai;
	msg.msg_namelen = sizeof(t_addrinfo);
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;
	msg.msg_control = buffer;
	msg.msg_controllen = sizeof(buffer);
	msg.msg_flags = 0;
	ret = recvmsg(g_ping.sock, &msg, 0);
	if (ret == -1)
	{
		ft_printf("recvmsg failed");
		exit(1);
	}

	diff = ft_updatetstat(packet.tv, ft_time());
	print_ping(diff, packet.icmp);
	return 0;
}

int ft_socket(t_addrinfo *ai)
{
	int sock;

	sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (sock < 0)
		ft_printf("socket fail");
	return sock;
}
