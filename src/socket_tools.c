/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeyret <apeyret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 16:11:06 by apeyret           #+#    #+#             */
/*   Updated: 2021/04/12 16:17:02 by apeyret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include "libft.h"

t_ping	g_ping;

uint16_t	check_sum(uint16_t *buffer, size_t len)
{
	uint32_t	sum;

	sum = 0;
	while (len > 1)
	{
		sum += *buffer;
		buffer++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char*)buffer;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return ((uint16_t)~sum);
}

void		atos(t_addrinfo *ai)
{
	struct addrinfo	*res;

	res = ai;
	while (res)
	{
		if (res->ai_family == AF_INET)
		{
			if (NULL == inet_ntop(AF_INET,
				&((struct sockaddr_in *)res->ai_addr)->sin_addr,
					g_ping.address, sizeof(g_ping.address)))
			{
				dprintf(2, "ping: inet_ntop: fail\n");
				exit(1);
			}
		}
		res = res->ai_next;
	}
}

char		*ft_gaierr(int err)
{
	if (err == -2)
		return ("Name or service not known");
	else if (err == -3)
		return ("Temporary failure in name resolution");
	return ("Fail to resolve host");
}

t_addrinfo	*get_addr(char *host)
{
	t_addrinfo	hints;
	t_addrinfo	*result;
	int			s;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	if ((s = getaddrinfo(host, NULL, &hints, &result)) != 0)
	{
		printf("ping: %s: %s\n", host, ft_gaierr(s));
		exit(EXIT_FAILURE);
	}
	return (result);
}
