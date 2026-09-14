/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_coders.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:46:15 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 06:46:16 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	ft_assign_dongles(t_coder *coder, t_env *env, int id)
{
	int	next_id;

	if (env->args->nb_coders > 1)
	{
		next_id = (id + 1) % env->args->nb_coders;
		if (id < next_id)
		{
			coder->left_dongle = ft_get_dongle_by_id(env, id);
			coder->right_dongle = ft_get_dongle_by_id(env, next_id);
		}
		else
		{
			coder->left_dongle = ft_get_dongle_by_id(env, next_id);
			coder->right_dongle = ft_get_dongle_by_id(env, id);
		}
		return ;
	}
	coder->left_dongle = ft_get_dongle_by_id(env, id);
	coder->right_dongle = NULL;
}

static t_coder	*ft_coder_new(int id, t_env *env)
{
	t_coder	*coder;

	coder = (t_coder *)malloc(sizeof(t_coder));
	if (!coder)
		return (NULL);
	coder->id = id + 1;
	pthread_mutex_init(&(coder->state_lock), NULL);
	coder->code_compiled = 0;
	coder->have_finished = 0;
	coder->time_burnout = ft_get_current_time();
	coder->env = env;
	ft_assign_dongles(coder, env, id);
	coder->next = NULL;
	return (coder);
}

static void	ft_coderadd_back(t_coder **coders, t_coder *coder)
{
	t_coder	*current;

	if (!coders || !coder)
		return ;
	if (*coders)
	{
		current = *coders;
		while (current->next)
			current = current->next;
		current->next = coder;
	}
	else
		*coders = coder;
}

void	ft_coders_free(t_coder **coders)
{
	t_coder	*current;
	t_coder	*next;

	if (!coders)
		return ;
	current = *coders;
	while (current)
	{
		next = current->next;
		pthread_mutex_destroy(&(current->state_lock));
		free(current);
		current = next;
	}
	*coders = NULL;
}

t_coder	*ft_init_coders(t_env *env)
{
	t_coder	*coders;
	t_coder	*node;
	int		i;

	i = 0;
	coders = NULL;
	while (i < env->args->nb_coders)
	{
		node = ft_coder_new(i, env);
		if (!node)
		{
			ft_coders_free(&coders);
			return (NULL);
		}
		ft_coderadd_back(&coders, node);
		i++;
	}
	return (coders);
}
