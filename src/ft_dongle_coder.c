/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dongle_coder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:46:23 by nfitahin          #+#    #+#             */
/*   Updated: 2026/10/24 08:26:17 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_can_take(t_dongle *dongle, int coder_id, long long now)
{
	t_heap_entry	top;

	if (!ft_heap_peek(&dongle->queue, &top))
		return (0);
	return (top.coder_id == coder_id && dongle->is_available
		&& now >= dongle->available_at);
}

static int	ft_register(t_coder *coder)
{
	int	ok;

	pthread_mutex_lock(&coder->left_dongle->state_lock);
	pthread_mutex_lock(&coder->right_dongle->state_lock);
	ok = ft_heap_push(&coder->left_dongle->queue, coder);
	if (ok)
		ok = ft_heap_push(&coder->right_dongle->queue, coder);
	pthread_mutex_unlock(&coder->right_dongle->state_lock);
	pthread_mutex_unlock(&coder->left_dongle->state_lock);
	return (ok);
}

static int	ft_try_acquire(t_coder *coder)
{
	t_dongle		*first;
	t_dongle		*second;
	t_heap_entry	top;
	int				granted;

	first = coder->left_dongle;
	second = coder->right_dongle;
	pthread_mutex_lock(&first->state_lock);
	pthread_mutex_lock(&second->state_lock);
	granted = ft_can_take(first, coder->id, ft_get_current_time())
		&& ft_can_take(second, coder->id, ft_get_current_time());
	if (granted)
	{
		ft_heap_pop(&first->queue, &top);
		ft_heap_pop(&second->queue, &top);
		first->is_available = 0;
		second->is_available = 0;
	}
	pthread_mutex_unlock(&second->state_lock);
	pthread_mutex_unlock(&first->state_lock);
	return (granted);
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
	if (!coder->left_dongle || !coder->right_dongle
		|| coder->left_dongle == coder->right_dongle)
	{
		while (ft_is_simulation_running(coder->env))
			usleep(500);
		return (0);
	}
	if (!ft_register(coder))
		return (0);
	while (ft_is_simulation_running(coder->env))
	{
		if (ft_try_acquire(coder))
		{
			ft_print_taken_dongle(coder);
			return (1);
		}
		usleep(200);
	}
	return (0);
}
