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

int		opt_addvar2(t_opt **opt, char *arg, void **var, char type_var)
{
	t_opt *nopt;

	if (!(nopt = malloc(sizeof(t_opt))))
		return (1);
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
	ft_strcpy(nopt->opt, arg);
	nopt->type = OPT_FUNC;
	nopt->func = func;
	nopt->next = *opt;
	*opt = nopt;
	return (0);
}

void	opt_init(t_opt **opt)
{
	*opt = NULL;
}

t_opt	*isoptin(t_opt *opt, char *arg)
{
	while (opt)
	{
		printf("%s %s", arg, opt->opt);
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
	printf("arg: %s\n", *arg);
	if (!arg[1])
	{
		printf("%s: option requires an argument -- '%s'\n", name, arg[0]);
		return (OPT_MISSARG);
	}
	if (mopt->type == OPT_ARG && mopt->type_var == OPT_STR)
		*mopt->var2 = arg[1];
	else if (mopt->type == OPT_ARG && mopt->type_var == OPT_INT)
	{
		if (!ft_isint(arg[1]))
		{
			printf("%s: invalid argument: '%s'\n", name, arg[1]);
			return (OPT_IVLARG);
		}
		*(int*)mopt->var = ft_atoi(arg[1]);
	}
	else if (mopt->type == OPT_FUNC)
	{
		mopt->func(NULL, *arg);
		printf("func\n");
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
				if (mopt->var)
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
