/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utilities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 15:57:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/10 16:19:19 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*low_dongle(t_coder *coder)
{
	if (coder->l_dongle->id < coder->r_dongle->id)
		return (coder->l_dongle);
	return (coder->r_dongle);
}

t_dongle	*high_dongle(t_coder *coder)
{
	if (coder->l_dongle->id < coder->r_dongle->id)
		return (coder->r_dongle);
	return (coder->l_dongle);
}

long long	compute_deadline(t_coder *coder)
{
	if (coder->last_compile_start == 0)
		return (coder->sim->start_time + coder->sim->time_to_burnout);
	return (coder->last_compile_start + coder->sim->time_to_burnout);
}

int	dongle_is_free(t_coder *coder)
{
	return (!coder->l_dongle->busy && !coder->r_dongle->busy);
}

void	queue_add(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->queue[0] == NULL)
		dongle->queue[0] = coder;
	else
		dongle->queue[1] = coder;
	pthread_mutex_unlock(&dongle->mutex);
}
