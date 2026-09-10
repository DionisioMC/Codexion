/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:59:17 by dcoelho           #+#    #+#             */
/*   Updated: 2026/09/09 17:35:28 by dcoelho          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	arg_error(void)
{
	printf("Incorrect usage\n");
	printf("Usage: ./codexion number_of_coders time_to_burnout "
		"time_to_compile time_to_debug time_to_refactor "
		"number_of_compiles_required dongle_cooldown scheduler\n");
	exit(1);
}

void	error_and_exit(t_simulation *config)
{
	free(config);
	exit(1);
}

void	thread_error(t_simulation *sim, t_coder *coders, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(coders[j].thread);
		j++;
	}
	j = 0;
	while (j < sim->number_of_coders)
	{
		free(coders[j].r_dongle);
		coders[j].r_dongle = NULL;
		pthread_mutex_destroy(&coders[j].mutex);
		j++;
	}
	free(sim);
	free(coders);
	exit(1);
}

void	mutex_error(t_simulation *sim, t_coder *coders, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(coders[j].thread);
		j++;
	}
	j = 0;
	while (j < sim->number_of_coders)
	{
		free(coders[j].r_dongle);
		coders[j].r_dongle = NULL;
		j++;
	}
	free(sim);
	free(coders);
	exit(1);
}
