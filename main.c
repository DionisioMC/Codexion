/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 11:36:56 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/24 16:58:53 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	gen_coders(t_simulation *sim)
{
	int		i;
	t_coder	*coder;

	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!sim->coders)
	{
		fprintf(stderr, "Error: Failed to initialize coders.\n");
		return (1);
	}
	i = 0;
	while (i < (sim->number_of_coders))
	{
		coder = &sim->coders[i];
		coder->id = i + 1;
		coder->compile_count = 0;
		coder->last_compile_start = sim->start_time;
		coder->sim = sim;
		coder->l_dongle = &sim->dongles[i];
		coder->r_dongle = &sim->dongles[(i + 1) % sim->number_of_coders];
		if (pthread_mutex_init(&coder->mutex, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	create_threads(t_simulation *sim, pthread_t *monitor_thread)
{
	int	i;

	i = 0;
	pthread_create(monitor_thread, NULL, mon_thread, sim);
	usleep(1000);
	while (i < sim->number_of_coders)
	{
		pthread_create(&sim->coders[i].thread, NULL,
			coder_thread, &sim->coders[i]);
		i++;
	}
}

void	join_threads(t_simulation *sim, pthread_t monitor_thread)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
}

void	cleanup_simulation(t_simulation *sim)
{
	int			i;
	t_dongle	*dongle;

	i = 0;
	while (i < sim->number_of_coders)
	{
		dongle = &sim->dongles[i];
		free(dongle->heap.data);
		pthread_mutex_destroy(&dongle->mutex);
		pthread_cond_destroy(&dongle->wake_cond);
		pthread_mutex_destroy(&sim->coders[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->request_mutex);
	free(sim->dongles);
	free(sim->coders);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;
	pthread_t		monitoring_thread;
	char			**parsed_args;

	parsed_args = args_verify(argc, argv);
	if (!parsed_args)
		return (1);
	if (gen_simulation(&sim, parsed_args) == 1)
	{
		fprintf(stderr, "Error: Simulation failed to be initialized.\n");
		return (1);
	}
	create_threads(&sim, &monitoring_thread);
	join_threads(&sim, monitoring_thread);
	cleanup_simulation(&sim);
}
