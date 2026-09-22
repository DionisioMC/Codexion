/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 15:57:44 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/22 11:35:48 by dcoelho          ###   ########.fr       */
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
