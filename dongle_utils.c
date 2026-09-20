/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 15:57:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/20 23:26:23 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	compute_deadline(t_coder *coder)
{
	long long	deadline;

	pthread_mutex_lock(&coder->mutex);
	deadline = coder->last_compile_start + coder->sim->time_to_burnout;
	if (coder->last_compile_start == 0)
		deadline = coder->sim->start_time + coder->sim->time_to_burnout;
	pthread_mutex_unlock(&coder->mutex);
	return (deadline);
}

bool	occupy_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;
	int				stop;

	if (!dongle)
	{
		pthread_mutex_lock(&coder->sim->stop_mutex);
		stop = coder->sim->stop;
		pthread_mutex_unlock(&coder->sim->stop_mutex);
		while (!stop)
		{
			pthread_mutex_lock(&coder->sim->stop_mutex);
			stop = coder->sim->stop;
			pthread_mutex_unlock(&coder->sim->stop_mutex);
		}
		return (false);
	}
	queue_add(dongle, coder);
	pthread_mutex_lock(&dongle->mutex);
	while (dongle->busy || get_time_ms() <= dongle->active_timestamp
		|| !is_next(dongle, coder, coder->sim))
	{
		pthread_mutex_lock(&coder->sim->stop_mutex);
		if (coder->sim->stop)
		{
			pthread_mutex_unlock(&coder->sim->stop_mutex);
			pthread_mutex_unlock(&dongle->mutex);
			return (false);
		}
		pthread_mutex_unlock(&coder->sim->stop_mutex);
		ts.tv_sec = dongle->active_timestamp / 1000;
		ts.tv_nsec = (dongle->active_timestamp % 1000) * 1000000;
		pthread_cond_timedwait(&dongle->wake_cond, &dongle->mutex, &ts);
	}
	dongle->busy = true;
	pthread_mutex_unlock(&dongle->mutex);
	thread_print(coder, "has taken a dongle");
	return (true);
}

void	release_dongle(t_dongle *dongle, t_coder *coder)
{
	queue_remove(dongle, coder);
	pthread_mutex_lock(&dongle->mutex);
	dongle->busy = 0;
	dongle->active_timestamp = get_time_ms() + dongle->cooldown;
	pthread_cond_broadcast(&dongle->wake_cond);
	pthread_mutex_unlock(&dongle->mutex);
}
