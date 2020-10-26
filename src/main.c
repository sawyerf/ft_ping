#include "libft.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <stdio.h>



int main(int arg, char **argv)
{
	(void)arg;
	int sock;
    struct addrinfo hints;
    struct addrinfo *ai;
    struct addrinfo *result;
    int s; 

	
    memset(&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    if ((s = getaddrinfo (argv[1], NULL, &hints, &result)) != 0) {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
        exit (EXIT_FAILURE);
    }
	ai = result;

	sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (sock < 0)
		ft_printf("fail: %d\n", sock);

	s = sendto(sock, NULL, 0, 0, ai->ai_addr, ai->ai_addrlen);
	ft_printf("sendto: %d\n", sock);
}
