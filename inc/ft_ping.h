#ifndef FT_PING_H
# define FT_PING_H

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/ip_icmp.h>
# include <netdb.h>
# include <stdio.h>
# include <arpa/inet.h>
# include <sys/time.h>

typedef struct addrinfo t_addrinfo;
typedef struct icmphdr t_icmphdr;
typedef struct timeval t_timeval;

typedef struct	s_ping
{
	char		*host;
	//char		address[INET6_ADDRSTRLEN];
	char		*address;
	int			sock;
	t_addrinfo	*ai;
	t_icmphdr	icmp_hdr;
	t_timeval	t1;
}				t_ping;


void		fill_icmp(t_icmphdr *icmp);
t_timeval	ft_time();
float		ft_timediff(t_timeval t1, t_timeval t2);
int			main(int arg, char **argv);
//int			ft_ping(int sock, t_addrinfo *ai, t_icmphdr icmp_hdr);
//int			ft_pong(int sock, t_addrinfo *ai, t_icmphdr icmp_hdr);
void			ft_ping(int sig);
int			ft_pong();
int			ft_socket(t_addrinfo *ai);
t_addrinfo	*get_addr(char *host);

#endif
