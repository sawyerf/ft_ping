#include "libft.h"

int		opt_addvar2(t_opt **opt, char *arg, void **var, char type_var)
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

int		opt_addvar(t_opt **opt, char *arg, void *var, char type_var)
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

int		opt_parser(t_opt *opt, char **arg)
{
	t_opt	*mopt;
	char	end;

	end = 0;
	while (*arg)
	{
		if (!ft_strlen(*arg) || end)
		{
			ft_printf("arg: %s\n", *arg); // ajouter au arg
		}
		else if (!ft_strcmp(*arg, "--"))
			end = 1;
		else if (!ft_strncmp(*arg, "-", 1))
		{
			if ((mopt = isoptin(opt, *arg)))
			{
				ft_printf("opt: %s\n", *arg);
				if (mopt->var)
				{
					if (!arg[1])
					{
						ft_printf("error pas assez d'arg\n");
						return (1);
					}
					if (mopt->type_var == OPT_STR)
						*opt->var2 = arg[1];
					else if (mopt->type_var == OPT_INT)
					{
						ft_printf("2\n");
						if (!ft_isalldigit(arg[1]))
						{
							ft_printf("3\n");
							ft_printf("invalid argument: '%s'\n", arg[1]);
							return 1;
						}
						ft_printf("4\n");
						*(int*)opt->var = 1; //ft_atoi(arg[1]);
						ft_printf("5\n");
					}
					arg++;
				}
			}
			else
			ft_printf("not found: %s\n", *arg);
		}
		else 
			ft_printf("arg: %s\n", *arg); // ajouter au arg
		arg++;
	}
	return 0;
}
