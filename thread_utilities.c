/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utilities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 12:18:43 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/15 12:15:52 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_burned_out(t_coder *coders, t_simulation *sim)
{
	int			i;
	long long	deadline;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&coders[i].mutex);
		if (coders[i].last_compile_start == 0)
			deadline = get_time_ms() - sim->start_time;
		else
			deadline = get_time_ms() - coders[i].last_compile_start;
		pthread_mutex_unlock(&coders[i].mutex);
		if (deadline >= sim->time_to_burnout)
			return (i);
		i++;
	}
	return (-1);
}

int	is_everyone_finished(t_coder *coders, t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&coders[i].mutex);
		if (coders[i].compile_count < sim->number_of_compiles_required)
		{
			pthread_mutex_unlock(&coders[i].mutex);
			return (0);
		}
		pthread_mutex_unlock(&coders[i].mutex);
		i++;
	}
	return (1);
}

void	coder_compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	if (!acquire_dongles(coder))
		return ;
	pthread_mutex_lock(&coder->sim->log_mutex);
	pthread_mutex_lock(&coder->sim->stop_mutex);
	if (!coder->sim->stop)
		printf("%lld %d is compiling\n",
			get_time_ms() - coder->sim->start_time, coder->number);
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	pthread_mutex_unlock(&coder->sim->log_mutex);
	usleep(coder->sim->time_to_compile * 1000);
	release_dongles(coder);
	pthread_mutex_lock(&coder->mutex);
	coder->compile_count++;
	coder->task = DEBUG;
	pthread_mutex_unlock(&coder->mutex);
}

void	coder_debug(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_lock(&coder->sim->log_mutex);
	pthread_mutex_lock(&coder->sim->stop_mutex);
	if (!coder->sim->stop)
	{
		pthread_mutex_unlock(&coder->sim->stop_mutex);
		printf("%lld %d is debugging\n",
			get_time_ms() - coder->sim->start_time, coder->number);
	}
	else
		pthread_mutex_unlock(&coder->sim->stop_mutex);
	pthread_mutex_unlock(&coder->sim->log_mutex);
	usleep(coder->sim->time_to_debug * 1000);
	pthread_mutex_lock(&coder->mutex);
	coder->task = REFACTOR;
	pthread_mutex_unlock(&coder->mutex);
}

void	coder_refactor(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_lock(&coder->sim->log_mutex);
	pthread_mutex_lock(&coder->sim->stop_mutex);
	if (!coder->sim->stop)
	{
		pthread_mutex_unlock(&coder->sim->stop_mutex);
		printf("%lld %d is refactoring\n",
			get_time_ms() - coder->sim->start_time, coder->number);
	}
	else
		pthread_mutex_unlock(&coder->sim->stop_mutex);
	pthread_mutex_unlock(&coder->sim->log_mutex);
	usleep(coder->sim->time_to_refactor * 1000);
	pthread_mutex_lock(&coder->mutex);
	coder->task = COMPILE;
	pthread_mutex_unlock(&coder->mutex);
}
