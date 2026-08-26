/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:38:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/26 11:43:51 by dcoelho          ###   ########.fr       */
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

enum e_task
{
	COMPILE,
	DEBUG,
	REFACTOR
};

typedef struct s_dongle
{
	int				id;
	int				cooldown;
}	t_dongle;

typedef struct s_simulation
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
	long	start_time;
	int		stop;
}	t_simulation;

typedef struct s_coder
{
	int				number;
	time_t			last_compile_start;
	int				compile_count;
	enum e_task		task;
	pthread_t		*thread;
	t_dongle		*l_dongle;
	t_dongle		*r_dongle;
	t_simulation	*sim;
}	t_coder;

t_simulation	*parser(int argc, char **argv);
void			arg_error(void);
void			error_and_exit(t_simulation *config);
void			thread_error(t_simulation *config, t_coder *coders, int i);
void			*coder_thread(void *work);
void			gen_coder_threads(t_coder *coders, t_simulation *sim);
long			get_time_ms(void);
void			launch_mon_thread(pthread_t *mon_thread, t_coder *coders);

#endif