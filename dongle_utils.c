/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 15:57:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/24 12:19:38 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	is_burned_out(t_coder *coder, t_simulation *sim)
{
	long long	last_compile;
	long long	time_since_compile;

	pthread_mutex_lock(&coder->mutex);
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&coder->mutex);
	time_since_compile = get_time_ms() - last_compile;
	return (time_since_compile >= sim->time_to_burnout);
}

bool	is_finished(t_simulation *sim, t_coder *coder)
{
	int	compiled;

	pthread_mutex_lock(&coder->mutex);
	compiled = coder->compile_count;
	pthread_mutex_unlock(&coder->mutex);
	return (compiled >= sim->number_of_compiles_required);
}

void	get_request_number(t_simulation *sim, t_coder *coder)
{
	pthread_mutex_lock(&sim->request_mutex);
	sim->request_count++;
	coder->request_number = sim->request_count;
	pthread_mutex_unlock(&sim->request_mutex);
}

bool	should_stop_now(t_simulation *sim)
{
	bool	result;

	pthread_mutex_lock(&sim->stop_mutex);
	result = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (result);
}

int	gen_dongles(t_simulation *sim)
{
	int			i;
	t_dongle	*dongle;

	i = 0;
	sim->dongles = (t_dongle *) malloc((sizeof(t_dongle)
				* sim->number_of_coders));
	if (!sim->dongles)
		return (1);
	while (i < sim->number_of_coders)
	{
		dongle = &sim->dongles[i];
		dongle->id = i + 1;
		dongle->busy = false;
		dongle->release_timestamp = 0;
		if (init_dongle_heap(dongle, sim->number_of_coders) == 1
			|| pthread_mutex_init(&dongle->mutex, NULL) != 0
			|| pthread_cond_init(&dongle->wake_cond, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}
