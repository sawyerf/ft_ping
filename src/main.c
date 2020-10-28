#include "libft.h"
#include "ft_ping.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>

typedef struct addrinfo t_addrinfo;
typedef struct icmphdr t_icmphdr;

void atos(char *hostname, t_addrinfo *ai)
{
	struct addrinfo * _res;
	char _address[INET6_ADDRSTRLEN];

	for(_res = ai; _res != NULL; _res = _res->ai_next)
	{
		if (_res->ai_family == AF_INET)
		{
			if (NULL == inet_ntop(AF_INET,
				&((struct sockaddr_in *)_res->ai_addr)->sin_addr,
					_address,sizeof(_address) ))
			{
				perror("inet_ntop");
			}
			ft_printf("PING %s(??? (%s)) ?? data bytes\n", hostname, _address);
		}
	}
}

void fill_icmp(t_icmphdr *icmp)
{
	memset(icmp, 0, sizeof(t_icmphdr));
	icmp->type = 8;
}

int main(int arg, char **argv)
{
	(void)arg;
	int sock;
	t_addrinfo hints;
	t_addrinfo *ai;
	t_addrinfo *result;

    unsigned char data[2048];
	t_icmphdr icmp;
    t_icmphdr ricmp;
	int s; 

	fill_icmp(&icmp);
    memcpy(data, &icmp, sizeof(t_icmphdr));

	memset(&hints, 0, sizeof (struct addrinfo));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	if ((s = getaddrinfo (argv[1], NULL, &hints, &result)) != 0)
	{
		ft_printf ("getaddrinfo: %s\n", gai_strerror (s));
		exit (EXIT_FAILURE);
	}
	ai = result;
	atos(argv[1], ai);

	sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (sock < 0)
		ft_printf("fail: %d\n", sock);
	s = sendto(sock, data, sizeof(t_icmphdr), 0, ai->ai_addr, ai->ai_addrlen);
	ft_printf("sendto: %d\n", sock);

	socklen_t slen = 0;
	int rc = 0;
	
	ft_printf("ici\n");
    rc = recvfrom(sock, &ricmp, sizeof(t_icmphdr), 0, NULL, &slen);
    if (rc <= 0) {
        perror("recvfrom");
        return 1;
    } else if (rc < sizeof(ricmp)) {
        ft_printf("Error, got short ICMP packet, %d bytes\n", rc);
        return 1;
    }
	ft_printf("recv: %d\n", rc);
}
