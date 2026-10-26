/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_simulation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:46:58 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 06:47:00 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_init_simulation(t_env *env)
{
	pthread_mutex_init(&env->print_mutex, NULL);
	pthread_mutex_init(&env->state_mutex, NULL);
	env->dongles = ft_init_dongles(env);
	env->coders = ft_init_coders(env);
	env->start_time = ft_get_current_time();
	env->is_running = 1;
	return (0);
}

void	ft_clean_simulation(t_env *env)
{
	if (env->coders)
		ft_coders_free(&(env->coders));
	if (env->dongles)
		ft_dongles_free(&(env->dongles));
	env->coders = NULL;
	env->dongles = NULL;
	pthread_mutex_destroy(&(env->print_mutex));
	pthread_mutex_destroy(&(env->state_mutex));
}

int	ft_heap_init(t_heap *heap, int capacity)
{
	if (!heap || capacity <= 0)
		return (0);
	heap->data = malloc(sizeof(t_heap_entry) * capacity);
	if (!heap->data)
		return (0);
	heap->size = 0;
	heap->capacity = capacity;
	heap->next_seq = 0;
	return (1);
}

void	ft_heap_destroy(t_heap *heap)
{
	if (!heap)
		return ;
	free(heap->data);
	heap->data = NULL;
	heap->size = 0;
	heap->capacity = 0;
	heap->next_seq = 0;
}
