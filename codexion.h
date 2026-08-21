/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:38:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/21 17:07:38 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <time.h>
# include <sys/resource.h>
# include <sys/time.h>

typedef struct s_setting
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}	t_settings;

typedef struct s_dongle
{
	int				id;
	int				cooldown;
}	t_dongle;

typedef struct s_coder
{
	int			number;
	time_t		last_compile_start;
	int			compile_count;
	pthread_t	*thread;
	t_dongle	*l_dongle;
	t_dongle	*r_dongle;
}	t_coder;

typedef struct s_simulation
{
	t_settings	*config;
	t_coder		*coders;
	long		start_time;
}	t_simulation;

typedef struct s_work
{
	t_coder			*coder;
	t_simulation	*sim;
}	t_work;

t_settings	*parser(int argc, char **argv);
void		arg_error(void);
void		error_and_exit(t_settings *config);
void		*coder_thread(void *work);
void		gen_coder_threads(t_coder *coders, t_settings *config);
long		get_time_ms(void);

#endif