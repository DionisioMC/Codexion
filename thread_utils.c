/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 12:18:43 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/22 11:45:43 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
			thread_print(coder, "is burned out");
			return (true);
		}
		if (!is_finished(sim, coder))
			all_finished = false;
		i++;
	}
	return (all_finished);
}

void	coder_compile(t_coder *coder)
{
	if (!take_both_dongles(coder))
		return ;
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_lock(&coder->sim->stop_mutex);
	if (!coder->sim->stop)
		thread_print(coder, "is compiling");
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	usleep(coder->sim->time_to_compile * 1000);
	release_both_dongles(coder);
	pthread_mutex_lock(&coder->mutex);
	coder->compile_count++;
	coder->task = DEBUG;
	pthread_mutex_unlock(&coder->mutex);
}

void	coder_debug(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->stop_mutex);
	if (!coder->sim->stop)
		thread_print(coder, "is debugging");
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	usleep(coder->sim->time_to_debug * 1000);
	pthread_mutex_lock(&coder->mutex);
	coder->task = REFACTOR;
	pthread_mutex_unlock(&coder->mutex);
}

void	coder_refactor(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->stop_mutex);
	if (!coder->sim->stop)
		thread_print(coder, "is refactoring");
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	usleep(coder->sim->time_to_refactor * 1000);
	pthread_mutex_lock(&coder->mutex);
	coder->task = COMPILE;
	pthread_mutex_unlock(&coder->mutex);
}
