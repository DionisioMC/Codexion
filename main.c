/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:36:56 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/26 12:20:46 by dcoelho          ###   ########.fr       */
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
			free(coders[i].r_dongle);
			free(coders[i].thread);
			coders[i].r_dongle = NULL;
			coders[i].thread = NULL;
		}
		i++;
	}
	free(coders);
}

t_dongle	*gen_dongle(int i, t_simulation *sim, t_coder *coders)
{
	t_dongle	*dongle;

	dongle = (t_dongle *) malloc(sizeof(t_dongle));
	if (dongle)
	{
		dongle->id = i + 1;
		dongle->cooldown = sim->dongle_cooldown;
		return (dongle);
	}
	else
	{
		free_coders(coders, sim);
		free(sim);
		exit(1);
	}
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	gen_coders_and_dongles(t_coder *coders, t_simulation *sim)
{
	int				i;
	t_coder			coder;

	i = 0;
	while (i < sim->number_of_coders)
	{
		coder.number = i + 1;
		coder.compile_count = 0;
		coder.l_dongle = NULL;
		coder.r_dongle = gen_dongle(i, sim, coders);
		coder.task = COMPILE;
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

void	print_coders(t_coder *coders, int number_of_coders)
{
	int	i;

	i = 0;
	while (i < number_of_coders)
	{
		printf("Coder %d:\n", coders[i].number);
		printf("  last_compile_start: %ld\n", coders[i].last_compile_start);
		printf("  compile_count: %d\n", coders[i].compile_count);
		printf("  l_dongle: %p\n", (void *)coders[i].l_dongle);
		printf("  r_dongle: %p\n", (void *)coders[i].r_dongle);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_coder			*coders;
	t_simulation	*sim;
	pthread_t		*mon_thread;
	int				i;

	i = 0;
	sim = parser(argc, argv);
	coders = (t_coder *)malloc(sizeof(t_coder) * sim->number_of_coders);
	mon_thread = (pthread_t *)malloc(sizeof(pthread_t));
	if (coders)
	{
		gen_coders_and_dongles(coders, sim);
		gen_coder_threads(coders, sim);
		launch_mon_thread(mon_thread, coders);
		pthread_join(*mon_thread, NULL);
		free(mon_thread);
		free_coders(coders, sim);
		free(sim);
	}
	else
	{
		free(coders);
		free(sim);
		exit(1);
	}
}
