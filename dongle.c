/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:38:23 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/22 12:40:58 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	take_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;
	long long		remaining_cooldown;

	pthread_mutex_lock(&dongle->mutex);
	coder->priority_ts = compute_deadline(coder);
	get_request_number(coder->sim, coder);
	remaining_cooldown = get_time_ms() - dongle->release_timestamp;
	min_heap_push(coder->sim, &dongle->heap, coder);
	while ((dongle->busy
			|| remaining_cooldown < coder->sim->dongle_cooldown
			|| dongle->heap.data[0] != coder) && should_stop_now(coder->sim))
	{
		ts.tv_sec = dongle->release_timestamp / 1000;
		ts.tv_nsec = (dongle->release_timestamp % 1000) * 1000000;
		pthread_cond_timedwait(&dongle->wake_cond, &dongle->mutex, &ts);
		remaining_cooldown = get_time_ms() - dongle->release_timestamp;
	}
	if (should_stop_now(coder->sim))
		return (pthread_mutex_unlock(&dongle->mutex), false);
	dongle->busy = true;
	pthread_mutex_unlock(&dongle->mutex);
	thread_print(coder, "has taken a dongle");
	min_heap_push(coder->sim, &dongle->heap, coder);
	return (true);
}

bool	take_both_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->l_dongle;
	right = coder->r_dongle;
	if (coder->id % 2 != 0 && left)
	{
		if (!take_dongle(left, coder))
			return (false);
		if (!take_dongle(right, coder))
			return (release_both_dongles(coder), false);
	}
	else
	{
		if (!take_dongle(right, coder))
			return (false);
		if (!take_dongle(left, coder))
			return (release_both_dongles(coder), false);
	}
	return (true);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->busy = false;
	dongle->release_timestamp = get_time_ms();
	pthread_cond_broadcast(&dongle->wake_cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	release_both_dongles(t_coder *coder)
{
	t_dongle		*left;
	t_dongle		*right;

	left = coder->l_dongle;
	right = coder->r_dongle;
	release_dongle(right);
	if (left)
		release_dongle(left);
}

int	init_dongle_heap(t_dongle *dongle, int capacity)
{
	dongle->heap.data = malloc(sizeof(t_coder *) * capacity);
	if (!dongle->heap.data)
	{
		fprintf(stderr, "ERROR: Failed to initialize dongle heap.\n");
		return (1);
	}
	dongle->heap.size = 0;
	return (0);
}
