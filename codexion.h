/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:38:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/17 17:07:14 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <time.h>
# include <sys/resource.h>
# include <sys/time.h>

struct	s_dongle;

enum e_task
{
	COMPILE,
	DEBUG,
	REFACTOR
};

typedef struct s_simulation
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;
	long long		start_time;
	int				stop;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	log_mutex;
}	t_simulation;

typedef struct s_coder
{
	int				number;
	long long		last_compile_start;
	long long		request_ts;
	int				compile_count;
	enum e_task		task;
	pthread_t		*thread;
	struct s_dongle	*l_dongle;
	struct s_dongle	*r_dongle;
	t_simulation	*sim;
	pthread_mutex_t	mutex;
}	t_coder;

typedef struct s_dongle
{
	int				id;
	int				cooldown;
	int				busy;
	long long		active_timestamp;
	t_coder			*queue[2];
	pthread_mutex_t	mutex;
	pthread_cond_t	wake_cond;
}	t_dongle;

t_simulation	*parser(int argc, char **argv);
void			arg_error(void);
void			error_and_exit(t_simulation *config);
void			thread_error(t_simulation *config, t_coder *coders, int i);
void			mutex_error(t_simulation *sim, t_coder *coders, int i);
void			*coder_thread(void *work);
void			gen_coder_threads(t_coder *coders, t_simulation *sim);
long long		get_time_ms(void);
void			gen_coders_and_dongles(t_coder *coders, t_simulation *sim,
					pthread_t *monitoring_thread);
void			*mon_thread(void *coders);
void			coder_compile(t_coder *coder);
void			coder_debug(t_coder *coder);
void			coder_refactor(t_coder *coder);
void			gen_simulation(t_coder *coders, t_simulation *sim,
					pthread_t *monitoring_thread);
void			free_coders(t_coder *coders, t_simulation *sim);
int				acquire_dongles(t_coder *coder);
void			release_dongles(t_coder *coder);
int				is_burned_out(t_coder *coders, t_simulation *sim);
int				is_everyone_finished(t_coder *coders, t_simulation *sim);
long long		compute_deadline(t_coder *coder);
void			queue_add(t_dongle *dongle, t_coder *coder);
int				occupy_dongle(t_dongle *dongle, t_coder *coder);
void			queue_remove(t_dongle *dongle, t_coder *coder);
int				is_next(t_dongle *dongle, t_coder *coder, t_simulation *sim);
void			dongle_error(int i, t_coder *coders, t_simulation *sim,
					pthread_t *monitoring_thread);
void			thread_print(t_coder *coder, char *string);
void			release_dongle(t_dongle *dongle, t_coder *coder);

#endif