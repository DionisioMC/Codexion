/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 11:26:50 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/25 12:23:36 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_swap(t_coder **coder_a, t_coder **coder_b)
{
	t_coder	*temp;

	temp = *coder_a;
	*coder_a = *coder_b;
	*coder_b = temp;
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long long	priority_tie_breaker(t_simulation *sim, t_coder *coder)
{
	if (strcmp(sim->scheduler, "edf") == 0)
		return (coder->id);
	return (coder->request_number);
}

long long	compute_deadline(t_coder *coder)
{
	long long	deadline;

	if (strcmp(coder->sim->scheduler, "edf") == 0)
	{
		pthread_mutex_lock(&coder->mutex);
		deadline = coder->last_compile_start + coder->sim->time_to_burnout;
		if (coder->last_compile_start == 0)
			deadline = coder->sim->start_time + coder->sim->time_to_burnout;
		pthread_mutex_unlock(&coder->mutex);
	}
	else
		deadline = get_time_ms();
	return (deadline);
}

bool	has_higher_priority(t_simulation *sim, t_coder *coder_a,
	t_coder *coder_b)
{
	if (coder_a->priority_ts < coder_b->priority_ts)
		return (true);
	if (coder_a->priority_ts == coder_b->priority_ts)
		return (priority_tie_breaker(sim, coder_a)
			< priority_tie_breaker(sim, coder_b));
	return (false);
}
