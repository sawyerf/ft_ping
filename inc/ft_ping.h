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

void		fill_icmp(t_icmphdr *icmp);
t_timeval	ft_time();
t_timeval	ft_timediff(t_timeval t1, t_timeval t2);
int			main(int arg, char **argv);
int			ft_ping(int sock, t_addrinfo *ai, t_icmphdr icmp_hdr);
int			ft_pong(int sock, t_addrinfo *ai, t_icmphdr icmp_hdr);
int			ft_socket(t_addrinfo *ai);
t_addrinfo	*get_addr(char *host);

#endif
