/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeyret <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 15:13:10 by apeyret           #+#    #+#             */
/*   Updated: 2019/04/16 15:19:16 by apeyret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_tabcmp(char **tab, char *str)
{
	int	i;

	i = 0;
	while (tab[i] != NULL)
	{
		if (!ft_strcmp(tab[i], str))
			return (i);
		i++;
	}
	return (-1);
}

int			ft_tabfind(char **tab, char *str)
{
	int	i;

	i = 0;
	while (tab[i] != NULL)
	{
		if (!ft_strcmp(tab[i], str))
			return (1);
		i++;
	}
	return (0);
}
