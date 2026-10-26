/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:47:40 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 10:42:34 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	codexion(t_args *args)
{
	t_env	env;

	memset(&env, 0, sizeof(env));
	env.args = args;
	if (ft_init_simulation(&env) != 0)
		return (1);
	if (ft_init_threads(&env) != 0)
	{
		ft_clean_simulation(&env);
		return (1);
	}
	ft_clean_simulation(&env);
	return (0);
}

int	main(int ac, char *av[])
{
	t_args	args;
	int		ret;

	if (ac == 1)
		return (0);
	if ((ac - 1) != 8)
	{
		ft_print_arg_instruction();
		return (1);
	}
	if (!ft_args_is_valid(av + 1))
		return (1);
	ft_parse_args(&(args), av + 1);
	ret = codexion(&(args));
	return (ret);
}
