/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:38:23 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/18 16:21:18 by dcoelho          ###   ########.fr       */
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
	else if (dongle->queue[0] == coder && dongle->queue[1] != NULL)
	{
		dongle->queue[0] = dongle->queue[1];
		dongle->queue[1] = NULL;
	}
	pthread_mutex_unlock(&dongle->mutex);
}

int	higher_priority(t_simulation *sim, t_coder *coder_a,
	t_coder *coder_b)
{
	long long	deadline_a;
	long long	deadline_b;

	if (!coder_b)
		return (1);
	if (strcmp(sim->scheduler, "edf") == 0)
	{
		deadline_a = compute_deadline(coder_a);
		deadline_b = compute_deadline(coder_b);
		if (deadline_a == deadline_b)
			return (coder_a->request_ts < coder_b->request_ts);
		else
			return (compute_deadline(coder_a) < compute_deadline(coder_b));
	}
	return (coder_a->request_ts < coder_b->request_ts);
}

int	is_next(t_dongle *dongle, t_coder *coder, t_simulation *sim)
{
	t_coder	*next_coder;

	if (!dongle)
		return (0);
	next_coder = dongle->queue[0];
	if (next_coder == coder)
		next_coder = dongle->queue[1];
	return (higher_priority(sim, coder, next_coder));
}

int	acquire_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->l_dongle;
	right = coder->r_dongle;
	if (coder->number % 2 != 0 && left)
	{
		if (!occupy_dongle(left, coder))
			return (0);
		if (!occupy_dongle(right, coder))
			return (release_dongles(coder), 0);
	}
	else
	{
		if (!occupy_dongle(right, coder))
			return (0);
		if (!occupy_dongle(left, coder))
			return (release_dongles(coder), 0);
	}
	return (1);
}

void	release_dongles(t_coder *coder)
{
	t_dongle		*left;
	t_dongle		*right;

	left = coder->l_dongle;
	right = coder->r_dongle;
	if (left)
		release_dongle(left, coder);
	release_dongle(right, coder);
}
