/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 12:07:29 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/26 12:26:03 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_thread(void *coder)
{
	t_coder	*coder_original;

	coder_original = (t_coder *) coder;
	while (coder_original->compile_count
		< coder_original->sim->number_of_compiles_required)
	{
		if (coder_original->task == COMPILE)
		{
			printf("%ld %d is compiling\n",
				get_time_ms() - coder_original->sim->start_time,
				coder_original->number);
			coder_original->last_compile_start = get_time_ms();
			coder_original->compile_count++;
			coder_original->task = DEBUG;
		}
		else if (coder_original->task == DEBUG)
		{
			printf("%ld %d is debbuging\n",
				get_time_ms() - coder_original->sim->start_time,
				coder_original->number);
			coder_original->task = REFACTOR;
		}
		else if (coder_original->task == REFACTOR)
		{
			printf("%ld %d is refactoring\n",
				get_time_ms() - coder_original->sim->start_time,
				coder_original->number);
			coder_original->task = COMPILE;
		}
	}
	return (NULL);
}

void	gen_coder_threads(t_coder *coders, t_simulation *sim)
{
	int	i;

	i = 0;
	sim->start_time = get_time_ms();
	while (i < sim->number_of_coders)
	{
		coders[i].thread = (pthread_t *)malloc(sizeof(pthread_t));
		if (!coders[i].thread)
			thread_error(sim, coders, i);
		pthread_create(coders[i].thread, NULL, coder_thread, &coders[i]);
		i++;
	}
}

int	is_burned_out(t_coder *coders, t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (get_time_ms() - coders[i].last_compile_start
			>= sim->time_to_burnout)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

int	is_everyone_finished(t_coder *coders, t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (coders[i].compile_count < sim->number_of_compiles_required)
		{
			return (0);
		}
		i++;
	}
	return (1);
}

void	*mon_thread(void *coders)
{
	t_coder			*coders_original;
	t_simulation	*sim;

	coders_original = (t_coder *) coders;
	sim = coders_original[0].sim;
	while (!is_burned_out(coders_original, sim)
		|| !is_everyone_finished(coders, sim))
	{
		printf("Monitoring...\n");
		usleep(10);
	}
	return (NULL);
}

void	launch_mon_thread(pthread_t *thread, t_coder *coders)
{
	pthread_create(thread, NULL, mon_thread, coders);
}
