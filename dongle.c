/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:38:23 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/15 15:25:41 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	queue_remove(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->queue[0] == coder && dongle->queue[1] == NULL)
		dongle->queue[0] = NULL;
	else if (dongle->queue[1] == coder && dongle->queue[0] != NULL)
		dongle->queue[1] = NULL;
	pthread_mutex_unlock(&dongle->mutex);
}

int	higher_priority(t_simulation *sim, t_coder *coder_a,
	t_coder *coder_b)
{
	if (!coder_b || coder_b == coder_a)
		return (1);
	if (strcmp(sim->scheduler, "edf") == 0)
		return (compute_deadline(coder_a) < compute_deadline(coder_b));
	return (coder_a->request_ts < coder_b->request_ts);
}

int	is_next(t_dongle *dongle, t_coder *coder, t_simulation *sim)
{
	t_coder	*next_coder;

	printf("%d\n", coder->number);
	pthread_mutex_lock(&dongle->mutex);
	next_coder = dongle->queue[0];
	if (next_coder == coder)
		next_coder = dongle->queue[1];
	pthread_mutex_unlock(&dongle->mutex);
	return (higher_priority(sim, coder, next_coder));
}

int	acquire_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->l_dongle;
	right = coder->r_dongle;
	if (coder->number % 2 != 0)
	{
		if (!occupy_dongle(left, coder))
			return (0);
		if (occupy_dongle(right, coder))
			return (queue_remove(left, coder), 0);
	}
	else
	{
		if (!occupy_dongle(right, coder))
			return (0);
		if (occupy_dongle(right, coder))
			return (queue_remove(right, coder), 0);
	}
	return (1);
}

void	release_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->l_dongle;
	right = coder->r_dongle;
	pthread_mutex_lock(&left->mutex);
	pthread_mutex_lock(&right->mutex);
	left->busy = 0;
	right->busy = 0;
	pthread_mutex_unlock(&right->mutex);
	pthread_mutex_unlock(&left->mutex);
}
