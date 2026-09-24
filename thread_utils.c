/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 12:18:43 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/24 17:01:36 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	check_coders(t_simulation *sim)
{
	int		i;
	t_coder	*coder;
	bool	all_finished;

	i = 0;
	all_finished = true;
	while (i < sim->number_of_coders)
	{
		coder = &sim->coders[i];
		if (is_burned_out(coder, sim))
		{
			pthread_mutex_lock(&sim->stop_mutex);
			sim->stop = true;
			pthread_mutex_unlock(&sim->stop_mutex);
			thread_print(coder, "burned out");
			return (true);
		}
		if (!is_finished(sim, coder))
			all_finished = false;
		i++;
	}
	return (all_finished);
}

bool	coder_compile(t_simulation *sim, t_coder *coder)
{
	if (!take_both_dongles(sim, coder))
		return (false);
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	thread_print(coder, "is compiling");
	usleep(coder->sim->time_to_compile * 1000);
	pthread_mutex_lock(&coder->mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->mutex);
	release_both_dongles(coder);
	return (true);
}

void	ft_swap(t_coder **coder_a, t_coder **coder_b)
{
	t_coder	*temp;

	temp = *coder_a;
	*coder_a = *coder_b;
	*coder_b = temp;
}
