/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_optadd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeyret <apeyret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 16:01:30 by apeyret           #+#    #+#             */
/*   Updated: 2021/04/12 16:01:33 by apeyret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	opt_init(t_opt **opt)
{
	*opt = NULL;
}

int		opt_addvar2(t_opt **opt, char *arg, void **var, char type_var)
{
	t_opt *nopt;

	if (!(nopt = malloc(sizeof(t_opt))))
		return (1);
	ft_memset(nopt, 0, sizeof(t_opt));
	ft_strcpy(nopt->opt, arg);
	nopt->var2 = var;
	nopt->type = OPT_VAR;
	nopt->type_var = type_var;
	nopt->next = *opt;
	*opt = nopt;
	return (0);
}

int		opt_addvar(t_opt **opt, char *arg, void *var, char type_var)
{
	t_opt *nopt;

	if (!(nopt = malloc(sizeof(t_opt))))
		return (1);
	ft_memset(nopt, 0, sizeof(t_opt));
	ft_strcpy(nopt->opt, arg);
	nopt->var = var;
	nopt->type = OPT_VAR;
	nopt->type_var = type_var;
	nopt->next = *opt;
	*opt = nopt;
	return (0);
}

int		opt_addfunc(t_opt **opt, char *arg, int (*func)(char *opt, char *arg))
{
	t_opt *nopt;

	if (!(nopt = malloc(sizeof(t_opt))))
		return (1);
	ft_memset(nopt, 0, sizeof(t_opt));
	ft_strcpy(nopt->opt, arg);
	nopt->type = OPT_FUNC;
	nopt->func = func;
	nopt->next = *opt;
	*opt = nopt;
	return (0);
}
