/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_opt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeyret <apeyret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 16:01:30 by apeyret           #+#    #+#             */
/*   Updated: 2021/04/12 16:01:33 by apeyret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

t_opt	*isoptin(t_opt *opt, char *arg)
{
	while (opt)
	{
		if (!ft_strcmp(arg, opt->opt))
			return (opt);
		opt = opt->next;
	}
	return (NULL);
}

int		opt_parseopt(t_opt *mopt, char ***argv, char *name)
{
	char **arg;

	arg = *argv;
	if (mopt->type == OPT_FUNC)
	{
		if (mopt->func(NULL, *arg))
			return (OPT_FUNCERR);
	}
	else if (!arg[1])
	{
		printf("%s: option requires an argument -- '%s'\n", name, arg[0]);
		return (OPT_MISSARG);
	}
	else if (mopt->type == OPT_VAR && mopt->type_var == OPT_STR)
		*mopt->var2 = arg[1];
	else if (mopt->type == OPT_VAR && mopt->type_var == OPT_INT)
	{
		if (!ft_isint(arg[1]))
		{
			printf("%s: invalid argument: '%s'\n", name, arg[1]);
			return (OPT_IVLARG);
		}
		*(int*)mopt->var = ft_atoi(arg[1]);
	}
	(*argv)++;
	return (OPT_OK);
}

void	opt_free(t_opt **opt)
{
	t_opt *nopt;

	nopt = *opt;
	while (*opt)
	{
		nopt = (*opt)->next;
		free(*opt);
		*opt = nopt;
	}
	*opt = NULL;
}

int		opt_parser(t_opt *opt, char **arg, t_optpars *optpars, char *name)
{
	t_opt	*mopt;
	char	end;
	int		ret;

	end = 0;
	optpars->arg[0] = NULL;
	optpars->opt[0] = NULL;
	while (*arg)
	{
		if (!ft_strlen(*arg) || end)
			ft_tabadd(optpars->arg, *arg);
		else if (!ft_strcmp(*arg, "--"))
			end = 1;
		else if (!ft_strncmp(*arg, "-", 1))
		{
			if ((mopt = isoptin(opt, *arg)))
			{
				ft_tabadd(optpars->opt, *arg);
				if (mopt->var || mopt->func)
					if ((ret = opt_parseopt(mopt, &arg, name)))
						return (ret);
			}
			else
			{
				printf("%s: invalid option -- '%s'\n", name, *arg);
				return (OPT_NFOUND);
			}
		}
		else
			ft_tabadd(optpars->arg, *arg);
		arg++;
	}
	return (0);
}
