/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 12:07:29 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/25 12:22:10 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	coder_compile(t_simulation *sim, t_coder *coder)
{
	if (!take_both_dongles(sim, coder))
		return (false);
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	thread_print(coder, "is compiling");
	usleep(coder->sim->time_to_compile * 1000);
	pthread_mutex_lock(&coder->mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->mutex);
	release_both_dongles(coder);
	return (true);
}

void	*coder_thread(void *arg)
{
	t_coder			*coder;
	t_simulation	*sim;

	coder = (t_coder *) arg;
	sim = coder->sim;
	while (!should_stop_now(sim))
	{
		if (!coder_compile(sim, coder))
			break ;
		if (should_stop_now(sim))
			break ;
		thread_print(coder, "is debugging");
		usleep(coder->sim->time_to_debug * 1000);
		if (should_stop_now(sim))
			break ;
		thread_print(coder, "is refactoring");
		usleep(coder->sim->time_to_refactor * 1000);
	}
	return (NULL);
}

bool	check_coders(t_simulation *sim)
{
	int		i;
	t_coder	*coder;
	bool	all_finished;

	i = 0;
	all_finished = true;
	while (i < sim->number_of_coders)
	{
		coder = &sim->coders[i];
		if (is_burned_out(coder, sim))
		{
			pthread_mutex_lock(&sim->stop_mutex);
			sim->stop = true;
			pthread_mutex_unlock(&sim->stop_mutex);
			thread_print(coder, "burned out");
			return (true);
		}
		if (!is_finished(sim, coder))
			all_finished = false;
		i++;
	}
	return (all_finished);
}

void	*mon_thread(void *arg)
{
	t_simulation	*sim;
	int				i;

	sim = (t_simulation *)arg;
	i = 0;
	while (1)
	{
		if (check_coders(sim))
		{
			pthread_mutex_lock(&sim->stop_mutex);
			sim->stop = true;
			pthread_mutex_unlock(&sim->stop_mutex);
			return (NULL);
		}
	}
	usleep(1000);
}

void	thread_print(t_coder *coder, char *string)
{
	pthread_mutex_lock(&coder->sim->log_mutex);
	printf("%lld %d %s\n", get_time_ms() - coder->sim->start_time,
		coder->id, string);
	pthread_mutex_unlock(&coder->sim->log_mutex);
}
