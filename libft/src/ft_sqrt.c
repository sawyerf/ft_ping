/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeyret <apeyret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 16:11:30 by apeyret           #+#    #+#             */
/*   Updated: 2021/04/12 16:11:33 by apeyret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_sqrt(double nb)
{
	double x;
	double y;
	double prcs;

	x = nb;
	y = 1;
	prcs = 0.00000001;
	while (x - y > prcs)
	{
		x = (x + y) / 2;
		y = nb / x;
	}
	return (x);
}
