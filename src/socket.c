/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeyret <apeyret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 16:11:00 by apeyret           #+#    #+#             */
/*   Updated: 2021/04/12 16:13:10 by apeyret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_ping.h"

extern t_ping	g_ping;

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
	packet.icmp.icmp_cksum = check_sum((void*)&packet.icmp,
		sizeof(t_packet) - sizeof(t_iphdr));
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
