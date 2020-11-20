#include "libft.h"

int		opt_addvar2(t_opt **opt, char *arg, void **var, char type_var)
{
	t_opt *nopt;

	if (!(nopt = malloc(sizeof(t_opt))))
		return 1;
	ft_strcpy(nopt->opt, arg);
	nopt->var2 = var;
	nopt->type = OPT_VAR;
	nopt->type_var = type_var;
	nopt->next = *opt;
	*opt = nopt;
	return 0;
}

int		opt_addvar(t_opt **opt, char *arg, void *var, char type_var)
{
	t_opt *nopt;

	if (!(nopt = malloc(sizeof(t_opt))))
		return 1;
	ft_strcpy(nopt->opt, arg);
	nopt->var = var;
	nopt->type = OPT_VAR;
	nopt->type_var = type_var;
	nopt->next = *opt;
	*opt = nopt;
	return 0;
}

void	opt_init(t_opt **opt)
{
	*opt = NULL;	
}

t_opt	*isoptin(t_opt *opt, char *arg)
{
	while (opt)
	{
		if (!ft_strcmp(arg, opt->opt))
			return opt;
		opt = opt->next;
	}
	return NULL;
}

int		opt_parseopt(t_opt *mopt, char ***argv)
{
	char **arg;

	arg = *argv;
	if (!arg[1])
	{
		ft_printf("error pas assez d'arg\n");
		return OPT_MISSARG;
	}
	if (mopt->type_var == OPT_STR)
		*mopt->var2 = arg[1];
	else if (mopt->type_var == OPT_INT)
	{
		if (!ft_isalldigit(arg[1]))
		{
			ft_printf("invalid argument: '%s'\n", arg[1]);
			return OPT_IVLARG;
		}
		*(int*)mopt->var = ft_atoi(arg[1]);
		ft_printf("ttl: %d\n", *(int*)mopt->var);
	}
	(*argv)++;
	return OPT_OK;
}

void	opt_free(t_opt **opt)
{
	t_opt *nopt;

	nopt = *opt;
	while (nopt)
	{
		free(nopt);
		nopt = nopt->next;
	}
	*opt = NULL;
}

int		opt_parser(t_opt *opt, char **arg, t_optpars *optpars)
{
	t_opt	*mopt;
	char	end;
	int ret;

	end = 0;
	optpars->arg[0] = NULL;
	optpars->opt[0] = NULL;
	while (*arg)
	{
		if (!ft_strlen(*arg) || end)
		{
			ft_printf("arg: %s\n", *arg); // ajouter au arg
			ft_tabadd(optpars->arg, *arg);
		}
		else if (!ft_strcmp(*arg, "--"))
			end = 1;
		else if (!ft_strncmp(*arg, "-", 1))
		{
			if ((mopt = isoptin(opt, *arg)))
			{
				ft_printf("opt: %s\n", *arg);
				ft_tabadd(optpars->opt, *arg);
				if (mopt->var)
					if ((ret = opt_parseopt(mopt, &arg)))
						return ret;
			}
			else
			{
				ft_printf("not found: %s\n", *arg);
				return OPT_NFOUND;
			}
		}
		else 
		{
			ft_printf("arg: %s\n", *arg); // ajouter au arg
			ft_tabadd(optpars->arg, *arg);
		}
		arg++;
	}
	return 0;
}
