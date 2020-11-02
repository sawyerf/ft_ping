#include "libft.h"
#include "ft_ping.h"

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

int ft_ping(int sock, t_addrinfo *ai, t_icmphdr icmp_hdr)
{
	int rc;

	icmp_hdr.un.echo.sequence++;
	//t1 = ft_time();
	rc = sendto(sock, &icmp_hdr, sizeof(icmp_hdr), 0, ai->ai_addr, ai->ai_addrlen);
	if (rc <= 0) {
		perror("Sendto\n");
		return 1;
	}
	return 0;
}

int ft_pong(int sock, t_addrinfo *ai, t_icmphdr icmp_hdr)
{
	int rc;
	t_icmphdr rcv_hdr;
	char address[] = "lol";
	t_timeval diff = {0, 0} ;

	rc = recvfrom(sock, &rcv_hdr, sizeof(rcv_hdr), 0, ai->ai_addr, &ai->ai_addrlen);
	if (rc <= 0) {
		perror("recvfrom\n");
		return 1;
	} else if (rc < sizeof(rcv_hdr)) {
		ft_printf("Error, got short ICMP packet, %d bytes\n", rc);
		return 1;
	}
	//diff = ft_timediff(t1, ft_time());
	ft_printf("?? bytes from %s: icmp_seq=%d ttl=?? time= %d s %d ms type=%d\n", address, icmp_hdr.un.echo.sequence, diff.tv_sec, diff.tv_usec, rcv_hdr.type);
	alarm(1);
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
