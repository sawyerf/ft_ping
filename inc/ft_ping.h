#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <signal.h>
# include <stdio.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <sys/types.h>


typedef struct addrinfo	t_addrinfo;
typedef struct icmp	t_icmphdr;
typedef struct ip	t_iphdr;
typedef struct timeval	t_timeval;
typedef struct msghdr	t_msghdr;


typedef struct	s_stat {
	int error;
}		t_stat;

typedef struct	s_ping
{
	char		*host;
	char		address[INET6_ADDRSTRLEN];
	int		sock;
	t_addrinfo	*ai;
	t_icmphdr	icmp_hdr;

	t_timeval	tstart;
	float		tmin;
	float		tmax;
	float		tavg;
	float		tdev;
	float		tsum;
	long long	tsumd;
	int		ti;
	int		ttl;
	int		error;
	t_optpars	popt;
}				t_ping;

typedef struct	s_icmp_err {
  uint8_t	type;
  uint8_t	code;
  uint16_t	checksum;
  uint16_t	id;
  uint16_t	sequence;
}		t_icmp_err;

typedef struct	s_packet
{
	t_iphdr		ip;
	t_icmphdr	icmp;
	t_timeval	tv;
}		t_packet;

void		fill_icmp(t_icmphdr *icmp);
t_timeval	ft_time();
long		timediff(t_timeval t1, t_timeval t2);
int		main(int arg, char **argv);
void		ft_ping(int sig);
int		ft_pong();
int		ft_socket(t_addrinfo *ai);
t_addrinfo	*get_addr(char *host);
float		ft_updatetstat(t_timeval t1, t_timeval t2);
void		print_ping(t_packet *msg, float diff, int bfrom);
void		ft_finalstat(int sig);
int		get_ttl(t_msghdr *msg);
void		atos(t_addrinfo *ai);
void		fill_ip(t_iphdr *ip);
unsigned int	ft_ttime();
uint16_t	check_sum(uint16_t *buffer, size_t len);
void			atos(t_addrinfo *ai);
char	*ft_gaierr(int err);
t_addrinfo		*get_addr(char *host);

#endif
