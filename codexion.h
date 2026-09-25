/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:38:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/25 12:32:50 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <time.h>
# include <sys/resource.h>
# include <sys/time.h>

typedef struct s_heap
{
	struct s_coder		**data;
	int					size;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	bool			busy;
	int				cooldown;
	long long		release_ts;
	t_heap			heap;
	pthread_mutex_t	mutex;
	pthread_cond_t	wake_cond;
}	t_dongle;

typedef struct s_coder
{
	int					id;
	long long			priority_ts;
	int					request_number;
	int					compile_count;
	long long			last_compile_start;
	t_dongle			*l_dongle;
	t_dongle			*r_dongle;
	pthread_t			thread;
	pthread_mutex_t		mutex;
	struct s_simulation	*sim;
}	t_coder;

typedef struct s_simulation
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				request_count;
	char			*scheduler;
	long long		start_time;
	bool			stop;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	request_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	log_mutex;
}	t_simulation;

char			**args_verify(int argc, char **argv);
void			*coder_thread(void *work);
long long		get_time_ms(void);
int				gen_coders(t_simulation *sim);
void			*mon_thread(void *arg);
int				gen_simulation(t_simulation *sim, char **args);
bool			take_both_dongles(t_simulation *sim, t_coder *coder);
void			release_both_dongles(t_coder *coder);
long long		compute_deadline(t_coder *coder);
void			thread_print(t_coder *coder, char *string);
void			min_heap_push(t_simulation *sim, t_heap *heap,
					t_coder *coder);
void			min_heap_pop(t_heap *heap, t_simulation *sim);
bool			should_stop_now(t_simulation *sim);
void			ft_swap(t_coder **coder_a, t_coder **coder_b);
bool			is_burned_out(t_coder *coder, t_simulation *sim);
bool			is_finished(t_simulation *sim, t_coder *coder);
void			get_request_number(t_simulation *sim, t_coder *coder);
bool			has_higher_priority(t_simulation *sim, t_coder *coder_a,
					t_coder *coder_b);
int				gen_dongles(t_simulation *sim);
int				init_dongle_heap(t_dongle *dongle, int capacity);

#endif