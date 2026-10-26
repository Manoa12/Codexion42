/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_thread.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:47:05 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 06:47:06 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_join_coder_threads(pthread_t *threads, int count)
{
	int	i;
	int	ret;

	i = 0;
	while (i < count)
	{
		ret = pthread_join(threads[i], NULL);
		if (ret != 0)
			return (1);
		i++;
	}
	return (0);
}

static int	ft_coder_threads(t_env *env, pthread_t *threads)
{
	t_coder	*coder;
	int		count;
	int		ret;

	count = 0;
	coder = env->coders;
	while (coder)
	{
		ret = pthread_create(&(threads[count]), NULL, ft_coder_routine, coder);
		if (ret != 0)
		{
			while (count > 0)
			{
				count--;
				pthread_join(threads[count], NULL);
			}
			return (-1);
		}
		coder = coder->next;
		count++;
	}
	return (count);
}

int	ft_init_threads(t_env *env)
{
	pthread_t	monitor;
	pthread_t	*coder_threads;
	int			created;
	int			ret;

	coder_threads = (pthread_t *)malloc(sizeof(pthread_t)
			* env->args->nb_coders);
	if (!coder_threads)
		return (1);
	ret = pthread_create(&monitor, NULL, ft_monitor_routine, env);
	if (ret != 0)
		return (free(coder_threads), 1);
	created = ft_coder_threads(env, coder_threads);
	if (created < 0)
	{
		pthread_join(monitor, NULL);
		return (free(coder_threads), 1);
	}
	ret = pthread_join(monitor, NULL);
	ret = ft_join_coder_threads(coder_threads, created);
	free(coder_threads);
	if (ret == 0)
		return (0);
	return (1);
}
