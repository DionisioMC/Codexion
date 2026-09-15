/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utilities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 15:57:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/15 16:12:26 by dcoelho          ###   ########.fr       */
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
	else if (dongle->queue[0] != NULL && dongle->queue[1] == NULL)
		dongle->queue[1] = coder;
	pthread_mutex_unlock(&dongle->mutex);
}

int	occupy_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;

	queue_add(dongle, coder);
	pthread_mutex_lock(&coder->sim->stop_mutex);
	if (coder->sim->stop)
	{
		pthread_mutex_unlock(&coder->sim->stop_mutex);
		queue_remove(dongle, coder);
		return (0);
	}
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	pthread_mutex_lock(&dongle->mutex);
	while (dongle->busy && !is_next(dongle, coder, coder->sim))
	{
		get_cooldown_timestamp(&ts, dongle);
		pthread_cond_timedwait(&coder->sim->wake_cond, &dongle->mutex, &ts);
	}
	dongle->busy = 1;
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

void	get_cooldown_timestamp(struct timespec *ts, t_dongle *dongle)
{
	long long	total_ms;

	total_ms = get_time_ms() + dongle->cooldown;
	ts->tv_sec = total_ms / 1000;
	ts->tv_nsec = (total_ms % 1000) * 1000000;
}