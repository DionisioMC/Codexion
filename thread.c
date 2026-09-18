/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 12:07:29 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/18 16:18:42 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_thread(void *coder)
{
	t_coder	*coder_original;

	coder_original = (t_coder *) coder;
	while (1)
	{
		pthread_mutex_lock(&coder_original->sim->stop_mutex);
		if (coder_original->sim->stop)
		{
			pthread_mutex_unlock(&coder_original->sim->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&coder_original->sim->stop_mutex);
		if (coder_original->task == COMPILE)
			coder_compile(coder_original);
		else if (coder_original->task == DEBUG)
			coder_debug(coder_original);
		else
			coder_refactor(coder_original);
	}
	return (NULL);
}

void	gen_coder_threads(t_coder *coders, t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		coders[i].thread = (pthread_t *)malloc(sizeof(pthread_t));
		if (!coders[i].thread)
			thread_error(sim, coders, i);
		pthread_create(coders[i].thread, NULL, coder_thread, &coders[i]);
		i++;
	}
}

void	*mon_thread(void *coders)
{
	t_coder			*coders_original;
	t_simulation	*sim;
	int				burned_out;
	int				i;

	coders_original = (t_coder *) coders;
	sim = coders_original[0].sim;
	i = 0;
	while (is_burned_out(coders_original, sim) < 0
		&& !is_everyone_finished(coders_original, sim))
	{
		usleep(1000);
	}
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	burned_out = is_burned_out(coders_original, sim);
	if (burned_out >= 0)
	{
		thread_print(&coders_original[burned_out], "burned out");
	}
	while (i < sim->number_of_coders)
	{
		pthread_cond_broadcast(&coders_original[i].r_dongle->wake_cond);
		i++;
	}
	return (NULL);
}

void	gen_simulation(t_coder *coders, t_simulation *sim,
	pthread_t *monitoring_thread)
{
	int	i;

	i = 0;
	gen_coders_and_dongles(coders, sim, monitoring_thread);
	pthread_create(monitoring_thread, NULL, mon_thread, coders);
	gen_coder_threads(coders, sim);
	pthread_join(*monitoring_thread, NULL);
	while (i < sim->number_of_coders)
	{
		pthread_join(*coders[i].thread, NULL);
		i++;
	}
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->log_mutex);
	free(monitoring_thread);
	free_coders(coders, sim);
	free(sim);
}

void	thread_print(t_coder *coder, char *string)
{
	pthread_mutex_lock(&coder->sim->log_mutex);
	printf("%lld %d %s\n", get_time_ms() - coder->sim->start_time,
		coder->number, string);
	pthread_mutex_unlock(&coder->sim->log_mutex);
}
