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

	g_ping.icmp_hdr.un.echo.sequence++;
	rc = sendto(g_ping.sock, &g_ping.icmp_hdr, sizeof(g_ping.icmp_hdr),
				0, g_ping.ai->ai_addr, g_ping.ai->ai_addrlen);
	if (rc <= 0) {
		perror("Sendto\n");
		exit(1);
	}
}

void print_ping(float diff, t_icmphdr rcv_hdr)
{
	ft_printf("?? bytes from %s: icmp_seq=%d ttl=?? ",
		g_ping.address, rcv_hdr.un.echo.sequence);
	ft_printf("time=%.2f ms type=%d\n", diff, rcv_hdr.type);
//	else
//		ft_printf("time=%d s %.2f ms type=%d\n", diff.tv_sec, usec, rcv_hdr.type);
	alarm(1);
}

int ft_pong()
{
	t_icmphdr rcv_hdr;
	float diff;
	struct iovec io;
	struct msghdr msg;
	char			buffer[512];
	int ret;

	io.iov_base = &rcv_hdr;
	io.iov_len = sizeof(t_icmphdr);
	msg.msg_name = g_ping.ai;
	msg.msg_namelen = sizeof(t_addrinfo);
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;
	msg.msg_control = buffer;
	msg.msg_controllen = sizeof(buffer);
	msg.msg_flags = 0;
	//ft_printf("buffer: %s\n", buffer);
	ret = recvmsg(g_ping.sock, &msg, 0);
	if (ret == -1)
		ft_printf("recvmsg failed");

	diff = ft_timediff(g_ping.t1, ft_time());
	print_ping(diff, rcv_hdr);
	g_ping.t1 = ft_time();
	g_ping.t1.tv_sec += 1;
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
