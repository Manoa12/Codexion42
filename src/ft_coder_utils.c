/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_coder_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:45:56 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 06:45:57 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_get_have_finished(t_coder *coder)
{
	int	finished;

	finished = 0;
	pthread_mutex_lock(&coder->state_lock);
	finished = coder->have_finished;
	pthread_mutex_unlock(&coder->state_lock);
	return (finished);
}

long long	ft_get_burnout(t_coder *coder)
{
	long long	burnout;

	burnout = 0;
	pthread_mutex_lock(&coder->state_lock);
	burnout = coder->time_burnout;
	pthread_mutex_unlock(&coder->state_lock);
	return (burnout);
}

void	ft_set_finished(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_lock);
	coder->have_finished = 1;
	pthread_mutex_unlock(&coder->state_lock);
}

void	ft_set_burnout(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_lock);
	coder->time_burnout = ft_get_current_time();
	pthread_mutex_unlock(&coder->state_lock);
}

t_coder	*ft_get_coder_by_id(t_env *env, int id)
{
	t_coder	*coder;

	coder = env->coders;
	while (coder)
	{
		if (coder->id == id)
			return (coder);
		coder = coder->next;
	}
	return (NULL);
}
