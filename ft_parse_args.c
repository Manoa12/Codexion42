/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:46:51 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 10:42:34 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_scheduler	ft_parse_scheduler(char *str)
{
	if (strcmp(str, "fifo") == 0)
		return (SCHEDULER_FIFO);
	if (strcmp(str, "edf") == 0)
		return (SCHEDULER_EDF);
	return (SCHEDULER_UNKNOWN);
}

static int	ft_check_valid_time(char *av[])
{
	if (ft_check_number(av[1]) <= 0)
	{
		ft_putstr_fd("Args Error: Invalid burnout time\n", 2);
		return (0);
	}
	if (ft_check_number(av[2]) < 0)
	{
		ft_putstr_fd("Args Error: Invalid compile time\n", 2);
		return (0);
	}
	if (ft_check_number(av[3]) < 0)
	{
		ft_putstr_fd("Args Error: Invalid debug time\n", 2);
		return (0);
	}
	if (ft_check_number(av[4]) < 0)
	{
		ft_putstr_fd("Args Error: Invalid refactor time\n", 2);
		return (0);
	}
	return (1);
}

static int	ft_check_valid_number(char *av[])
{
	if (ft_check_number(av[0]) <= 0)
	{
		ft_putstr_fd("Args Error: Invalid nb coders\n", 2);
		return (0);
	}
	if (ft_check_number(av[5]) <= 0)
	{
		ft_putstr_fd("Args Error: Invalid compile numbers\n", 2);
		return (0);
	}
	if (ft_check_number(av[6]) < 0)
	{
		ft_putstr_fd("Args Error: Invalid dongle cooldown\n", 2);
		return (0);
	}
	return (1);
}

int	ft_args_is_valid(char *av[])
{
	if (ft_parse_scheduler(av[7]) == SCHEDULER_UNKNOWN)
	{
		ft_putstr_fd("Args Error: unknown scheduler type\n", 2);
		return (0);
	}
	if (!ft_check_valid_number(av))
		return (0);
	if (!ft_check_valid_time(av))
		return (0);
	return (1);
}

void	ft_parse_args(t_args *args, char *av[])
{
	args->nb_coders = atoi(av[0]);
	args->burnout_time = atoi(av[1]);
	args->compile_time = atoi(av[2]);
	args->debug_time = atoi(av[3]);
	args->refactor_time = atoi(av[4]);
	args->nb_compiles = atoi(av[5]);
	args->dongle_cooldown = atoi(av[6]);
	args->scheduler = ft_parse_scheduler(av[7]);
}
