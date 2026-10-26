/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dongles.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:46:30 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 06:46:31 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_dongle	*ft_dongle_new(int id, t_env *env)
{
	t_dongle	*dongle;

	dongle = (t_dongle *)malloc(sizeof(t_dongle));
	if (!dongle)
		return (NULL);
	dongle->id = id;
	dongle->is_available = 1;
	dongle->available_at = 0;
	dongle->env = env;
	dongle->next = NULL;
	ft_heap_init(&dongle->queue, env->args->nb_coders);
	pthread_mutex_init(&(dongle->state_lock), NULL);
	pthread_cond_init(&(dongle->state_cond), NULL);
	return (dongle);
}

static void	ft_dongleadd_back(t_dongle **dongles, t_dongle *dongle)
{
	t_dongle	*current;

	if (!dongles || !dongle)
		return ;
	if (*dongles)
	{
		current = *dongles;
		while (current->next)
			current = current->next;
		current->next = dongle;
	}
	else
		*dongles = dongle;
}

t_dongle	*ft_get_dongle_by_id(t_env *env, int id)
{
	t_dongle	*dongle;

	dongle = env->dongles;
	while (dongle)
	{
		if (dongle->id == id)
			return (dongle);
		dongle = dongle->next;
	}
	return (NULL);
}

void	ft_dongles_free(t_dongle **dongles)
{
	t_dongle	*current;
	t_dongle	*next;

	if (!dongles)
		return ;
	current = *dongles;
	while (current)
	{
		next = current->next;
		ft_heap_destroy(&(current->queue));
		pthread_mutex_destroy(&(current->state_lock));
		pthread_cond_destroy(&(current->state_cond));
		free(current);
		current = next;
	}
	*dongles = NULL;
}

t_dongle	*ft_init_dongles(t_env *env)
{
	t_dongle	*dongles;
	t_dongle	*node;
	int			i;

	i = 0;
	dongles = NULL;
	while (i < env->args->nb_coders)
	{
		node = ft_dongle_new(i, env);
		if (!node)
		{
			ft_dongles_free(&dongles);
			return (NULL);
		}
		ft_dongleadd_back(&dongles, node);
		i++;
	}
	return (dongles);
}
