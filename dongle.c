/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:38:23 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/11 16:16:51 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	queue_remove(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue[0] == coder)
		dongle->queue[0] = NULL;
	else if (dongle->queue[1] == coder)
		dongle->queue[1] = NULL;
}

static int	higher_priority(t_simulation *sim, t_coder *coder_a,
	t_coder *coder_b)
{
	if (!coder_b || coder_b == coder_a)
		return (1);
	if (strcmp(sim->scheduler, "edf") == 0)
		return (compute_deadline(coder_a) < compute_deadline(coder_b));
	return (coder_a->request_ts < coder_b->request_ts);
}

static int	is_next(t_dongle *dongle, t_coder *coder, t_simulation *sim)
{
	t_coder	*next_coder;

	next_coder = dongle->queue[0];
	if (next_coder == coder)
		next_coder = dongle->queue[1];
	return (higher_priority(sim, coder, next_coder));
}

int	acquire_dongles(t_coder *coder)
{
	t_dongle	*lo;
	t_dongle	*hi;
	int			got;
	int			stop;

	lo = low_dongle(coder);
	hi = high_dongle(coder);
	coder->request_ts = get_time_ms();
	queue_add(lo, coder);
	queue_add(hi, coder);
	got = 0;
	while (!got)
	{
		pthread_mutex_lock(&coder->sim->stop_mutex);
		stop = coder->sim->stop;
		pthread_mutex_unlock(&coder->sim->stop_mutex);
		if (stop)
		{
			queue_remove(lo, coder);
			queue_remove(hi, coder);
			return (0);
		}
		pthread_mutex_lock(&lo->mutex);
		pthread_mutex_lock(&hi->mutex);
		if (!lo->busy && !hi->busy
			&& is_next(lo, coder, coder->sim) && is_next(hi, coder, coder->sim))
		{
			lo->busy = 1;
			hi->busy = 1;
			queue_remove(lo, coder);
			queue_remove(hi, coder);
			got = 1;
		}
		pthread_mutex_unlock(&hi->mutex);
		pthread_mutex_unlock(&lo->mutex);
		if (!got)
		{
			pthread_mutex_lock(&coder->sim->wake_mutex);
			pthread_cond_wait(&coder->sim->wake_cond, &coder->sim->wake_mutex);
			pthread_mutex_unlock(&coder->sim->wake_mutex);
		}
	}
	return (1);
}

void	release_dongles(t_coder *coder)
{
	t_dongle	*lo;
	t_dongle	*hi;

	lo = low_dongle(coder);
	hi = high_dongle(coder);
	pthread_mutex_lock(&lo->mutex);
	lo->busy = 0;
	pthread_mutex_unlock(&lo->mutex);
	pthread_mutex_lock(&hi->mutex);
	hi->busy = 0;
	pthread_mutex_unlock(&hi->mutex);
	pthread_mutex_lock(&coder->sim->wake_mutex);
	pthread_cond_broadcast(&coder->sim->wake_cond);
	pthread_mutex_unlock(&coder->sim->wake_mutex);
}
