/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcoelho <dcoelho@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:59:17 by dcoelho           #+#    #+#             */
/*   Updated: 2026/08/26 12:14:27 by dcoelho          ###   ########.fr       */
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
	free(sim);
	while (j < i)
	{
		free(coders[j].thread);
		j++;
	}
	j = 0;
	while (j < sim->number_of_coders)
	{
		free(coders[j].r_dongle);
		free(coders[j].sim);
		coders[j].r_dongle = NULL;
		j++;
	}
	free(coders);
	exit(1);
}
