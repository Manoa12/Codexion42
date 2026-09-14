/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:47:13 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 06:47:14 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	ft_get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) != 0)
		return (0);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long long	ft_get_time_from_start(t_env *env)
{
	long long	current_time;

	current_time = ft_get_current_time() - env->start_time;
	return (current_time);
}
