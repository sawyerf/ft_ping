/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeyret <apeyret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 16:11:11 by apeyret           #+#    #+#             */
/*   Updated: 2021/04/12 16:18:03 by apeyret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_ping.h"

extern t_ping	g_ping;

t_timeval		ft_time(void)
{
	t_timeval	tv;
	int			ret;

	ret = gettimeofday(&tv, NULL);
	(void)ret;
	return (tv);
}

unsigned int	ft_ttime(void)
{
	t_timeval	tv;
	int			ret;

	ret = gettimeofday(&tv, NULL);
	(void)ret;
	return (tv.tv_sec);
}

unsigned short icmp_uniqueid(void)
{
	t_timeval	tv;
	long		ret;

	ret = gettimeofday(&tv, NULL);
	ret = (tv.tv_sec * 100 + tv.tv_usec / 10000) % 65534;
	return ((unsigned short)ret);
}

long			timediff(t_timeval t1, t_timeval t2)
{
	t_timeval	diff;
	long		ldiff;

	diff.tv_sec = t2.tv_sec - t1.tv_sec;
	diff.tv_usec = 0;
	if (t1.tv_usec <= t2.tv_usec)
		diff.tv_usec = t2.tv_usec - t1.tv_usec;
	else
	{
		diff.tv_sec -= 1;
		diff.tv_usec = 1000000 - t1.tv_usec + t2.tv_usec;
	}
	ldiff = (int)diff.tv_sec * 100 * 10000;
	if (diff.tv_usec)
		ldiff += (float)diff.tv_usec;
	return (ldiff);
}

float			ft_updatetstat(t_timeval t1, t_timeval t2)
{
	float	fdiff;

	fdiff = timediff(t1, t2);
	if (fdiff < g_ping.tmin || !g_ping.tmin)
		g_ping.tmin = fdiff;
	if (fdiff > g_ping.tmax)
		g_ping.tmax = fdiff;
	g_ping.tsum += fdiff;
	g_ping.tsumd += (long long)fdiff * (long long)fdiff;
	g_ping.ti += 1;
	return (fdiff / 1000.0);
}
