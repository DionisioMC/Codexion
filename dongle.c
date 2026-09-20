/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:38:23 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/20 23:29:59 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	higher_priority(t_simulation *sim, t_coder *coder_a,
	t_coder *coder_b)
{
	long long	deadline_a;
	long long	deadline_b;

	if (!coder_b)
		return (true);
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

bool	acquire_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->l_dongle;
	right = coder->r_dongle;
	if (coder->number % 2 != 0 && left)
	{
		if (!occupy_dongle(left, coder))
			return (false);
		if (!occupy_dongle(right, coder))
			return (release_dongles(coder), false);
	}
	else
	{
		if (!occupy_dongle(right, coder))
			return (false);
		if (!occupy_dongle(left, coder))
			return (release_dongles(coder), false);
	}
	return (true);
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
