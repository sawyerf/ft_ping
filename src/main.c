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

char *atos(t_addrinfo *ai)
{
	struct addrinfo * _res;
	char address[INET6_ADDRSTRLEN];

	for(_res = ai; _res != NULL; _res = _res->ai_next)
	{
		if (_res->ai_family == AF_INET)
		{
			if (NULL == inet_ntop(AF_INET,
				&((struct sockaddr_in *)_res->ai_addr)->sin_addr,
					address,sizeof(address) ))
			{
				perror("inet_ntop");
				return NULL;
			}
		}
	}
	return ft_strdup(address);
}

void fill_icmp(t_icmphdr *icmp)
{
	memset(icmp, 0, sizeof(t_icmphdr));
	icmp->type = 8;
	icmp->un.echo.id = 1234;
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
		return NULL;
	}
	result->ai_socktype = SOCK_DGRAM;
	return result;
}

int main(int arg, char **argv)
{
	t_addrinfo *ai;
    t_icmphdr icmp_hdr;
    t_icmphdr rcv_hdr;
    int sock;
    int rc;
	char *address;

	ai = get_addr(argv[1]);
	address = atos(ai);
	ft_printf("PING %s(%s) ?? data bytes\n", argv[1], address);
	sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sock < 0) {
        perror("socket");
        return 1;
    }
	
    memset(&icmp_hdr, 0, sizeof icmp_hdr);
    icmp_hdr.type = 8;
    icmp_hdr.un.echo.sequence = 0;
    icmp_hdr.un.echo.id = 1234;

    while (1){

        icmp_hdr.un.echo.sequence++;
		rc = sendto(sock, &icmp_hdr, sizeof(icmp_hdr), 0, ai->ai_addr, ai->ai_addrlen);
        if (rc <= 0) {
            perror("Sendto");
            break;
        }

        rc = recvfrom(sock, &rcv_hdr, sizeof(rcv_hdr), 0, ai->ai_addr, &ai->ai_addrlen);
        if (rc <= 0) {
            perror("recvfrom");
            break;
        } else if (rc < sizeof(rcv_hdr)) {
            ft_printf("Error, got short ICMP packet, %d bytes\n", rc);
            break;
        }
                            
		ft_printf("?? bytes from %s: icmp_seq=%d ttl=?? time=?? ms type=%d\n", address, icmp_hdr.un.echo.sequence, rcv_hdr.type);
		if (icmp_hdr.un.echo.sequence == 3)
			break;
    }
}
