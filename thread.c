/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 12:07:29 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/24 14:11:11 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
