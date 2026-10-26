/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_writer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:47:34 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 06:47:36 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_putstr_fd(char *str, int fd)
{
	size_t	length;

	if (!str)
		return ;
	length = strlen(str);
	write(fd, str, length);
}

void	ft_print_state(t_coder *coder, char *msg)
{
	t_env	*env;

	env = coder->env;
	pthread_mutex_lock(&(env->print_mutex));
	printf("%lld %d %s\n", ft_get_time_from_start(env), coder->id, msg);
	pthread_mutex_unlock(&(env->print_mutex));
}

void	ft_print_taken_dongle(t_coder *coder)
{
	long long	start_time;
	t_env		*env;

	env = coder->env;
	pthread_mutex_lock(&(env->print_mutex));
	start_time = ft_get_time_from_start(env);
	printf("%lld %d has taken a dongle\n", start_time, coder->id);
	printf("%lld %d has taken a dongle\n", start_time, coder->id);
	pthread_mutex_unlock(&(env->print_mutex));
}

void	ft_print_arg_instruction(void)
{
	ft_putstr_fd("Args error: Error argument number\n", STDERR_FILENO);
	ft_putstr_fd("Usage: ./codexion <number_of_coders> ", STDERR_FILENO);
	ft_putstr_fd("<time_to_burnout> <time_to_compile> ", STDERR_FILENO);
	ft_putstr_fd("<time_to_debug> <time_to_refactor> ", STDERR_FILENO);
	ft_putstr_fd("<number_of_compiles_required> ", STDERR_FILENO);
	ft_putstr_fd("<dongle_cooldown> <scheduler>\n", STDERR_FILENO);
	ft_putstr_fd("\nExample: ", STDERR_FILENO);
	ft_putstr_fd("./codexion 4 1200 200 200 200 5 10 fifo\n", STDERR_FILENO);
}
