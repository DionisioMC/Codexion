/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 15:57:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/18 16:21:13 by dcoelho          ###   ########.fr       */
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

void	queue_add(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->request_ts = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->queue[0] == NULL && dongle->queue[1] == NULL)
		dongle->queue[0] = coder;
	else if (dongle->queue[0] != NULL && dongle->queue[1] == NULL
		&& coder != dongle->queue[0])
		dongle->queue[1] = coder;
	pthread_mutex_unlock(&dongle->mutex);
}

int	occupy_dongle(t_dongle *dongle, t_coder *coder)
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
		return (0);
	}
	queue_add(dongle, coder);
	pthread_mutex_lock(&dongle->mutex);
	while ((dongle->busy || get_time_ms() <= dongle->active_timestamp)
		&& !is_next(dongle, coder, coder->sim))
	{
		pthread_mutex_lock(&coder->sim->stop_mutex);
		if (coder->sim->stop)
		{
			pthread_mutex_unlock(&coder->sim->stop_mutex);
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		pthread_mutex_unlock(&coder->sim->stop_mutex);
		ts.tv_sec = dongle->active_timestamp / 1000;
		ts.tv_nsec = (dongle->active_timestamp % 1000) * 1000000;
		pthread_cond_timedwait(&dongle->wake_cond, &dongle->mutex, &ts);
	}
	dongle->busy = 1;
	pthread_mutex_unlock(&dongle->mutex);
	thread_print(coder, "has taken a dongle");
	return (1);
}

void	release_dongle(t_dongle *dongle, t_coder *coder)
{
	queue_remove(dongle, coder);
	pthread_mutex_lock(&dongle->mutex);
	dongle->busy = 0;
	dongle->active_timestamp = get_time_ms() + dongle->cooldown;
	printf("%lld\n", dongle->active_timestamp - coder->sim->start_time);
	pthread_cond_broadcast(&dongle->wake_cond);
	pthread_mutex_unlock(&dongle->mutex);
}
