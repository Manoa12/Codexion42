/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_coder_routine.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:45:49 by nfitahin          #+#    #+#             */
/*   Updated: 2026/10/24 08:25:51 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	action_compile(t_coder *coder)
{
	if (!ft_is_simulation_running(coder->env))
		return ;
	ft_set_burnout(coder);
	ft_print_state(coder, "is compiling");
	ft_sleep(coder->env, coder->env->args->compile_time);
	if (!ft_is_simulation_running(coder->env))
		return ;
	coder->code_compiled += 1;
	if (coder->code_compiled >= coder->env->args->nb_compiles)
		ft_set_finished(coder);
}

static void	action_debug(t_coder *coder)
{
	if (!ft_is_simulation_running(coder->env))
		return ;
	ft_print_state(coder, "is debugging");
	ft_sleep(coder->env, coder->env->args->debug_time);
}

static void	action_refactor(t_coder *coder)
{
	if (!ft_is_simulation_running(coder->env))
		return ;
	ft_print_state(coder, "is refactoring");
	ft_sleep(coder->env, coder->env->args->refactor_time);
}

static void	run_cycle(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->left_dongle;
	second = coder->right_dongle;
	action_compile(coder);
	ft_release_dongle(coder->env, second);
	ft_release_dongle(coder->env, first);
	action_debug(coder);
	action_refactor(coder);
}

void	*ft_coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (ft_is_simulation_running(coder->env) && !(coder->have_finished))
	{
		if (!ft_acquire_dongles(coder))
			break ;
		run_cycle(coder);
	}
	return (NULL);
}
