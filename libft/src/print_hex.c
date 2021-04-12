/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeyret <apeyret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 16:11:44 by apeyret           #+#    #+#             */
/*   Updated: 2021/04/12 16:11:45 by apeyret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_hex(unsigned char *addr, size_t size)
{
	int i;

	i = 0;
	ft_printf("\n %d bytes\n00 - ", size);
	while (size)
	{
		ft_printf("%.2X ", *addr);
		addr++;
		size--;
		i++;
		if (!(i % 10))
			ft_printf("\n%d - ", i);
	}
	ft_printf("\n");
}
