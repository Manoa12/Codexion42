/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_monitor_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:46:45 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 06:46:46 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	ft_stop_simulation(t_env *env)
{
	pthread_mutex_lock(&env->state_mutex);
	env->is_running = 0;
	pthread_mutex_unlock(&env->state_mutex);
}

int	ft_is_simulation_running(t_env *env)
{
	int	status;

	status = 0;
	pthread_mutex_lock(&env->state_mutex);
	status = env->is_running;
	pthread_mutex_unlock(&env->state_mutex);
	return (status);
}

static int	ft_check_burns_out(t_env *env, int *finish)
{
	t_coder		*coder;
	long long	remains;
	long long	current_time;

	*finish = 0;
	coder = env->coders;
	current_time = ft_get_current_time();
	while (coder)
	{
		remains = current_time - ft_get_burnout(coder);
		if (ft_get_have_finished(coder))
			(*finish)++;
		else if (remains > env->args->burnout_time)
		{
			ft_stop_simulation(env);
			ft_print_state(coder, "burned out");
			return (1);
		}
		coder = coder->next;
	}
	return (0);
}

void	*ft_monitor_routine(void *arg)
{
	t_env	*env;
	int		finish;

	env = (t_env *)arg;
	while (!ft_is_simulation_running(env))
		usleep(100);
	while (ft_is_simulation_running(env))
	{
		if (ft_check_burns_out(env, &finish))
			return (NULL);
		if (finish == env->args->nb_coders)
		{
			ft_stop_simulation(env);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
