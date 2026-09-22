/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:38:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/22 17:13:22 by dcoelho          ###   ########.fr       */
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

typedef enum e_task
{
	COMPILE,
	DEBUG,
	REFACTOR
}	t_task;

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
	long long		release_timestamp;
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
	t_task				task;
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
void			arg_error(void);
void			error_and_exit(t_simulation *config);
void			thread_error(t_simulation *config, t_coder *coders, int i);
void			mutex_error(t_simulation *sim, t_coder *coders, int i);
void			*coder_thread(void *work);
void			gen_coder_threads(t_coder *coders, t_simulation *sim);
long long		get_time_ms(void);
int				gen_coders(t_simulation *sim);
void			*mon_thread(void *coders);
void			coder_compile(t_coder *coder);
void			coder_debug(t_coder *coder);
void			coder_refactor(t_coder *coder);
int				gen_simulation(t_simulation *sim, char **args);
void			free_coders(t_coder *coders, t_simulation *sim);
bool			take_both_dongles(t_coder *coder);
void			release_both_dongles(t_coder *coder);
bool			check_coders(t_simulation *sim);
long long		compute_deadline(t_coder *coder);
bool			take_dongle(t_dongle *dongle, t_coder *coder);
void			dongle_error(int i, t_coder *coders, t_simulation *sim,
					pthread_t *monitoring_thread);
void			thread_print(t_coder *coder, char *string);
void			release_dongle(t_dongle *dongle);
int				min_heap_push(t_simulation *sim, t_heap *heap,
					t_coder *new_coder);
long long		priority_tie_breaker(t_simulation *sim, t_coder *coder);
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