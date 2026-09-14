/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:45:30 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 10:42:34 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "ft_type.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

void		ft_parse_args(t_args *args, char *av[]);
long		ft_check_number(const char *str);
int			ft_args_is_valid(char *av[]);

t_coder		*ft_init_coders(t_env *env);
t_coder		*ft_get_coder_by_id(t_env *env, int id);
void		ft_coders_free(t_coder **coders);
int			ft_get_have_finished(t_coder *coder);
long long	ft_get_burnout(t_coder *coder);
void		ft_set_finished(t_coder *coder);
void		ft_set_burnout(t_coder *coder);

t_dongle	*ft_init_dongles(t_env *env);
t_dongle	*ft_get_dongle_by_id(t_env *env, int id);
void		ft_dongles_free(t_dongle **dongles);

void		*ft_monitor_routine(void *arg);
int			ft_init_threads(t_env *env);
void		*ft_coder_routine(void *arg);

int			ft_heap_init(t_heap *heap, int capacity);
int			ft_heap_push(t_heap *heap, t_coder *coder);
int			ft_heap_pop(t_heap *heap, t_heap_entry *out);
int			ft_heap_peek(t_heap *heap, t_heap_entry *out);
void		ft_heap_destroy(t_heap *heap);

int			ft_acquire_dongles(t_coder *coder);
void		ft_release_dongle(t_env *env, t_dongle *dongle);

int			ft_init_simulation(t_env *env);
void		ft_clean_simulation(t_env *env);
int			ft_is_simulation_running(t_env *env);

long long	ft_get_time_from_start(t_env *env);
long long	ft_get_current_time(void);

void		ft_print_taken_dongle(t_coder *coder);
void		ft_putstr_fd(char *str, int fd);
void		ft_print_state(t_coder *coder, char *msg);
void		ft_print_arg_instruction(void);

#endif
