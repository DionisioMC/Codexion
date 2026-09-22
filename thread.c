/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 12:07:29 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/22 17:06:22 by dcoelho          ###   ########.fr       */
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
