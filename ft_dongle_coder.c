/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dongle_coder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:46:23 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 06:46:24 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_try_take_dongle(t_dongle *dongle, int coder_id, long long now,
		t_heap_entry *top)
{
	if (!ft_heap_peek(&dongle->queue, top))
		return (0);
	if (top->coder_id != coder_id)
		return (0);
	if (!dongle->is_available)
		return (0);
	if (now < dongle->available_at)
		return (0);
	return (1);
}

static int	ft_grant_dongle(t_dongle *dongle, t_heap_entry *top)
{
	ft_heap_pop(&dongle->queue, top);
	dongle->is_available = 0;
	pthread_mutex_unlock(&dongle->state_lock);
	return (1);
}

static int	ft_take_dongle(t_coder *coder, t_dongle *dongle)
{
	t_heap_entry	top;
	long long		now;

	pthread_mutex_lock(&dongle->state_lock);
	if (!ft_heap_push(&dongle->queue, coder))
	{
		pthread_mutex_unlock(&dongle->state_lock);
		return (0);
	}
	while (ft_is_simulation_running(coder->env))
	{
		now = ft_get_current_time();
		if (ft_try_take_dongle(dongle, coder->id, now, &top))
			return (ft_grant_dongle(dongle, &top));
		pthread_mutex_unlock(&dongle->state_lock);
		usleep(500);
		pthread_mutex_lock(&dongle->state_lock);
	}
	pthread_mutex_unlock(&dongle->state_lock);
	return (0);
}

void	ft_release_dongle(t_env *env, t_dongle *dongle)
{
	long	cooldown;

	if (!dongle)
		return ;
	cooldown = env->args->dongle_cooldown;
	pthread_mutex_lock(&dongle->state_lock);
	dongle->is_available = 1;
	dongle->available_at = ft_get_current_time() + cooldown;
	pthread_mutex_unlock(&dongle->state_lock);
}

int	ft_acquire_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->left_dongle;
	second = coder->right_dongle;
	if (!first || !second || first == second)
	{
		while (ft_is_simulation_running(coder->env))
			usleep(500);
		return (0);
	}
	if (!ft_take_dongle(coder, first))
		return (0);
	if (!ft_take_dongle(coder, second))
	{
		ft_release_dongle(coder->env, first);
		return (0);
	}
	ft_print_taken_dongle(coder);
	return (1);
}
