/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:36:56 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/15 16:50:34 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_coders(t_coder *coders, t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (coders[i].r_dongle)
		{
			pthread_mutex_destroy(&coders[i].mutex);
			free(coders[i].r_dongle);
			coders[i].r_dongle = NULL;
			free(coders[i].thread);
			coders[i].thread = NULL;
			pthread_mutex_destroy(&coders[i].mutex);
		}
		i++;
	}
	free(coders);
}

t_dongle	*gen_dongle(int i, t_simulation *sim, t_coder *coders,
	pthread_t *monitoring_thread)
{
	t_dongle	*dongle;
	int			j;

	dongle = (t_dongle *) malloc(sizeof(t_dongle));
	j = 0;
	if (dongle)
	{
		dongle->id = i + 1;
		dongle->cooldown = sim->dongle_cooldown;
		pthread_mutex_init(&dongle->mutex, NULL);
		return (dongle);
	}
	else
	{
		while (j < i - 1)
		{
			if (coders[j].r_dongle)
			{
				pthread_mutex_destroy(&coders[j].mutex);
				free(coders[j].r_dongle);
				coders[j].r_dongle = NULL;
				free(coders[j].thread);
				coders[j].thread = NULL;
			}
			j++;
		}
		free(coders);
		free(sim);
		free(monitoring_thread);
		exit(1);
	}
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	gen_coders_and_dongles(t_coder *coders, t_simulation *sim,
	pthread_t *monitoring_thread)
{
	int				i;
	t_coder			coder;

	i = 0;
	while (i < sim->number_of_coders)
	{
		coder.number = i + 1;
		coder.compile_count = 0;
		coder.l_dongle = NULL;
		coder.r_dongle = gen_dongle(i, sim, coders, monitoring_thread);
		coder.task = COMPILE;
		coder.last_compile_start = 0;
		coder.sim = sim;
		if (i > 0 && i != (sim->number_of_coders - 1))
			coder.l_dongle = coders[i - 1].r_dongle;
		else if (i > 0)
		{
			coder.l_dongle = coders[i - 1].r_dongle;
			coders[0].l_dongle = coder.r_dongle;
		}
		coders[i] = coder;
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_coder			*coders;
	t_simulation	*sim;
	pthread_t		*monitoring_thread;
	int				i;

	i = 0;
	sim = parser(argc, argv);
	sim->start_time = get_time_ms();
	coders = (t_coder *)malloc(sizeof(t_coder) * sim->number_of_coders);
	monitoring_thread = (pthread_t *)malloc(sizeof(pthread_t));
	if (coders && monitoring_thread)
		gen_simulation(coders, sim, monitoring_thread);
	else
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		pthread_mutex_destroy(&sim->log_mutex);
		pthread_mutex_destroy(&sim->wake_mutex);
		pthread_cond_destroy(&sim->wake_cond);
		free(monitoring_thread);
		free_coders(coders, sim);
		free(sim);
		exit(1);
	}
}
