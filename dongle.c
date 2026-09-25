/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:38:23 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/25 12:00:01 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongle(t_simulation *sim, t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;
	long long		remaining_cooldown;

	pthread_mutex_lock(&dongle->mutex);
	coder->priority_ts = compute_deadline(coder);
	get_request_number(sim, coder);
	remaining_cooldown = get_time_ms() - dongle->release_ts;
	min_heap_push(sim, &dongle->heap, coder);
	while ((dongle->busy || remaining_cooldown < sim->dongle_cooldown
			|| dongle->heap.data[0] != coder) && !should_stop_now(sim))
	{
		ts.tv_sec = dongle->release_ts / 1000;
		ts.tv_nsec = (dongle->release_ts % 1000) * 1000000;
		pthread_cond_timedwait(&dongle->wake_cond, &dongle->mutex, &ts);
		remaining_cooldown = get_time_ms() - dongle->release_ts;
	}
	if (should_stop_now(sim))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	dongle->busy = true;
	thread_print(coder, "has taken a dongle");
	min_heap_pop(&dongle->heap, sim);
	pthread_mutex_unlock(&dongle->mutex);
}

bool	take_both_dongles(t_simulation *sim, t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->l_dongle == coder->r_dongle)
	{
		take_dongle(sim, coder->l_dongle, coder);
		while (!should_stop_now(sim))
			usleep(1000);
		return (false);
	}
	if (coder->id % 2 == 0)
	{
		first = coder->l_dongle;
		second = coder->r_dongle;
	}
	else
	{
		first = coder->r_dongle;
		second = coder->l_dongle;
	}
	take_dongle(sim, first, coder);
	if (should_stop_now(sim))
		return (false);
	take_dongle(sim, second, coder);
	return (!should_stop_now(sim));
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->busy = false;
	dongle->release_ts = get_time_ms();
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
		fprintf(stderr, "Error: Dongle heap failed to initialize .\n");
		return (1);
	}
	dongle->heap.size = 0;
	return (0);
}
